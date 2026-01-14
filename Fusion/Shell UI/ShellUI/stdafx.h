#pragma once

#include <kernel.h>
#include <string>
#include <queue>
#include <vector>
#include <mutex>
#include <variant>
#include <map>

// StubMaker
#include <KernelExt.h>
#include <monosgen.h>

// libUtils
#include <StringExt.h>
#include <Logging.h>
#include <Logger.h>
#include <Patcher.h>

// libDetour
#include <Detour.h>
#include <DetourManager.h>

extern DetourManager* Manager;
