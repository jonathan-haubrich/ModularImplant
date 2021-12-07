#pragma once

#include <Windows.h>

#include <bcrypt.h>
#pragma comment(lib, "Bcrypt.lib")

#include "ModularImplant.h"

enum CRYPTO_MODULE_RESOURCE_IDS
{
	CMRID_PrivateKey = 0,
	CMRID_PublicKey,
};