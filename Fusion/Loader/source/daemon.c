#include "common.h"
#include "compressedblob.h"

#define DAEMON_PARAM_PATH "/system/vsh/app/FUSN00000/sce_sys/param.sfo"
#define DAEMON_SYS_PATH "/system/vsh/app/FUSN00000/sce_sys"
#define DAEMON_EBOOT_PATH "/system/vsh/app/FUSN00000/eboot.bin"
#define DAEMON_PATH "/system/vsh/app/FUSN00000"

extern uint8_t _binary_resources_eboot_bin_compressed_start[];
extern uint8_t _binary_resources_eboot_bin_compressed_end[];
extern uint8_t _binary_resources_param_sfo_compressed_start[];
extern uint8_t _binary_resources_param_sfo_compressed_end[];

void InstallDaemon() 
{
	klog("Initialize Daemon...\n");

//#ifdef HAS_DAEMON

	CreateDirectory(DAEMON_PATH);
	CreateDirectory(DAEMON_SYS_PATH);

	// Decompress and write the param.sfo. if it does not exist.
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



	klog("Done.\n");

//#else
//	klog("Skipping... Daemon not present, some features disabled!\n");
//#endif
}

