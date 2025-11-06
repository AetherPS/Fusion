#include "Common.h"
#include "Fusion.h"

#include "FakePkgs.h"
#include "FakeSelfs.h"
#include "Watcher.h"
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
	InstallPatches();				// Install all needed patches.
	PrintFeatureFlags();			// Print the active feature flags.
	MkDir("/data/Fusion", 0777);	// Ensure the fusion dir is made.

	kprintf("Initializing Detour Memory Pool...");
	DetourMemoryPool::Init(PAGE_SIZE); // 4 MB Pool
	kprintf("Done.\n");

	kprintf("Initializing Watcher...");
	Watcher::Init();
	kprintf("Done.\n");

#ifdef FF_BETA
	dmamini_initialize_ioctl();
#endif

#ifdef FF_HomeBrew
	kprintf("Initializing Fake Packages...");
	FakePkgs::Init();
	kprintf("Done.\n");

	kprintf("Initializing Fake Selfs...");
	FakeSelf::Init();
	kprintf("Done.\n");
#endif

#ifdef FF_DirectMemory
	kprintf("Initializing Direct Memory Reservation...");
	DirectMemory::Init();
	kprintf("Done.\n");
#endif

#ifdef FF_Driver
	kprintf("Initializing Driver...");
	FusionDriver::Init();
	kprintf("Done.\n");
#endif

#ifdef FF_DevAct
	kprintf("Initializing Dev Activation Spoofer...");
	DevActSpoofer::Init();
	kprintf("Done.\n");
#endif

#ifdef FF_Dipsw
	kprintf("Initializing Dip Switch Spoofer...");
	DipSwitchSpoofer::Init();
	kprintf("Done.\n");
#endif

#ifdef FF_LibraryReplacer
	kprintf("Initializing Library Replacer...");
	LibraryReplacer::Init();
	kprintf("Done.\n");
#endif

#ifdef FF_Fuse
	kprintf("Starting Fuse...");
	kprintf(fuse_loader(NULL, 0, NULL) == 0 ? "Done.\n" : "Failed.\n");
#endif

#ifdef FF_TTYRedirect
	kprintf("Initializing TTY Redirector...");
	TTYRedirector::Init();
	kprintf("Done.\n");
#endif

}
