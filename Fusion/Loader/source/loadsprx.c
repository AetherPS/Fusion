#include "common.h"
#include "loadsprx.h"

int StartPThread(int processId, uint64_t entryPoint)
{
	struct ThreadShellCodeHeader* shellCodeHeader = (struct ThreadShellCodeHeader*)&_binary_resources_ThreadShellCode_bin_start;
	shellCodeHeader->ShellCodeComplete = 0;
	shellCodeHeader->ThreadEntry = entryPoint;

	int res = Resolve(processId, 8193, "libkernel", "scePthreadCreate", 0, &shellCodeHeader->scePthreadCreate);
	if (res != 0)
	{
		klog("%s: Failed to resolve scePthreadCreate.\n", __FUNCTION__);
		return -1;
	}

	res = Resolve(processId, 8193, "libkernel", "scePthreadJoin", 0, &shellCodeHeader->scePthreadJoin);
	if (res != 0)
	{
		klog("%s: Failed to resolve scePthreadJoin.\n", __FUNCTION__);
		return -1;
	}

	res = Resolve(processId, 8193, "libkernel", "scePthreadExit", 0, &shellCodeHeader->scePthreadExit);
	if (res != 0)
	{
		klog("%s: Failed to resolve scePthreadExit.\n", __FUNCTION__);
		return -1;
	}

	uint64_t environ = 0;
	res = Resolve(processId, 8193, 0, "environ", 0, &environ);
	if (res != 0)
	{
		klog("%s: Failed to resolve 'environ'... [%llX] Can not continue.\n", __FUNCTION__, res);
		return -1;
	}

	// thr_initial is always +0x10 from the environ symbol.
	shellCodeHeader->thr_initial = environ + 0x10;

	uint64_t shellcodeSize = (uint64_t)&_binary_resources_ThreadShellCode_bin_end - (uint64_t)&_binary_resources_ThreadShellCode_bin_start;
	uint64_t totalAllocatedSize = shellcodeSize + STACK_SIZE;
	uint64_t shellCodeMemory;

	res = AllocateMemory(processId, &shellCodeMemory, totalAllocatedSize, VM_PROT_ALL);
	if (res != 0 || shellCodeMemory <= 0)
	{
		klog("%s: Failed to allocate memory on the process. %llX\n", __FUNCTION__, res);
		return -1;
	}

	if (ReadWriteMemory(processId, shellCodeMemory, shellCodeHeader, shellcodeSize, true) != 0)
	{
		klog("%s: Write shellcode failed.\n", __FUNCTION__);
		FreeMemory(processId, shellCodeMemory, totalAllocatedSize);
		return false;
	}

	// Create a thread to run our shellcode.
	StartThread(processId, shellCodeMemory + shellCodeHeader->entry, shellCodeMemory + shellcodeSize, STACK_SIZE);

	// Wait for the shellcode to complete by reading the byte that will be set to 1 on completion.
	bool shellCodeComplete = 0;
	int timeout = 20; // 2 seconds
	while (!shellCodeComplete && timeout > 0)
	{
		sceKernelUsleep(1000 * 500);

		if (ReadWriteMemory(processId, shellCodeMemory + offsetof(struct ThreadShellCodeHeader, ShellCodeComplete), (void*)&shellCodeComplete, sizeof(shellCodeComplete), false) != 0)
		{
			klog("%s: Failed to read shellCodeComplete.\n", __FUNCTION__);
			FreeMemory(processId, shellCodeMemory, totalAllocatedSize);
			return -1;
		}

		timeout--;
	}

	if (timeout == 0)
	{
		klog("%s: Timeout waiting for shellcode completion\n", __FUNCTION__);
	}
	else
	{
		sceKernelUsleep(1000 * 200);
	}

	FreeMemory(processId, shellCodeMemory, totalAllocatedSize);

	return 0;
}

int LoadSprx(char* processName, const char* path)
{
	int processId = GetPidByName(processName);
	if (processId == -1)
	{
		klog("%s: Failed to find process '%s'... Can not continue.\n", __FUNCTION__, processName);
		return -1;
	}

	struct SprxLoaderHeader* shellCodeHeader = (struct SprxLoaderHeader*)&_binary_resources_LoaderShellCode_bin_start;
	shellCodeHeader->ModuleHandle = -1;
	strcpy(shellCodeHeader->Path, (char*)path);

	int res = Resolve(processId, 8193, 0, "sceKernelLoadStartModule", 0, &shellCodeHeader->sceKernelLoadStartModule);
	if (res != 0)
	{
		klog("%s: Failed to resolve sceKernelLoadStartModule.\n", __FUNCTION__);
		return -1;
	}

	res = Resolve(processId, 8193, 0, "scePthreadExit", 0, &shellCodeHeader->scePthreadExit);
	if (res != 0)
	{
		klog("%s: Failed to resolve scePthreadExit.\n", __FUNCTION__);
		return -1;
	}

	uint64_t shellcodeSize = (uint64_t)&_binary_resources_LoaderShellCode_bin_end - (uint64_t)&_binary_resources_LoaderShellCode_bin_start;
	uint64_t shellCodeMemory;

	res = AllocateMemory(processId, &shellCodeMemory, shellcodeSize, VM_PROT_ALL);
	if (res != 0 || shellCodeMemory <= 0)
	{
		klog("%s: Failed to allocate memory on the process. %llX\n", __FUNCTION__, res);
		return -1;
	}

	if (ReadWriteMemory(processId, shellCodeMemory, shellCodeHeader, shellcodeSize, true) != 0)
	{
		klog("%s: Write shellcode failed.\n", __FUNCTION__);
		FreeMemory(processId, shellCodeMemory, shellcodeSize);
		return false;
	}

	if (StartPThread(processId, (uint64_t)(shellCodeMemory + shellCodeHeader->entry)) != 0)
	{
		klog("%s: shellcode thread failed.\n", __FUNCTION__);
		FreeMemory(processId, shellCodeMemory, shellcodeSize);
		return false;
	}

	// Read out the module handle to see if the loading was a success or not.
	int32_t moduleHandle = -1;
	if (ReadWriteMemory(processId, shellCodeMemory + offsetof(struct SprxLoaderHeader, ModuleHandle), (void*)&moduleHandle, sizeof(moduleHandle), false) != 0)
	{
		klog("%s: Failed to read ModuleHandle.\n", __FUNCTION__);
		//FreeMemory(processId, shellCodeMemory, shellcodeSize);
		return -1;
	}

	// Free up the memory we dont need it anymore.
	//FreeMemory(processId, shellCodeMemory, shellcodeSize);

	return moduleHandle;
}