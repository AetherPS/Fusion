#include "stdafx.h"
#include <iostream>

// TODO: Add Daemon helper class that can Start and/or Install Daemons. We will want to add a list of Daemons to launch at startup to the config.

extern uint8_t _binary_FFTP10000_7z_start[];
extern uint8_t _binary_FFTP10000_7z_end[];

#define DAEMON_TITLEID "FFTP10000"
#define DAEMON_ARCHIVE_PATH "/system/vsh/app/FFTP10000.7z"
#define DAEMON_PATH "/system/vsh/app"

void StartFTP()
{
	// TODO: Add install action... likely the above.

	FileSystem::Write(DAEMON_ARCHIVE_PATH, &_binary_FFTP10000_7z_start, (uint64_t)&_binary_FFTP10000_7z_end - (uint64_t)&_binary_FFTP10000_7z_start);
	Extract7zFile(DAEMON_ARCHIVE_PATH, DAEMON_PATH);
	FileSystem::Remove(DAEMON_ARCHIVE_PATH);

	StartRestartApp("FFTP10000", nullptr, -1);

	if (GetAppIdByTitleId("FFTP10000") <= 0)
	{
		Logger::Error("[Fusion] Failed to start daemon.");
	}
}

int main(int argc, char** arg)
{
	// Set up the Logger.
	Logger::Init(true, Logger::LoggingLevels::LogLevelAll);
	Logger::Info("Fusion Daemon started.");

	// Jailbreak our current process.
	if (!Jailbreak())
	{
		Notify("Failed to jailbreak Process...");
		ExitGraceful();
		return 0;
	}

	// Initialize and load required modules.
	if (!LoadModules())
	{
		Notify("Failed to Load Modules...");
		ExitGraceful();
		return 0;
	}

	if (!Offsets::Init())
	{
		Notify("Failed to initialize offsets...");
		ExitGraceful();
		return 0;
	}

	ThreadPool::Init(5);
	SystemPatcher::Init();
	
	Config config("/data/Fusion/Settings.cfg");

	if (config.Get<bool>("EnableFTP", false))
	{
		StartFTP();
	}

	if (config.Get<bool>("StartDECI", false))
	{
		StartDECI();
	}

	Notify("Fusion 3 Loaded\nDownload More RAM Edition");

	while (true) { sceKernelSleep(1); }

	ExitGraceful();
	return 0;
}