#include "stdafx.h"
#include <FusionDriver.h>

#include "Embed.h"


#define ORBIS_TITLEID "ORBS30000"
#define FTP_TITLEID "OFTP10000"
#define FTP_ARCHIVE_PATH "/system/vsh/app/OFTP10000.7z"
#define DAEMON_PATH "/system/vsh/app"

// TODO: Add Daemon helper class that can Start and/or Install Daemons. We will want to add a list of Daemons to launch at startup to the config.

void InstallFtpDaemon()
{
	FileSystem::Write(FTP_ARCHIVE_PATH, ftp_archive, ftp_archive_Size);
	Extract7zFile(FTP_ARCHIVE_PATH, DAEMON_PATH);

	StartRestartApp(FTP_TITLEID, nullptr, SCE_USER_SERVICE_USER_ID_EVERYONE);
	
	if (GetAppIdByTitleId(FTP_TITLEID) <= 0)
	{
		Logger::Error("[Fusion] Failed to start FTP Daemon.");
	}
}

extern "C"
{
	int __cdecl module_start(size_t argc, const void* args)
	{
		Logger::Init(true, Logger::LogLevelAll);
		Logger::Info("Hello World.");

		//Logger::Info("Jailbreak.");
		//Jailbreak(-1, false);

		// Logger::Info("Mount system as R/W");
		// // Mount system as R/W
		// RemountReadWrite("/dev/da0x4.crypt", "/system");
		// 
		// Logger::Info("FileSystem::Write");
		// FileSystem::Write("/data/OFTP10000.7z", ftp_archive, ftp_archive_Size);
		// 
		// Logger::Info("Extract7zFile");
		// Extract7zFile("/data/OFTP10000.7z", "/data/app");

		// // Disable the anoying system updates.
		// DisableUpdates();
		// 
		// // Installs and loads the FTP.
		// InstallFtpDaemon();
		// 
		// // Remove the temp prx.
		// sceKernelUnlink("/data/Fusion/libFusionSidecar.sprx");
		// 
		// Notify("Fusion 3 Loaded\n%s", "Uber haxor edition.");
		// 
		// // Exit the Web Browser/Bluray Player
		// // ExitGraceful();

		return 0;
	}

	int __cdecl module_stop(size_t argc, const void* args)
	{

		return 0;
	}
}
