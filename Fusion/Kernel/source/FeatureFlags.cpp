#include "Common.h"
#include "FeatureFlags.h"

void PrintFeatureFlags()
{
#ifdef DEBUG
	kprintf("\n!! DEBUG BUILD !!\n\n");
#endif
	printf("\nFeatures: \n");
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