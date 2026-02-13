#include "common.h"
#include "kernel.h"
#include "plugins.h"
#include "config.h"

int main(void)
{
	ascii();

	// Check if Fusion is already loaded
	if (IsAlreadyLoaded())
	{
		klog("Fusion already loaded - reloading configuration only\n");
		ApplyFeatureConfiguration();
		klog("Configuration reloaded successfully\n");
		kill(getpid(), SIGKILL);
		return 0;
	}

	// Decompress and load the kernel module.
	if (!LoadKernel())
	{
		klog("Failed to load the kernel module.\n");
		kill(getpid(), SIGKILL);
		return 0;
	}

	// Apply feature configuration via sysctls
	ApplyFeatureConfiguration();

	if (InstallPlugins() != 0)
	{
		klog("Failed to install the plugins.\n");
		kill(getpid(), SIGKILL);
		return 0;
	}

	if (LoadPlugins() != 0)
	{
		klog("Failed to load the plugins.\n");
		kill(getpid(), SIGKILL);
		return 0;
	}

	// Mark Fusion as loaded
	MarkAsLoaded();
	klog("Fusion loaded successfully\n");

	kill(getpid(), SIGKILL);
	return 0;
}