#include "stdafx.h"
#include "SystemPatcher.h"

void SystemPatcher::Init()
{

}

void SystemPatcher::ShellCore()
{
	// // Get the module handles.
	// int numModules;
	// int handles[256];
	// dynlib_get_list(p, handles, 256, &numModules);
	// 
	// // Get the info for the main executable.
	// OrbisLibraryInfo info;
	// dynlib_get_info(p, handles[0], &info);
	// uint64_t shellCoreBase = info.MapBase;
	// 
	// // Make sure we found the address.
	// if (shellCoreBase == 0)
	// {
	// 	kprintf("InstallShellCorePatches(): Failed to find the base address for \"SceShellCore.elf\".\n");
	// 	return;
	// }
	// 
	// uint8_t xor__eax_eax[5] = { 0x31, 0xC0, 0xEB, 0x01 };
	// 
	// // sceKernelIsGenuineCEX
	// ReadWriteProcessMemory(p->p_threads.tqh_first, p, (void*)(shellCoreBase + addr_sceKernelIsGenuineCEX1), xor__eax_eax, sizeof(xor__eax_eax), true);
	// ReadWriteProcessMemory(p->p_threads.tqh_first, p, (void*)(shellCoreBase + addr_sceKernelIsGenuineCEX2), xor__eax_eax, sizeof(xor__eax_eax), true);
	// ReadWriteProcessMemory(p->p_threads.tqh_first, p, (void*)(shellCoreBase + addr_sceKernelIsGenuineCEX3), xor__eax_eax, sizeof(xor__eax_eax), true);
	// ReadWriteProcessMemory(p->p_threads.tqh_first, p, (void*)(shellCoreBase + addr_sceKernelIsGenuineCEX4), xor__eax_eax, sizeof(xor__eax_eax), true);
	// 
	// // sceKernelIsAssistMode
	// ReadWriteProcessMemory(p->p_threads.tqh_first, p, (void*)(shellCoreBase + addr_sceKernelIsAssistMode1), xor__eax_eax, sizeof(xor__eax_eax), true);
	// ReadWriteProcessMemory(p->p_threads.tqh_first, p, (void*)(shellCoreBase + addr_sceKernelIsAssistMode2), xor__eax_eax, sizeof(xor__eax_eax), true);
	// ReadWriteProcessMemory(p->p_threads.tqh_first, p, (void*)(shellCoreBase + addr_sceKernelIsAssistMode3), xor__eax_eax, sizeof(xor__eax_eax), true);
	// ReadWriteProcessMemory(p->p_threads.tqh_first, p, (void*)(shellCoreBase + addr_sceKernelIsAssistMode4), xor__eax_eax, sizeof(xor__eax_eax), true);
	// 
	// // Enable fake pkg.
	// ReadWriteProcessMemory(p->p_threads.tqh_first, p, (void*)(shellCoreBase + addr_enableFpkg), (void*)"\xE9\x98\x00\x00\x00", 8, true);
	// 
	// // fake to free.
	// ReadWriteProcessMemory(p->p_threads.tqh_first, p, (void*)(shellCoreBase + addr_fakeText), (void*)"free", 4, true);
	// 
	// // Enable mounting data into sandboxes.
	// ReadWriteProcessMemory(p->p_threads.tqh_first, p, (void*)(shellCoreBase + addr_mountDataIntoSandbox), (void*)"\x31\xC0\xFF\xC0\x90", 5, true);
	// 
	// // Patch Pkg Update Checks
	// ReadWriteProcessMemory(p->p_threads.tqh_first, p, (void*)(shellCoreBase + addr_disablePkgPatchCheck1), (void*)"\xEB", 1, true);
	// ReadWriteProcessMemory(p->p_threads.tqh_first, p, (void*)(shellCoreBase + addr_disablePkgPatchCheck2), (void*)"\xEB", 1, true);
	// ReadWriteProcessMemory(p->p_threads.tqh_first, p, (void*)(shellCoreBase + addr_disablePkgPatchCheck3), (void*)"\x48\x31\xC0\xC3", 4, true);
}

void SystemPatcher::ShellUI()
{
	auto pid = GetPidByName("SceShellUI");
	
	OrbisLibraryInfo libraries[255];
	int libraryCount;
	int res = Fusion::GetLibraryList(pid, libraries, 255, &libraryCount);
	if (res != 0)
	{
		Logger::Error("Failed to get process libraries for pid %d for reason %llX", pid, res);
		return;
	}

	for (int i = 0; i < libraryCount; i++)
	{
		auto baseAddress = libraries[i].MapBase;

		if (strstr(libraries[i].Path, "libkernel_sys.sprx"))
		{
			Fusion::ReadWriteMemory(pid, baseAddress + Offsets::DebugMenuPatch1, "\xB8\x01\x00\x00\x00\xC3", 6, true); // sceKernelGetDebugMenuModeForRcmgr
			Fusion::ReadWriteMemory(pid, baseAddress + Offsets::DebugMenuPatch2, "\xB8\x01\x00\x00\x00\xC3", 6, true); // sceKernelGetUtokenStoreModeForRcmgr
		}
	}
}
