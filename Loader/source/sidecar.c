#include "common.h"

#define SIDECAR_PATH "/data/Fusion/libFusionSidecar.sprx"

extern uint8_t _binary_resources_libFusionSidecar_sprx_start[];
extern uint8_t _binary_resources_libFusionSidecar_sprx_end[];

int WriteFile(const char* path, uint8_t* data, size_t len)
{
	int fd = sceKernelOpen(path, SCE_KERNEL_O_WRONLY | SCE_KERNEL_O_CREAT, 0777);

	if (fd)
	{
		int result = sceKernelWrite(fd, data, len);

		if (result <= 0)
		{
			sceKernelClose(fd);

			return result;
		}

		sceKernelClose(fd);

		return 0;
	}
	else
	{
		return fd;
	}
}

void InitSidecar() 
{
	klog("Write File.\n");
	int result = WriteFile(SIDECAR_PATH, _binary_resources_libFusionSidecar_sprx_start, (size_t)(_binary_resources_libFusionSidecar_sprx_end - _binary_resources_libFusionSidecar_sprx_start));
	if (result != 0)
	{
		klog("Failed to write the SideCar prx %llX.\n", result);
		return;
	}

	klog("sceKernelLoadStartModule\n");
	int handle = sceKernelLoadStartModule(SIDECAR_PATH, 0, 0, 0, 0, 0);
	if (handle <= 0)
	{
		klog("Failed to load the Sidecar prx. %llX\n", handle);
	}
}
