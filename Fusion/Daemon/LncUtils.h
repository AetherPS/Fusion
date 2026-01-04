#pragma once

struct LaunchAppParam
{
	unsigned int size;      //0x00
	int userId;             //0x04
	int appAttr;            //0x08
	int enableCrashReport;  //0x0C
	uint64_t checkFlag;     //0x10
};

extern int (*sceLncUtilLaunchApp)(const char* titleId, char** args, struct LaunchAppParam* appParam);
extern int (*sceLncUtilGetAppId)(const char* tileId);
extern int (*sceLncUtilKillApp)(int appId);
extern int (*sceLncUtilGetAppIdOfBigApp)();
extern int (*sceLncUtilInitialize)();

void ResolveLncUtils();
int StartRestartApp(const char* titleId, char** args, int userId);