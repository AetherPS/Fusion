#include "common.h"

#define SIDECAR_PATH "/data/Fusion/libFusionSidecar.sprx"

extern uint8_t _binary_resources_libFusionSidecar_sprx_start[];
extern uint8_t _binary_resources_libFusionSidecar_sprx_end[];

void InitSidecar() 
{
	klog("Initialize Sidecar\n");

#ifdef HAS_SIDECAR

	// Remove existing Sidecar prx if exists.
	if (FileExist(SIDECAR_PATH))
	{
		RemoveFile(SIDECAR_PATH);
	}

	int result = WriteFile(SIDECAR_PATH, _binary_resources_libFusionSidecar_sprx_start, (size_t)(_binary_resources_libFusionSidecar_sprx_end - _binary_resources_libFusionSidecar_sprx_start));
	if (result != 0)
	{
		klog("Failed to write the SideCar prx %llX.\n", result);
		return;
	}

	int handle = sceKernelLoadStartModule(SIDECAR_PATH, 0, 0, 0, 0, 0);
	if (handle <= 0)
	{
		klog("Failed to load the Sidecar prx. %llX\n", handle);
	}

	RemoveFile(SIDECAR_PATH);

#else
	klog("Skipping... Sidecar not present, some features disabled!\n");
#endif
}

