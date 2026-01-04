#include "stdafx.h"
#include "FtpExtension.h"

void InstallDaemon() // TODO: Convert to LZ4 for simplicity?
{
	sceKernelMkdir("/data/Fusion/Daemons", 0777);
	sceKernelMkdir("/data/Fusion/Daemons/FFTP10000", 0777);
	sceKernelMkdir("/data/Fusion/Daemons/FFTP10000/sce_sys", 0777);

	// TODO: Extract param.sfo & eboot.bin

	// FileSystem::Write(FTP_DAEMON_ARCHIVE_PATH, &_binary_FFTP10000_7z_start, (uint64_t)&_binary_FFTP10000_7z_end - (uint64_t)&_binary_FFTP10000_7z_start);
	// Extract7zFile(FTP_DAEMON_ARCHIVE_PATH, FTP_DAEMON_PATH);
	// FileSystem::Remove(FTP_DAEMON_ARCHIVE_PATH);
}