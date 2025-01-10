#pragma once

#include <stdint.h>
#include <stdarg.h>

#include "FeatureFlags.h"
#include "Settings.h"
#include "Offsets.h"

#include "Sparse.h"
#include "Types.h"
#include "Asm.h"
#include "Patches.h"
#include "Resolver.h"
#include "hde64.h"
#include "Heap.h"
#include "Misc.h"
 
#include "BaseDetour.h"
#include "Detour64.h"
#include "Detour32.h"
#include "DetourCall.h"

#include "ProcessUtils.h"
#include "SystemCalls.h"

#include "FakePkgTypes.h"
#include "FakePkgs.h"
#include "FakeSelfTypes.h"
#include "FakeSelfs.h"

#include "DriverDefinitions.h"
#include "FusionDriver.h"

extern uint8_t* KernelBase;