
#include <Windows.h>

#include "ModularImplant.h"

__declspec(dllexport) BOOL
LoadImplantModule(
	PVOID pModuleData,
	DWORD cbModuleLen);

BOOL WINAPI
DllMain(
	HINSTANCE hinstDLL,
	DWORD fdwReason,
	LPVOID lpReserver)
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
	PVOID pModuleData,
	DWORD cbModuleLen)
{
	//if(FALSE == WriteDataToFile())

	return TRUE;
}