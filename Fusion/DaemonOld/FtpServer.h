#pragma once

extern uint8_t _binary_FFTP10000_7z_start[];
extern uint8_t _binary_FFTP10000_7z_end[];

#define FTP_DAEMON_TITLEID "FFTP10000"
#define FTP_DAEMON_ARCHIVE_PATH "/data/Fusion/Daemons/FFTP10000.7z"
#define FTP_DAEMON_PATH "/data/Fusion/Daemons"

void InstallDaemon();