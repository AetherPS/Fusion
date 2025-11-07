#include "stdafx.h"
#include "Embed.h"

#define DAEMON_TITLEID "FUSN00000"
#define DAEMON_ARCHIVE_PATH "/system/vsh/app/FUSN00000.7z"
#define DAEMON_PATH "/system/vsh/app"

void InstallDaemon()  
{
	FileSystem::Write(DAEMON_ARCHIVE_PATH, &_binary_FUSN00000_7z_start, (uint64_t)&_binary_FUSN00000_7z_end - (uint64_t)&_binary_FUSN00000_7z_start);
	Extract7zFile(DAEMON_ARCHIVE_PATH, DAEMON_PATH);
	FileSystem::Remove(DAEMON_ARCHIVE_PATH);

	StartRestartApp(DAEMON_TITLEID, nullptr, SCE_USER_SERVICE_USER_ID_EVERYONE);
	
	if (GetAppIdByTitleId(DAEMON_TITLEID) <= 0)
	{
		Logger::Error("[Fusion] Failed to start daemon.");
	}
}

extern "C"
{
	int __cdecl module_start(size_t argc, const void* args)
	{
		ScePthread thr;
		scePthreadCreate(&thr, 0, [](void* arg) -> void*
		{
			Logger::Init(true, Logger::LogLevelAll);

			Jailbreak(-1, false);
			
			// Mount system as R/W
			RemountReadWrite("/dev/da0x4.crypt", "/system");
			
			// Disable the annoying system updates.
			DisableUpdates();
			
			// Install and start the Fusion Daemon.
			InstallDaemon();
			
			Notify("Fusion 3 Loaded\n%s", "Uber haxor edition.");
			
			// Exit the Web Browser/Bluray Player
			// ExitGraceful();

			scePthreadExit(0);
			return 0;
		}, 0, "Init");
		scePthreadJoin(thr, nullptr);

		return 0;
	}

	int __cdecl module_stop(size_t argc, const void* args)
	{

		return 0;
	}
}
