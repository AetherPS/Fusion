#pragma once

#define DAEMON_TITLEID "FUSN00000"
#define DAEMON_PARAM_PATH "/system/vsh/app/FUSN00000/sce_sys/param.sfo"
#define DAEMON_SYS_PATH "/system/vsh/app/FUSN00000/sce_sys"
#define DAEMON_EBOOT_PATH "/system/vsh/app/FUSN00000/eboot.bin"
#define DAEMON_PATH "/system/vsh/app/FUSN00000"

extern uint8_t _binary_resources_eboot_bin_compressed_start[];
extern uint8_t _binary_resources_eboot_bin_compressed_end[];
extern uint8_t _binary_resources_param_sfo_compressed_start[];
extern uint8_t _binary_resources_param_sfo_compressed_end[];

struct LaunchAppParam
{
	unsigned int size;      //0x00
	int userId;             //0x04
	int appAttr;            //0x08
	int enableCrashReport;  //0x0C
	uint64_t checkFlag;     //0x10
};

void InstallDaemon();