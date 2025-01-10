#include "Common.h"
#include "FakePkgs.h"
#include "FakeSelfs.h"

uint8_t* KernelBase;

extern "C" int _main(uint64_t* p)
{
	KernelBase = (uint8_t*)Readmsr(0xC0000082) - (addr_Xfast_syscall + 0xFFFFFFFF82200000);
	KernelBase += 0xFFFFFFFF82200000;

	ResolveFunctions();
	InstallPatches();

	kprintf("Hello, World from Kernel ELF.\n");

	FakePkgs::Init();
	FakeSelf::Init();

	kprintf("Homebrew is Enabled!\n");

	NotifyCustom("https://imgur.com/y8JEqtu.png", "Fusion 3 Loaded\nMade by Master Odin & Faultz");

	return 0;
}