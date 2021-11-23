#include <Windows.h>

#include <stdio.h>

#include "ModularImplant.h"

INT
wmain(INT argc, PWSTR argv[])
{
	if (argc < 4)
	{
		fwprintf(stderr, L"Usage: %s <module_file> <resource_id> <resource_value>\r\n", argv[0]);
		return EXIT_FAILURE;
	}

	PWSTR pszModuleFileName = argv[1],
		pszResourceId = argv[2],
		pszResourceValue = argv[3];

	HANDLE hModule = NULL;

	DWORD dwResourceId = _wtoi(pszResourceId);

	hModule = BeginUpdateResourceW(pszModuleFileName, FALSE);
	if (NULL == hModule)
	{
		fwprintf(stderr, L"[-] (%s) BeginUpdateResource failed: %d\r\n",
			__FUNCTIONW__,
			GetLastError());
		return EXIT_FAILURE;
	}

	if (FALSE == UpdateResourceW(hModule,
		RT_RCDATA,
		MAKEINTRESOURCEW(dwResourceId),
		LOCALE_IDEFAULTLANGUAGE,
		pszResourceValue,
		lstrlenW(pszResourceValue) * sizeof(WCHAR)))
	{
		fwprintf(stderr, L"[-] (%s) UpdateResource failed: %d\r\n",
			__FUNCTIONW__,
			GetLastError());
		EndUpdateResourceW(hModule, TRUE);
		return EXIT_FAILURE;
	}

	if (FALSE == EndUpdateResourceW(hModule, FALSE))
	{
		fwprintf(stderr, L"[-] (%s) EndUpdateResource failed: %d\r\n",
			__FUNCTIONW__,
			GetLastError());
		EndUpdateResourceW(hModule, TRUE);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}