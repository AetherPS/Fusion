#include "Common.h"
#include "FeatureFlags.h"

void PrintFeatureFlags()
{
#ifdef DEBUG
	printf("\n!! DEBUG BUILD !!\n\n");
#endif
	printf("\nFeatures: \n");
#ifdef FF_TrapHooks
	printf("   Fatal Trap Hooks\n");
#endif
#ifdef FF_HomeBrew
	printf("   Homebrew\n");
#endif
#ifdef FF_Driver
	printf("   Kernel Driver\n");
#endif
#ifdef FF_Fuse
	printf("   Fuse\n");
#endif
}