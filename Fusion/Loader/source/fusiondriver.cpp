#include "Common.h"
#include "fusiondriver.h"
#include <sys/errno.h>
#include <sys/ioccom.h>

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
		klog("Driver request 0x%llx failed with %x\n", request, res);
		sceKernelClose(fd);
		return res;
	}

	sceKernelClose(fd);
	return 0;
}

int Jailbreak(int processId, struct JailBackup* backup, uint64_t authId, bool nullSandboxPath)
{
	Input_Jailbreak input;
	input.ProcessId = processId;
	input.Jail = backup;
	input.AuthId = authId;
	input.NullRandPath = nullSandboxPath;

	return MakeDriverRequest(PROC_JAILBREAK, &input);
}

int RestoreJail(int processId, struct JailBackup backup)
{
	Input_RestoreJail input;
	input.ProcessId = processId;
	input.Jail = backup;

	return MakeDriverRequest(PROC_JAIL, &input);
}

int ReadWriteMemory(int processId, uint64_t addr, void* data, size_t len, bool write)
{
	Input_ReadWriteMemory input;
	input.ProcessId = processId;
	input.ProcessAddress = addr;
	input.DataAddress = data;
	input.Length = len;
	input.IsWrite = write;

	return MakeDriverRequest(PROC_READ_WRITE_MEMORY, &input);
}

int AllocateMemory(int processId, uint64_t* outAddress, size_t length, int protection)
{
	Input_AllocMemory input;
	input.ProcessId = processId;
	input.OutAddress = outAddress;
	input.Length = length;
	input.Protection = protection;

	return MakeDriverRequest(PROC_ALLOCATE_MEMORY, &input);
}

int FreeMemory(int processId, uint64_t processAddress, size_t length)
{
	Input_FreeMemory input;
	input.ProcessId = processId;
	input.ProcessAddress = processAddress;
	input.Length = length;

	return MakeDriverRequest(PROC_FREE_MEMORY, &input);
}

int StartThread(int processId, uint64_t threadEntry, uint64_t stackMemory, size_t stackSize)
{
	Input_StartThreadInfo input;
	input.ProcessId = processId;
	input.ThreadEntry = threadEntry;
	input.StackMemory = stackMemory;
	input.StackSize = stackSize;

	return MakeDriverRequest(PROC_START_THREAD, &input);
}

int Resolve(int processId, int libHandle, const char* library, const char* symbol, unsigned int flags, uint64_t* addr)
{
	Input_ResolveInfo input;
	input.ProcessId = processId;
	input.Handle = libHandle;
	input.Flags = flags;

	if (library != nullptr)
		strcpy(input.Library, library);

	if (symbol != nullptr)
		strcpy(input.Symbol, symbol);

	int res = MakeDriverRequest(PROC_RESOLVE, &input);

	*addr = input.Address;

	return res;
}
