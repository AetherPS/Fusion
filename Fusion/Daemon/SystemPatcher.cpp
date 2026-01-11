#include "stdafx.h"
#include "SystemPatcher.h"

void SystemPatcher::Init()
{
	ShellCore();
}

void SystemPatcher::ShellCore()
{
	auto pid = GetPidByName("SceShellCore");

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

		if (libraries[i].Handle == 0)
		{
			// sceKernelIsGenuineCEX
			Fusion::ReadWriteMemory(pid, baseAddress + Offsets::IsGenuineCEX1, (void*)"\x31\xC0\xEB\x01", 4, true);
			Fusion::ReadWriteMemory(pid, baseAddress + Offsets::IsGenuineCEX2, (void*)"\x31\xC0\xEB\x01", 4, true);
			Fusion::ReadWriteMemory(pid, baseAddress + Offsets::IsGenuineCEX3, (void*)"\x31\xC0\xEB\x01", 4, true);
			Fusion::ReadWriteMemory(pid, baseAddress + Offsets::IsGenuineCEX4, (void*)"\x31\xC0\xEB\x01", 4, true);

			// sceKernelIsAssistMode
			Fusion::ReadWriteMemory(pid, baseAddress + Offsets::IsAssistMode1, (void*)"\x31\xC0\xEB\x01", 4, true);
			Fusion::ReadWriteMemory(pid, baseAddress + Offsets::IsAssistMode2, (void*)"\x31\xC0\xEB\x01", 4, true);
			Fusion::ReadWriteMemory(pid, baseAddress + Offsets::IsAssistMode3, (void*)"\x31\xC0\xEB\x01", 4, true);
			Fusion::ReadWriteMemory(pid, baseAddress + Offsets::IsAssistMode4, (void*)"\x31\xC0\xEB\x01", 4, true);

			// Enable fake pkg.
			Fusion::ReadWriteMemory(pid, baseAddress + Offsets::EnableFakePkg, (void*)"\xE9\x98\x00\x00\x00", 8, true);

			// fake to free.
			Fusion::ReadWriteMemory(pid, baseAddress + Offsets::FakeText, (void*)"free", 4, true);

			// Enable mounting data into sandboxes.
			Fusion::ReadWriteMemory(pid, baseAddress + Offsets::MountDataIntoSandbox, (void*)"\x31\xC0\xFF\xC0\x90", 5, true);

			// Patch Pkg Update Checks
			Fusion::ReadWriteMemory(pid, baseAddress + Offsets::DisablePkgPatchCheck1, (void*)"\xEB", 1, true);
			Fusion::ReadWriteMemory(pid, baseAddress + Offsets::DisablePkgPatchCheck2, (void*)"\xEB", 1, true);
			Fusion::ReadWriteMemory(pid, baseAddress + Offsets::DisablePkgPatchCheck3, (void*)"\x48\x31\xC0\xC3", 4, true);
		}
	}
}
