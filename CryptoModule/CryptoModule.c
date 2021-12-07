#include "CryptoModule.h"

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
Init(
	enum MODULE_INIT eModuleInitType,
	HMODULE hmThis)
{
	/*
	*	Init will:
	*		1. Pull key from resources (public for remote, private for local)
	*		2. Base64 decode key
	*		3. Import key from binary blob
	*		4. Perform handshake with distant end to get session key
	*			Handshake involves:
	*				a. Local end generates symmetric key
	*				b. encrypts and sends to remote
	*				c. remote decrypts key
	*				d. encrypts random data with asym and sym keys
	*				e. sends both copies of encrypted data to local, local verifies
	*				f. local encrypts random data with asym and sym
	*				g. remote decrypts both and verifies
	*				h. handshake complete
	*/

	PVOID pResourceData = NULL,
		pBinaryKeyData = NULL;
	DWORD dwResourceDataLen = 0,
		dwBinaryKeyDataLen = 0;
	BOOL fResult = TRUE;

	// Pull base64 encoded key data from resources
	if (FALSE == GetResourceData(hmThis,
		CMRID_PrivateKey,
		&pResourceData,
		&dwResourceDataLen))
	{
		LOG_ERROR("GetResourceData failed");
		fResult = FALSE;
		goto end;
	}

	// Decode base64 to binary data
	if (FALSE == CryptStringToBinaryW((LPCWSTR)pResourceData,
		dwResourceDataLen,
		CRYPT_STRING_BASE64,
		&pBinaryKeyData,
		&dwBinaryKeyDataLen,
		NULL,
		NULL))
	{
		LOG_ERROR("CryptStringToBinary failed");
		fResult = FALSE;
		goto end;
	}

end:
	return fResult;
}