#pragma once

#include <sstream>
#include <iostream>
#include <unistd.h>
#include <functional>
#include <mutex>
#include <vector>
#include <queue>
#include <atomic>
#include <string>
#include <thread>
#include <set>
#include <fcntl.h>
#include <dirent.h>
#include <optional>

// Open Orbis SDK
#include <orbis/libkernel.h>
#include <orbis/Sysmodule.h>
#include <orbis/SysCore.h>

// libFusionDaemon
#include <FusionDriver.h>
#include <RemoteSprx.h>

// Utils
#include "LncUtils.h"
#include "Misc.h"
#include "Json.h"
#include "FileSystem.h"
#include "FileUtils.h"
#include "IniParser.h"
#include "Process.h"
#include "ThreadPool.h"
#include "ProcessMonitor.h"
#include "SystemPatcher.h"
#include "Notify.h"

#include "Offsets.h"
#include "Config.h"

// Extensions
#include "PluginExtension.h"
#include "FtpExtension.h"
#include "DaemonExtension.h"
#include "DeciExtension.h"