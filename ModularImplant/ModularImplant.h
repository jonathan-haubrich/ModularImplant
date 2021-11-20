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

#define LOG_MSG(lpMsg)		LOG(GetStdHandle(STD_OUTPUT_HANDLE), "[+] " lpMsg)
#define LOG_ERROR(lpMsg)	LOG(GetStdHandle(STD_ERROR_HANDLE),	\
	"[!] (" __FUNCTION__ ") " lpMsg)

#define CLAMP_WCHAR_TO_UPPER(wChar)	\
	(WCHAR)(((USHORT)wChar % (L'Z' - L'A')) + (L'A'))

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
	enum MODULAR_IMPLANT_RESOURCE_IDS iFuncType);


BOOL
GetRandomPrefixW(
	PWSTR wszPrefixBuf,
	DWORD cbBufSize);

BOOL
GetRandomTempFileNameW(
	PWSTR wszTempFileNameBuf,
	SIZE_T ccFileNameBufSize);

BOOL
LoadEmbeddedLoader(
	VOID);

BOOL
WriteDataToFile(
	LPCWSTR wszFileName,
	PVOID pData,
	DWORD dwDataLen);