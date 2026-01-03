#include "stdafx.h"

int main(void)
{
	// Initialize and load required modules.
	if (!LoadModules())
	{
		klog("Failed to Load Modules...");
		return 0;
	}

	// Jailbreak the current process.
	JailBackup backup;
	if (Fusion::Jailbreak(getpid(), &backup) < 0)
	{
		klog("Failed to jailbreak...");
		return 0;
	}

	// Initialize Offsets.
	if (!Offsets::Init())
	{
		klog("Failed to initialize offsets...");
		return 0;
	}

	// Set a friendly name so we dont just have eboot.bin.
	sceKernelSetProcessName("Fusion Daemon");

	// Initialize the thread pool with a few threads for us to use.
	// ThreadPool::Init(5);

	// Initialize the system patcher which currently just takes over the job of patching shellcore & shellui for fpkg & debug settings.
	// SystemPatcher::Init();

	// Initialize the plugin extension system.
	// PluginExtension::Init();

	// Install the FTP Daemon.
	// InstallDaemon();

	// Load the config file.
	// Config config("/data/Fusion/Settings.cfg");
	// DaemonExtension daemons(config);

	sceKernelSleep(1);

	// if (config.Get<bool>("EnableFTP", false))
	// {
	// 	daemons.MountAndLaunchDaemon(FTP_DAEMON_TITLEID);
	// }

	// if (config.Get<bool>("StartDECI", false))
	// {
	// 	StartDECI();
	// }

	// Notify("Fusion 3 Loaded\nDownload More RAM Edition");

	klog("Fusion 3 Loaded\nDownload More RAM Edition");

	// Keep the daemon running.
	while (true) { sceKernelSleep(1); }

	return 0;
}