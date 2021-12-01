#include <Windows.h>

#include <stdio.h>

#include "ModularImplant.h"

BOOL
GetFileContents(
	PWSTR pszFileName,
	LPVOID *ppFileContents,
	PDWORD pdwFileSize)
{
	HANDLE hFile = INVALID_HANDLE_VALUE;
	DWORD dwFileSizeLow = 0, dwFileSizeHigh = 0, dwBytesRead = 0;
	PVOID pbFileContents = NULL;

	if (!(ppFileContents && pdwFileSize))
	{
		fwprintf(stderr, L"[-] (%s) Invalid args\r\n", __FUNCTIONW__);
		return FALSE;
	}

	hFile = CreateFileW(pszFileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		fwprintf(stderr, L"[-] (%s) CreateFile failed: %d\r\n", __FUNCTIONW__, GetLastError());
		return FALSE;
	}

	dwFileSizeLow = GetFileSize(hFile, &dwFileSizeHigh);
	if (0 != dwFileSizeHigh)
	{
		fwprintf(stderr, L"[-] (%s) File size too large\r\n", __FUNCTIONW__);
		CloseHandle(hFile);
		return FALSE;
	}
	else if (INVALID_FILE_SIZE == dwFileSizeLow)
	{
		fwprintf(stderr, L"[-] (%s) GetFileSize failed: %d\r\n", __FUNCTIONW__, GetLastError());
		CloseHandle(hFile);
		return FALSE;
	}

	pbFileContents = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwFileSizeLow);
	if (NULL == pbFileContents)
	{
		fwprintf(stderr, L"[-] (%s) HeapAlloc failed: %d\r\n", __FUNCTIONW__, GetLastError());
		CloseHandle(hFile);
		return FALSE;
	}

	if (FALSE == ReadFile(hFile, pbFileContents, dwFileSizeLow, &dwBytesRead, NULL))
	{
		fwprintf(stderr, L"[-] (%s) ReadFile failed: %d\r\n", __FUNCTIONW__, GetLastError());
		CloseHandle(hFile);
		return FALSE;
	}

	*ppFileContents = pbFileContents;
	*pdwFileSize = dwFileSizeLow;

	return TRUE;
}

INT
wmain(INT argc, PWSTR argv[])
{
	if (argc < 4)
	{
		fwprintf(stderr, L"Usage: %s <implant_file> <resource_id> <module_file>\r\n", argv[0]);
		return EXIT_FAILURE;
	}

	PWSTR pszImplantFileName = argv[1],
		pszResourceId = argv[2],
		pszModuleFileName = argv[3];

	HANDLE hImplant = NULL;

	PVOID pModuleContents = NULL;

	DWORD dwModuleSize = 0,
		dwResourceId = _wtoi(pszResourceId);

	hImplant = BeginUpdateResourceW(pszImplantFileName, FALSE);
	if (NULL == hImplant)
	{
		fwprintf(stderr, L"[-] (%s) BeginUpdateResource failed: %d\r\n",
			__FUNCTIONW__,
			GetLastError());
		return EXIT_FAILURE;
	}

	if (FALSE == GetFileContents(pszModuleFileName, &pModuleContents, &dwModuleSize))
	{
		fwprintf(stderr, L"[-] (%s) GetFileContents failed: %d\r\n",
			__FUNCTIONW__,
			GetLastError());
		return EXIT_FAILURE;
	}
	else
	{
		wprintf(L"[+] Got file contents of size: %d\r\n", dwModuleSize);
	}

	if (FALSE == UpdateResourceW(hImplant,
		RT_RCDATA,
		MAKEINTRESOURCEW(dwResourceId),
		LANG_SYSTEM_DEFAULT,
		pModuleContents,
		dwModuleSize))
	{
		fwprintf(stderr, L"[-] (%s) UpdateResource failed: %d\r\n",
			__FUNCTIONW__,
			GetLastError());
		HeapFree(GetProcessHeap(), 0, pModuleContents);
		EndUpdateResourceW(hImplant, TRUE);
		return EXIT_FAILURE;
	}

	if (FALSE == EndUpdateResourceW(hImplant, FALSE))
	{
		fwprintf(stderr, L"[-] (%s) EndUpdateResource failed: %d\r\n",
			__FUNCTIONW__,
			GetLastError());
		HeapFree(GetProcessHeap(), 0, pModuleContents);
		EndUpdateResourceW(hImplant, TRUE);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}