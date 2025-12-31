#include "Common.h"
#include "FeatureFlags.h"

void PrintFeatureFlags()
{
#ifdef DEBUG
	kprintf("\n!! DEBUG BUILD !!\n\n");
#endif

	printf("Features: \n");
#ifdef FF_BETA
	printf("   Beta Features Active!\n");
#endif
#ifdef FF_HomeBrew
	printf("   Homebrew\n");
#endif
#ifdef FF_Driver
	printf("   Kernel Driver\n");
#endif
#ifdef FF_DirectMemory
	printf("   Direct Memory\n");
#endif
#ifdef FF_LibraryReplacer
	printf("   Library Replacer\n");
#endif
#ifdef FF_Fuse
	printf("   Fuse\n");
#endif
#ifdef FF_TrapHooks
	kprintf("   Trap Hooks\n");
#endif
#ifdef FF_DevAct
	printf("   Dev Act Spoofer\n");
#endif
#ifdef FF_Dipsw
	printf("   Dip Switch Spoofer\n");
#endif
#ifdef FF_TTYRedirect
	kprintf("   TTY Redirector\n");
#endif
}