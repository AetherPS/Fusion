#pragma once

/*
	Define Feature flags that can be toggled depending on what functionality you may require for the given build.

	NOTE: Dont forget to update the *PrintFeatureFlags()* to print any new feature flags you may add!
*/

#define FF_BETA
#define FF_HomeBrew
#define FF_Driver
#define FF_DirectMemory
#define FF_LibraryReplacer
#define FF_Fuse
// #define FF_TrapHooks
#define FF_DevAct
#define FF_Dipsw
// #define FF_TTYRedirect

void PrintFeatureFlags();