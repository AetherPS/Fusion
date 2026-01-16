#include "Common.h"
#include "Fusion.h"

#include "FakePkgs.h"
#include "FakeSelfs.h"
#include "DirectMemory.h"
#include "DevActSpoofer.h"
#include "LibraryReplacer.h"
#include "DipSwitchSpoofer.h"
#include "TTYRedirector.h"

uint8_t* KernelBase;

void InitFusion()
{
	// Resolve the Kernel Base.
	KernelBase = (uint8_t*)Readmsr(0xC0000082) - addr_Xfast_syscall;

	ResolveFunctions();				// Resolve all needed functions.
	FusionSysctl::Init();			// Initialize sysctl interface.
	PrintFeatureFlags();			// Print the active feature flags.
	MkDir("/data/Fusion", 0777);	// Ensure the fusion dir is made.

	printf("Initializing Detour Memory Pool...");
	DetourMemoryPool::Init(PAGE_SIZE); // 4 MB Pool
	printf("Done.\n");

	dmamini_initialize_ioctl();

#ifdef FF_HomeBrew
	printf("Initializing Fake Packages...");
	FakePkgs::Init();
	printf("Done.\n");

	printf("Initializing Fake Selfs...");
	FakeSelf::Init();
	printf("Done.\n");
#endif

	printf("Initializing Direct Memory Reservation...");
	DirectMemory::Init();
	printf("Done.\n");

#ifdef FF_Driver
	printf("Initializing Driver...");
	FusionDriver::Init();
	printf("Done.\n");
#endif

	printf("Initializing Dev Activation Spoofer...");
	DevActSpoofer::Init();
	printf("Done.\n");

	printf("Initializing Dip Switch Spoofer...");
	DipSwitchSpoofer::Init();
	printf("Done.\n");

	printf("Initializing Library Replacer...");
	LibraryReplacer::Init();
	printf("Done.\n");

#ifdef FF_Fuse
	printf("Starting Fuse...");
	printf(fuse_loader(NULL, 0, NULL) == 0 ? "Done.\n" : "Failed.\n");
#endif

	printf("Initializing TTY Redirector...");
	TTYRedirector::Init();
	printf("Done.\n");

}
