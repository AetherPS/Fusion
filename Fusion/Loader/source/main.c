#include "common.h"
#include "kernel.h"
#include "plugins.h"

int main(void)
{
	ascii();

	// Check if Fusion Driver is already loaded.
	if (FileExist("/dev/Fusion"))
	{
		klog("Fusion already loaded.\n");
		return 1;
	}

	// Decompress and load the kernel module.
	if (LoadKernel() != 0)
	{
		klog("Failed to load the kernel module.\n");
		return 1;
	}

	if (InstallPlugins() != 0)
	{
		klog("Failed to install the plugins.\n");
		return 1;
	}

	if (LoadPlugins() != 0)
	{
		klog("Failed to load the plugins.\n");
		return 1;
	}

	return 0;
}