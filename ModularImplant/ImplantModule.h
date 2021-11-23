#pragma once

enum MODULE_INIT
{
	MODULE_INIT_LOCAL,
	MODULE_INIT_REMOTE,
};

typedef BOOL(*MODULE_INIT)(
	enum MODULE_INIT eModuleInitType,
	HMODULE hmThis);