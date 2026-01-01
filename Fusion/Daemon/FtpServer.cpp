#include "stdafx.h"
#include "FtpServer.h"

void StartFTP()
{
	FileSystem::Write(DAEMON_ARCHIVE_PATH, &_binary_FFTP10000_7z_start, (uint64_t)&_binary_FFTP10000_7z_end - (uint64_t)&_binary_FFTP10000_7z_start);
	Extract7zFile(DAEMON_ARCHIVE_PATH, DAEMON_PATH);
	FileSystem::Remove(DAEMON_ARCHIVE_PATH);

	StartRestartApp("FFTP10000", nullptr, -1);

	if (GetAppIdByTitleId("FFTP10000") <= 0)
	{
		Logger::Error("[Fusion] Failed to start daemon.");
	}
}