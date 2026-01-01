#include "Common.h"
#include "fusiondriver.h"
#include "Types/errno.h"

int MakeDriverRequest(unsigned long request, void* input)
{
	int fd = sceKernelOpen(DRIVER_PATH, 0, 0);
	if (fd < 0)
	{
		klog("Failed to open the driver device.\n");
		return -ENOENT;
	}

	int res = ioctl(fd, request, input);
	if (res != 0)
	{
		klog("Driver request 0x%lX failed with %d\n", request, res);
		sceKernelClose(fd);
		return res;
	}

	sceKernelClose(fd);
	return 0;
}

int Resolve(int processId, int libHandle, const char* library, const char* symbol, unsigned int flags, uint64_t* addr)
{
	struct Input_ResolveInfo input;
	input.ProcessId = processId;
	input.Handle = libHandle;
	input.Flags = flags;

	if (library != 0)
		strcpy(input.Library, library);

	if (symbol != 0)
		strcpy(input.Symbol, symbol);

	int res = MakeDriverRequest(PROC_RESOLVE, &input);

	*addr = input.Address;

	return res;
}