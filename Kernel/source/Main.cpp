#include "Common.h"

uint8_t* KernelBase;

extern "C" int _main(uint64_t* p)
{
	KernelBase = (uint8_t*)Readmsr(0xC0000082) - (addr_Xfast_syscall + 0xFFFFFFFF82200000);
	KernelBase += 0xFFFFFFFF82200000;

	// ResolveFunctions();
	InstallPatches();

	void(*kprintf)(const char* fmt, ...) = (decltype(kprintf))(KernelBase + addr_kprintf);
	kprintf("Hello, World from Kernel ELF.\n");

	return 0;
}