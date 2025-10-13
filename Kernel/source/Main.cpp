#include "Common.h"
#include "FakePkgs.h"
#include "FakeSelfs.h"
#include "Watcher.h"
#include "DirectMemory.h"
#include "DevActSpoofer.h"
#include "LibraryReplacer.h"
#include <DipSwitchSpoofer.h>
#include <TTYRedirector.h>

uint8_t* KernelBase;

extern "C" int _main(uint64_t* p)
{
	KernelBase = (uint8_t*)Readmsr(0xC0000082) - (addr_Xfast_syscall + 0xFFFFFFFF82200000);
	KernelBase += 0xFFFFFFFF82200000;

	ResolveFunctions();
	InstallPatches();

	kprintf("Hello, World!\n");
	PrintFeatureFlags();

	// Ensure the fusion dir is made.
	kern_mkdir(CurrentThread(), "/data/Fusion", 0, 0777);

	kprintf("Initializing Watcher...");
	Watcher::Init();
	kprintf("Done.\n");

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
	auto fuseResult = fuse_loader(NULL, 0, NULL);
	kprintf("Done. Result: %d\n", fuseResult);
#endif

#ifdef FF_TTYRedirect
	kprintf("Initializing TTY Redirector...");
	TTYRedirector::Init();
	kprintf("Done.\n");
#endif

	NotifyCustom("https://imgur.com/y8JEqtu.png", "Fusion %s Loaded\n%s", "3", "Uber haxor edition.");

	return 0;
}