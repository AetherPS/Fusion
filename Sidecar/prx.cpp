#include "stdafx.h"
#include <FusionDriver.h>

#define ORBIS_TITLEID "ORBS30000"
#define FTP_TITLEID "OFTP10000"
#define FTP_ARCHIVE_PATH "/system/vsh/app/OFTP10000.7z"
#define DAEMON_PATH "/system/vsh/app"

void InstallFtpDaemon()
{
	//WriteFile(FTP_ARCHIVE_PATH, ftp_archive, ftp_archive_Size);
	//Extract7zFile(FTP_ARCHIVE_PATH, DAEMON_PATH);

	StartRestartApp(FTP_TITLEID, nullptr, SCE_USER_SERVICE_USER_ID_EVERYONE);

	if (GetAppIdByTitleId(FTP_TITLEID) <= 0)
	{
		Logger::Error("[Fusion] Failed to start FTP Daemon.");
	}
}

void StartOrbisSuite()
{
	StartRestartApp(ORBIS_TITLEID, nullptr, SCE_USER_SERVICE_USER_ID_EVERYONE);

	if (GetAppIdByTitleId(ORBIS_TITLEID) <= 0)
	{
		Logger::Error("[Fusion] Failed to start Orbis Suite Daemon.");
	}
}

void LoadPlugins()
{
	
	// TODO:
}

extern "C"
{
	int __cdecl module_start(size_t argc, const void* args)
	{
		Logger::Init(true, Logger::LogLevelAll);
		
		Logger::Info("Hello World.");

		JailBackup bk;
		Fusion::Jailbreak(getpid(), &bk);

		//Settings::Init();

		// Mount system as R/W
		mount_large_fs("/dev/da0x4.crypt", "/system", "exfatfs", "511", MNT_UPDATE);

		// Disable the anoying system updates.
		DisableUpdates();

		// Installs and loads the FTP.
		//if (Settings::EnableFTP)
			InstallFtpDaemon();

		StartOrbisSuite();

		// Load launch plugin PRX's
		// LoadPlugins();

		// Remove the temp prx.
		sceKernelUnlink("/data/Fusion/libFusionSidecar.sprx");
		
		// Exit the Web Browser/Bluray Player
		// ExitGraceful();
		return 0;
	}

	int __cdecl module_stop(size_t argc, const void* args)
	{

		return 0;
	}
}
