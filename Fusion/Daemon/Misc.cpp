#include "stdafx.h"
#include "Misc.h"

void klog(const char* fmt, ...)
{
	char buffer[0x400];

	// Format the user message
	va_list args;
	va_start(args, fmt);
	int prefixLen = snprintf(buffer, sizeof(buffer), "[Fusion Daemon] ");

	// Append the formatted message (safely)
	if (prefixLen > 0 && prefixLen < (int)sizeof(buffer))
	{
		vsnprintf(buffer + prefixLen, sizeof(buffer) - prefixLen, fmt, args);
	}
	va_end(args);

	// Ensure null termination
	buffer[sizeof(buffer) - 1] = '\0';

	// Add newline if there's room and the message doesn't already end with one
	size_t len = strlen(buffer);
	if (len > 0 && len < sizeof(buffer) - 2)
	{
		if (buffer[len - 1] != '\n')
		{
			buffer[len] = '\n';
			buffer[len + 1] = '\0';
		}
	}

	sceKernelDebugOutText(0, buffer);
}

bool LoadModules()
{
	auto res = sceSysmoduleLoadModuleInternal(ORBIS_SYSMODULE_INTERNAL_SYSTEM_SERVICE);
	if (res != ORBIS_OK)
	{
		klog("%s: Failed to load SCE_SYSMODULE_INTERNAL_SYSTEM_SERVICE (%llX)", __FUNCTION__, res);
		return false;
	}

	ResolveLncUtils();
	
	res = sceLncUtilInitialize();
	if (res != ORBIS_OK)
	{
		klog("%s: sceLncUtilInitialize failed (%llX)", __FUNCTION__, res);
		return false;
	}

	klog("%s: Success!", __FUNCTION__);
	return true;
}