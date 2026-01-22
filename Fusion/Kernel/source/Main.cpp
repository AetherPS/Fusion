#include "Common.h"
#include "Fusion.h"

extern "C"
{
    int module_init(void)
    {
        printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
        printf("Kernel Base: %p\n", (void*)g_KernelBase);
        printf("Firmware: %s\n", g_DetectedFirmware);
        printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");

        InitFusion();

        return 0;
    }

    void module_fini(void)
    {
        
    }
}