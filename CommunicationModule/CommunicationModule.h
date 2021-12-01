#pragma once

#define MAX_IP_STR_LEN		16
#define MAX_PORT_STR_LEN	6
#define MAX_IPPORT_STR_LEN	(MAX_IP_STR_LEN + MAX_PORT_STR_LEN + 1) // +1 for colon

enum COMMUNICATION_MODULE_RESOURCE_IDS
{
	CMRID_BindIpAddressPort,
	CMRID_RemoteIpAddressPort,
};

__declspec(dllexport)
BOOL
Init(
	enum MODULE_INIT eModuleInitType,
	HMODULE hmThis);