#pragma once

#include <kernel.h>
#include <string>
#include <queue>
#include <vector>
#include <mutex>
#include <variant>
#include <SysmoduleInternal.h>
#include <libsysmodule.h>

// StubMaker
#include <LncUtil.h>
#include <KernelExt.h>

// libUtils
#include <StringExt.h>
#include <Logging.h>
#include <Logger.h>
#include <FileUtils.h>
#include <FileSystem.h>
#include <Notify.h>
#include <Json.h>
#include <ThreadPool.h>
#include <Process.h>
#include <SignalDefs.h>

// lib7z
#include <7zExtractor.h>

// libAppCtrl
#include <AppControl.h>

// libSysInt
#include <SystemInterface.h>

// libFusionDriver
#include <DriverDefinitions.h>
#include <FusionDriver.h>

#include "Misc.h"
#include "Config.h"
#include "Offsets.h"
#include "ProcessMonitor.h"
#include "SystemPatcher.h"
