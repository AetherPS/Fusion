#include "stdafx.h"
#include "LncUtils.h"

int (*sceLncUtilLaunchApp)(const char* titleId, char** args, struct LaunchAppParam* appParam);
int (*sceLncUtilGetAppId)(const char* tileId);
int (*sceLncUtilKillApp)(int appId);
int (*sceLncUtilGetAppIdOfBigApp)();
int (*sceLncUtilInitialize)();

void ResolveLncUtils()
{
	int libSceSystemServiceHandle = sceKernelLoadStartModule("libSceSystemService.sprx", 0, 0, 0, 0, 0);

	Fusion::Resolve(getpid(), libSceSystemServiceHandle, "libSceLncUtil", "sceLncUtilLaunchApp", 0, (uint64_t*)&sceLncUtilLaunchApp);
	Fusion::Resolve(getpid(), libSceSystemServiceHandle, "libSceLncUtil", "sceLncUtilGetAppId", 0, (uint64_t*)&sceLncUtilGetAppId);
	Fusion::Resolve(getpid(), libSceSystemServiceHandle, "libSceLncUtil", "sceLncUtilKillApp", 0, (uint64_t*)&sceLncUtilKillApp);
	Fusion::Resolve(getpid(), libSceSystemServiceHandle, "libSceLncUtil", "sceLncUtilGetAppIdOfBigApp", 0, (uint64_t*)&sceLncUtilGetAppIdOfBigApp);
	Fusion::Resolve(getpid(), libSceSystemServiceHandle, "libSceLncUtil", "sceLncUtilInitialize", 0, (uint64_t*)&sceLncUtilInitialize);
}

int StartRestartApp(const char* titleId, char** args, int userId)
{
	auto appId = sceLncUtilGetAppId(titleId);
	if (appId)
	{
		// App is already running kill it so we can restart it.
		sceLncUtilKillApp(appId);
	}

	// Set up the launch paramaters with the given userId as the owner/launcher.
	LaunchAppParam appParam;
	appParam.size = sizeof(LaunchAppParam);
	appParam.userId = userId;
	appParam.enableCrashReport = 0;
	appParam.checkFlag = 0;
	appParam.appAttr = 0;

	// Launch the app.
	auto res = sceLncUtilLaunchApp(titleId, args, &appParam);
	if (res <= 0)
	{
		klog("Failed to start app: %llX", res);
	}

	return res;
}