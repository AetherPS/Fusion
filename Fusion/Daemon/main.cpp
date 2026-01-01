#include "stdafx.h"

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