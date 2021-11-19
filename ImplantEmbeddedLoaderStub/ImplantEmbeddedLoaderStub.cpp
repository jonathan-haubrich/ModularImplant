#include <Windows.h>

#include "ModularImplant.h"

#pragma section("EmbeddedLoaderStub",read,execute)
/*
* What is the function signature for the embedded loader?
*/
VOID
EmbeddedLoaderStub(
	PIMPLANT_CONFIG pImplantConfig,
	IMPLANT_CALLBACK fnRegisterLoader)
{

	return;
}