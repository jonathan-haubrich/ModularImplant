#pragma once

#include <Windows.h>

#define LOG(hFileStream, lpMsg)		\
{									\
	WriteFile(hFileStream,			\
		lpMsg  "\r\n",				\
		sizeof(lpMsg),				\
		NULL,						\
		NULL						\
	);								\
}

#define LOG_MSG(lpMsg)		LOG(GetStdHandle(STD_OUTPUT_HANDLE), "[+] " lpMsg)
#define LOG_ERROR(lpMsg)	LOG(GetStdHandle(STD_ERROR_HANDLE),	\
	"[-] (" __FUNCTIONW__ ") " lpMsg)

enum MODULAR_IMPLANT_RESOURCE_IDS
{
	Loader,
};

typedef VOID(*IMPLANT_FUNC) ();
typedef VOID(*IMPLANT_LOADER) (LPCWSTR wszLibFileName, PVOID* lpOptArgs);

typedef struct _IMPLANT_CONFIG
{
	IMPLANT_LOADER fnLoader;
} IMPLANT_CONFIG, *PIMPLANT_CONFIG;

typedef VOID(*IMPLANT_CALLBACK)(PIMPLANT_CONFIG pImplantConfig,
	IMPLANT_FUNC fnFunc,
	MODULAR_IMPLANT_RESOURCE_IDS iFuncType);