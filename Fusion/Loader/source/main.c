#include "common.h"
#include "daemon.h"



int _main(void)
{
	ResolveDynlib();
	
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