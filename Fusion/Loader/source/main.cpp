#include "common.h"
#include "kernel.h"
#include "plugins.h"
#include "config.h"

int main(void)
{
	ascii();

	// Decompress and load the kernel module.
	if (!LoadKernel())
	{
		klog("Failed to load the kernel module.\n");
		return 0;
	}

	// Apply feature configuration via sysctls
	ApplyFeatureConfiguration();

	if (InstallPlugins() != 0)
	{
		klog("Failed to install the plugins.\n");
		return 0;
	}

	if (LoadPlugins() != 0)
	{
		klog("Failed to load the plugins.\n");
		return 0;
	}

	return 0;
}