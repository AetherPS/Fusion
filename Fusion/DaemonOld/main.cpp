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

	// Set a friendly name so we dont just have eboot.bin.
	sceKernelSetProcessName("Fusion Daemon");

	// Initialize Offsets.
	if (!Offsets::Init())
	{
		Notify("Failed to initialize offsets...");
		ExitGraceful();
		return 0;
	}

	// Initialize the thread pool with a few threads for us to use.
	ThreadPool::Init(5);

	// Initialize the system patcher which currently just takes over the job of patching shellcore & shellui for fpkg & debug settings.
	SystemPatcher::Init();

	// Initialize the plugin extension system.
	PluginExtension::Init();

	// Install the FTP Daemon.
	InstallDaemon();

	// Load the config file.
	Config config("/data/Fusion/Settings.cfg");
	DaemonExtension daemons(config);

	sceKernelSleep(1);

	if (config.Get<bool>("EnableFTP", false))
	{
		daemons.MountAndLaunchDaemon(FTP_DAEMON_TITLEID);
	}

	if (config.Get<bool>("StartDECI", false))
	{
		StartDECI();
	}

	Notify("Fusion 3 Loaded\nDownload More RAM Edition");

	// Keep the daemon running.
	while (true) { sceKernelSleep(1); }

	ExitGraceful();
	return 0;
}