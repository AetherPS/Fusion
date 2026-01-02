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

void StartDevPortThread(int sysCorePid)
{
	uint64_t DevPortThreadAddress = Fusion::GetRemoteAddress(sysCorePid, (int)0, Offsets::DevPortThread);
	if (DevPortThreadAddress <= 0)
	{
		Logger::Error("Failed to get DevPortThread address.");
		return;
	}

	uint64_t scePthreadCreateAddress = 0;
	auto result = Fusion::Resolve(sysCorePid, 8193, "libkernel", "scePthreadCreate", 0, &scePthreadCreateAddress);
	if (result != 0 || scePthreadCreateAddress == 0)
	{
		Logger::Error("Failed to resolve scePthreadCreate for reason %llX.", result);
		return;
	}

	uint64_t scePthreadDetachAddress = 0;
	result = Fusion::Resolve(sysCorePid, 8193, "libkernel", "scePthreadDetach", 0, &scePthreadDetachAddress);
	if (result != 0 || scePthreadDetachAddress == 0)
	{
		Logger::Error("Failed to resolve scePthreadDetach for reason %llX.", result);
		return;
	}

	auto caller = Fusion::RemoteCallerManager::GetInstance(sysCorePid);

	ScePthread thr;
	caller->Call<int>(scePthreadCreateAddress, &thr, 0, DevPortThreadAddress, 0, 0);
	caller->Call<int>(scePthreadDetachAddress, &thr);
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

	// Start the devport thread that spawns the decid.elf daemon process.
	StartDevPortThread(sysCorePid);
}