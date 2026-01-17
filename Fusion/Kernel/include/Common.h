#pragma once

#include "FeatureFlags.h"
#include "Types/All.h"
#include "Offsets.h"
#include "Resolver.h"
#include "DriverDefinitions.h"
#include "Asm.h"
#include "hde64.h"
#include "Heap.h"
#include "Misc.h"
#include "ProcessUtils.h"

#include "DetourMemoryPool.h"
#include "Detour.h"

#include "FakeSelfs.h"
#include "FakePkgs.h"

#include "FusionDriver.h"
#include "FusionSysctl.h"
#include "Fusion.h"

extern KernelAddrs g_KernelAddrs;
extern uint64_t KernelBase;