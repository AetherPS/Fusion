#include "stdafx.h"
#include "SystemPatcher.h"

void SystemPatcher::Init()
{

}

void SystemPatcher::ShellCore()
{

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
