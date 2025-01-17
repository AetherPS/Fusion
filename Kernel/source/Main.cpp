#include "Common.h"
#include "FakePkgs.h"
#include "FakeSelfs.h"
#include "Watcher.h"
#include "DirectMemory.h"
#include "DevActSpoofer.h"
#include "LibraryReplacer.h"

uint8_t* KernelBase;

bool ShouldPrint(int entry)
{
	if (entry < 0x78010100)
		return false;

	switch (entry)
	{
	case 0x78026400:
	case 0x7802AF00:
	case 0x2020000:
	case 0x5020000:
	case 0x9400200:
	case 0x20800000:
	case 0x9400300:
	case 0x20A0000:
	case 0x9C00100:
	case 0x2110000:
	case 0x9C04100:
	case 0x5010000:
	case 0x7802B400:
	case 0x14190600:
		return false;

	default:
		return true;
	}
}

int dispTitleId = 1;
int screenshot = 0;
int fakeVersion = 1;

Detour32* sceRegMgrGetIntDetour;
int sceRegMgrGetIntHook(int entry, int* valueOut)
{
	if (ShouldPrint(entry))
		kprintf("sceRegMgrGetIntHook: %llX\n", entry);

	// SCE_REGMGR_ENT_KEY_DEVENV_TOOL_SHELLUI_screenshot
	if (entry == 0x7802C914)
	{
		*valueOut = screenshot;
		return 0;
	}
	
	// SCE_REGMGR_ENT_KEY_DEVENV_TOOL_SHELLUI_disp_titleid
	if (entry == 0x7802C916)
	{
		*valueOut = dispTitleId;
		return 0;
	}

	// SCE_REGMGR_ENT_KEY_NP_fake_version
	if (entry == 0x19A50000)
	{
		*valueOut = fakeVersion;
		return 0;
	}

	if (entry == 0x2800300)
	{
		*valueOut = 400;
		return 0;
	}

	return sceRegMgrGetIntDetour->Invoke<int>(entry, valueOut);
}

Detour32* sceRegMgrSetIntDetour;
int sceRegMgrSetIntHook(int entry, int value)
{
	if (ShouldPrint(entry))
		kprintf("sceRegMgrSetIntHook: %llX\n", entry);

	// SCE_REGMGR_ENT_KEY_DEVENV_TOOL_SHELLUI_screenshot
	if (entry == 0x7802C914)
	{
		screenshot = value;
		return 0;
	}

	// SCE_REGMGR_ENT_KEY_DEVENV_TOOL_SHELLUI_disp_titleid
	if (entry == 0x7802C916)
	{
		dispTitleId = value;
		return 0;
	}

	// SCE_REGMGR_ENT_KEY_NP_fake_version
	if (entry == 0x19A50000)
	{
		fakeVersion = value;
		return 0;
	}

	return sceRegMgrSetIntDetour->Invoke<int>(entry, value);
}

extern "C" int _main(uint64_t* p)
{
	KernelBase = (uint8_t*)Readmsr(0xC0000082) - (addr_Xfast_syscall + 0xFFFFFFFF82200000);
	KernelBase += 0xFFFFFFFF82200000;

	ResolveFunctions();
	InstallPatches();

	kprintf("Hello, World! We are now #Root.\n");
	PrintFeatureFlags();

	sceRegMgrGetIntDetour = new Detour32(KernelBase + 0x4E9DD0, sceRegMgrGetIntHook);
	sceRegMgrSetIntDetour = new Detour32(KernelBase + 0x4E8B10, sceRegMgrSetIntHook);

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

	NotifyCustom("https://imgur.com/y8JEqtu.png", "Fusion 3 Loaded\nMade by Master Odin & Faultz");

	return 0;
}