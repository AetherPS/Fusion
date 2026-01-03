#include "stdafx.h"
#include "FtpServer.h"

void InstallDaemon()
{
	FileSystem::Write(FTP_DAEMON_ARCHIVE_PATH, &_binary_FFTP10000_7z_start, (uint64_t)&_binary_FFTP10000_7z_end - (uint64_t)&_binary_FFTP10000_7z_start);
	Extract7zFile(FTP_DAEMON_ARCHIVE_PATH, FTP_DAEMON_PATH);
	FileSystem::Remove(FTP_DAEMON_ARCHIVE_PATH);
}