#include "Common.h"
#include "Fusion.h"

#include "Patches.h"
#include "DirectMemory.h"

void InitFusion()
{
	InstallPatches();				// Install all needed kernel patches.
	FusionSysctl::Init();			// Initialize sysctl interface (registers feature flag handlers).
	MkDir("/data/Fusion", 0777);	// Ensure the fusion dir is made.
	dmamini_initialize_ioctl();
	DetourMemoryPool::Init(PAGE_SIZE); // 4 MB Pool
}
