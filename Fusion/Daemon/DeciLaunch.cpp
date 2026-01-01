#include "stdafx.h"
#include "DeciLaunch.h"
#include "Offsets.h"

#include <RemoteThread.h>
#include <RemoteCallerManager.h>

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

	uint64_t StartDecidServerAddress = Fusion::GetRemoteAddress(shellCorePid, (int)0, Offsets::StartDecidServer);
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

	uint64_t MountFuseAddress = Fusion::GetRemoteAddress(processId, (int)0, Offsets::MountFuse);
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

	uint64_t DevPortThreadAddress = Fusion::GetRemoteAddress(sysCorePid, (int)0, Offsets::DevPortThread);
	if (DevPortThreadAddress <= 0)
	{
		Logger::Error("Failed to get DevPortThread address.");
		return;
	}
	
	// Start the devport thread that spawns the decid.elf daemon process.
	Fusion::StartPThread(sysCorePid, DevPortThreadAddress);
}