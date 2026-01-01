#include "common.h"
#include "daemon.h"
#include "fusiondriver.h"

// libSceLncUtil
int (*sceLncUtilLaunchApp)(const char* titleId, char** args, struct LaunchAppParam* appParam);
int (*sceLncUtilGetAppId)(const char* tileId);
int (*sceLncUtilKillApp)(int appId);

void ResolveLnc()
{
	int libSceSystemServiceHandle = 0;
	sys_dynlib_load_prx("libSceSystemService.sprx", &libSceSystemServiceHandle);

	Resolve(getpid(), libSceSystemServiceHandle, "libSceLncUtil", "sceLncUtilLaunchApp", 0, (uint64_t*)&sceLncUtilLaunchApp);
	Resolve(getpid(), libSceSystemServiceHandle, "libSceLncUtil", "sceLncUtilGetAppId", 0, (uint64_t*)&sceLncUtilGetAppId);
	Resolve(getpid(), libSceSystemServiceHandle, "libSceLncUtil", "sceLncUtilKillApp", 0, (uint64_t*)&sceLncUtilKillApp);
}

void InstallDaemon() 
{
	klog("Initialize Daemon...\n");

#ifdef HAS_DAEMON
	// Mount system as R/W
	RemountReadWrite("/dev/da0x4.crypt", "/system");

	// Resolve libSceLncUtil functions.
	ResolveLnc();

	// Create necessary directories.
	CreateDirectory(DAEMON_PATH);
	CreateDirectory(DAEMON_SYS_PATH);

	// Decompress and write the param.sfo if it does not exist.
	if (!FileExist(DAEMON_PARAM_PATH))
	{
		int result = DecompressToFile(_binary_resources_param_sfo_compressed_start, DAEMON_PARAM_PATH);
		if (result != 0)
		{
			klog("Failed to write the param.sfo %llX.\n", result);
			return;
		}
	}

	// Remove the existing eboot.bin if it exists.
	if (FileExist(DAEMON_EBOOT_PATH))
	{
		RemoveFile(DAEMON_EBOOT_PATH);
	}

	// Decompress and write the eboot.bin.
	int result = DecompressToFile(_binary_resources_eboot_bin_compressed_start, DAEMON_EBOOT_PATH);
	if (result != 0)
	{
		klog("Failed to write the eboot %llX.\n", result);
		return;
	}

	// App is already running kill it so we can restart it.
	int appId = sceLncUtilGetAppId(DAEMON_TITLEID);
	if (appId)
	{
		sceLncUtilKillApp(appId);
	}

	// Setup launch parameters.
	struct LaunchAppParam appParam;
	appParam.size = sizeof(struct LaunchAppParam);
	appParam.userId = -1; // All users.
	appParam.enableCrashReport = 0;
	appParam.checkFlag = 0;
	appParam.appAttr = 0;

	// Launch the Daemon.
	int newAppId = sceLncUtilLaunchApp(DAEMON_TITLEID, 0, &appParam);

	if (newAppId <= 0)
	{
		klog("Failed to launch Daemon: %X\n", newAppId);
	}

#else
	klog("Skipping... Daemon not present, some features disabled!\n");
#endif
}

