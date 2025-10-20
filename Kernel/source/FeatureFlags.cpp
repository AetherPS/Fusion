#include "Common.h"
#include "FeatureFlags.h"

void PrintFeatureFlags()
{
#ifdef DEBUG
	kprintf("\n!! DEBUG BUILD !!\n\n");
#endif

	kprintf("Features: \n");
#ifdef FF_BETA
	kprintf("   Beta Features Active!\n");
#endif
#ifdef FF_HomeBrew
	kprintf("   Homebrew\n");
#endif
#ifdef FF_Driver
	kprintf("   Kernel Driver\n");
#endif
#ifdef FF_DirectMemory
	kprintf("   Direct Memory\n");
#endif
#ifdef FF_LibraryReplacer
	kprintf("   Library Replacer\n");
#endif
#ifdef FF_Fuse
	kprintf("   Fuse\n");
#endif
#ifdef FF_TrapHooks
	kprintf("   Trap Hooks\n");
#endif
#ifdef FF_DevAct
	kprintf("   Dev Act Spoofer\n");
#endif
#ifdef FF_Dipsw
	kprintf("   Dip Switch Spoofer\n");
#endif
#ifdef FF_TTYRedirect
	kprintf("   TTY Redirector\n");
#endif
}