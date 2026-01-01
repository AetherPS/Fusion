#pragma once

extern uint8_t _binary_FFTP10000_7z_start[];
extern uint8_t _binary_FFTP10000_7z_end[];

#define DAEMON_TITLEID "FFTP10000"
#define DAEMON_ARCHIVE_PATH "/system/vsh/app/FFTP10000.7z"
#define DAEMON_PATH "/system/vsh/app"

void StartFTP();