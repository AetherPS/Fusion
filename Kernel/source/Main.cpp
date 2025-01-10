#include "Common.h"
#include "FakePkgs.h"
#include "FakeSelfs.h"
#include "Watcher.h"

uint8_t* KernelBase;

extern "C" int _main(uint64_t* p)
{
	KernelBase = (uint8_t*)Readmsr(0xC0000082) - (addr_Xfast_syscall + 0xFFFFFFFF82200000);
	KernelBase += 0xFFFFFFFF82200000;

	ResolveFunctions();
	InstallPatches();

	kprintf("Hello, World! We are now $Root.\n");

	kprintf("Initializing Watcher...");
	Watcher::Init();
	kprintf("Done.\n");

	kprintf("Initializing Fake Packages...");
	FakePkgs::Init();
	kprintf("Done.\n");

	kprintf("Initializing Fake Selfs...");
	FakeSelf::Init();
	kprintf("Done.\n");

	kprintf("Starting Fuse...");
	auto fuseResult = fuse_loader(NULL, 0, NULL);
	kprintf("Done. Result: %d\n", fuseResult);

	NotifyCustom("https://imgur.com/y8JEqtu.png", "Fusion 3 Loaded\nMade by Master Odin & Faultz");

	return 0;
}