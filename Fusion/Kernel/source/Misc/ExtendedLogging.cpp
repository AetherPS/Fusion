#include "Common.h"
#include "ExtendedLogging.h"

Detour* ExtendedLogging::CallStackPrintfDetour;
void ExtendedLogging::CallStackPrintfHook(void* a1, void* a2, const char* fmt, uint64_t address)
{
	OrbisLibraryInfo libraryInfo;
	auto td = CurrentThread();
	auto res = GetModuleInfoFromAddr(td->td_proc, address, &libraryInfo);

	if (res != 0)
	{
		printf("# 0x%016lX\n", address);
		return;
	}

	printf("# 0x%016lX  %-32s  +0x%06lX\n", address, basename(libraryInfo.Path), address - libraryInfo.MapBase);
}

Detour* ExtendedLogging::FaultAddressPrintfDetour;
void ExtendedLogging::FaultAddressPrintfHook(void* a1, void* a2, const char* fmt, uint64_t address)
{
	OrbisLibraryInfo libraryInfo;
	auto td = CurrentThread();
	auto res = GetModuleInfoFromAddr(td->td_proc, address, &libraryInfo);

	if (res != 0)
	{
		printf("# fault address: %016lx\n", address);
		return;
	}

	printf("# fault address: %016lx  %s  +0x%06lX\n", address, basename(libraryInfo.Path), address - libraryInfo.MapBase);
}

void ExtendedLogging::Init()
{
	Detour::DetourCall(&CallStackPrintfDetour, g_KernelAddrs.CallStackPrintfHook, (void*)CallStackPrintfHook);
	Detour::DetourCall(&FaultAddressPrintfDetour, g_KernelAddrs.FaultAddressPrintfHook, (void*)FaultAddressPrintfHook);
}

void ExtendedLogging::Term() 
{
	delete CallStackPrintfDetour;
	delete FaultAddressPrintfDetour;
}