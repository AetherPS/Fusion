#include "stdafx.h"
#include "DeciLaunch.h"

#include <RemoteThread.h>
#include <RemoteCallerManager.h>

// For 9.00... TODO: Make these detect on version or use pattern.
#define StartDecidServerOffset 0x251F60
#define MountFuseOffset 0x12C0
#define DevPortThreadOffset 0x1480

void StartDecidServer()
{
	auto shellCorePid = GetPidByName("SceShellCore");
	if (shellCorePid <= 0)
	{
		Logger::Error("Failed to get pid of SceShellCore.");
		return;
	}

	auto caller = Fusion::RemoteCallerManager::GetInstance(shellCorePid);
	if (!caller)
		return;

	uint64_t StartDecidServerAddress = Fusion::GetRemoteAddress(shellCorePid, (int)0, StartDecidServerOffset);
	if (StartDecidServerAddress <= 0)
	{
		Logger::Error("Failed to get StartDecidServer address.");
		return;
	}

	caller->Call<int>(StartDecidServerAddress);
}

void MountFuse(int processId, const char* to, const char* from)
{
	auto caller = Fusion::RemoteCallerManager::GetInstance(processId);
	if (!caller)
		return;

	uint64_t MountFuseAddress = Fusion::GetRemoteAddress(processId, (int)0, MountFuseOffset);
	if (MountFuseAddress <= 0)
	{
		Logger::Error("Failed to get MountFuse address.");
		return;
	}

	caller->Call<void>(MountFuseAddress, to, from);
}

void StartDECI()
{
	auto decidPid = GetPidByName("decid.elf");
	if (decidPid > 0)
	{
		Logger::Warn("DECI already running.");
		return;
	}

	auto sysCorePid = GetPidByName("SceSysCore");
	if (sysCorePid <= 0)
	{
		Logger::Error("Failed to get pid of SceSysCore.");
		return;
	}

	// Start the DECI server that runs on SceShellCore.
	StartDecidServer();

	// Mount the fuse directories.
	MountFuse(sysCorePid, "/hostapp", "/dev/fuse0");
	MountFuse(sysCorePid, "/host", "/dev/fuse1");

	uint64_t DevPortThreadAddress = Fusion::GetRemoteAddress(sysCorePid, (int)0, DevPortThreadOffset);
	if (DevPortThreadAddress <= 0)
	{
		Logger::Error("Failed to get DevPortThread address.");
		return;
	}
	
	// Start the devport thread that spawns the decid.elf daemon process.
	Fusion::StartPThread(sysCorePid, DevPortThreadAddress);
}