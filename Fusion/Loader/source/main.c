#include "common.h"
#include "daemon.h"

int _main(void)
{
	if (ResolveDynlib() != 0)
	{
		klog("[Fusion] Cant load dynlib look up failed.\n");
		return 1;
	}

	// Check if Fusion Driver is already loaded.
	if (FileExist("/dev/Fusion"))
	{
		klog("Fusion already loaded.\n");
		return 1;
	}

	ascii();
	LoadKernel();
	InstallDaemon();

	return 0;
}