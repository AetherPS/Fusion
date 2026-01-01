#include "stdafx.h"
#include "Misc.h"

bool LoadModules()
{
	auto res = sceSysmoduleLoadModuleInternal(SCE_SYSMODULE_INTERNAL_SYSTEM_SERVICE);
	if (res != SCE_OK)
	{
		Logger::Error("%s: Failed to load SCE_SYSMODULE_INTERNAL_SYSTEM_SERVICE (%llX)", __FUNCTION__, res);
		return false;
	}

	res = sceLncUtilInitialize();
	if (res != SCE_OK)
	{
		Logger::Error("%s: sceLncUtilInitialize failed (%llX)", __FUNCTION__, res);
		return false;
	}

	Logger::Success("%s: Success!", __FUNCTION__);
	return true;
}
