#include "Common.h"
#include "Bootstrapper.h"

extern uint8_t _binary_Bootstrapper_start[];
extern uint8_t _binary_Bootstrapper_end[];

extern uint8_t _binary_UIBootstrapper_start[];
extern uint8_t _binary_UIBootstrapper_end[];

void* LoadBootstrapper(thread* td, void* original)
{
	uint64_t shellCodeMemory = 0;
	int res = AllocateMemory(td->td_proc, "Fusion Bootstrapper", (uint64_t)&_binary_Bootstrapper_end - (uint64_t)&_binary_Bootstrapper_start, VM_PROT_ALL, &shellCodeMemory);

	if (res != 0 || shellCodeMemory <= 0)
	{
		printf("Bootstrapper failed to allocate shellcode memory for reason %llX.\n", res);
		return original;
	}

	auto shellCodeHeader = (BootStrapperShellCodeHeader*)_binary_Bootstrapper_start;
	dynlib_dlsym(td->td_proc, 8193, "sceKernelLoadStartModule", nullptr, 0, (void**)&shellCodeHeader->sceKernelLoadStartModule);
	dynlib_dlsym(td->td_proc, 8193, "sceKernelStopUnloadModule", nullptr, 0, (void**)&shellCodeHeader->sceKernelStopUnloadModule);
	shellCodeHeader->sceSysmodulePreloadModuleForLibkernel = (uint64_t)original;

	if (ReadWriteProcessMemory(td, td->td_proc, (void*)shellCodeMemory, _binary_Bootstrapper_start, (uint64_t)&_binary_Bootstrapper_end - (uint64_t)&_binary_Bootstrapper_start, true) != 0)
	{
		printf("Bootstrapper failed to write shellcode.\n");
		return original;
	}

	return (void*)(shellCodeMemory + shellCodeHeader->entry);
}

void* LoadUIBootStrapper(thread* td, void* original)
{
	uint64_t shellCodeMemory = 0;
	int res = AllocateMemory(td->td_proc, "Fusion UIBootstrapper", (uint64_t)&_binary_UIBootstrapper_end - (uint64_t)&_binary_UIBootstrapper_start, VM_PROT_ALL, &shellCodeMemory);

	if (res != 0 || shellCodeMemory <= 0)
	{
		printf("UIBootstrapper failed to allocate shellcode memory for reason %llX.\n", res);
		return original;
	}

	auto shellCodeHeader = (UIBootStrapperShellCodeHeader*)_binary_UIBootstrapper_start;
	dynlib_dlsym(td->td_proc, 8193, "sceKernelLoadStartModule", nullptr, 0, (void**)&shellCodeHeader->sceKernelLoadStartModule);
	shellCodeHeader->Original = (uint64_t)original;

	if (ReadWriteProcessMemory(td, td->td_proc, (void*)shellCodeMemory, _binary_UIBootstrapper_start, (uint64_t)&_binary_UIBootstrapper_end - (uint64_t)&_binary_UIBootstrapper_start, true) != 0)
	{
		printf("UIBootstrapper failed to write shellcode.\n");
		return original;
	}

	return (void*)(shellCodeMemory + shellCodeHeader->entry);
}

Detour* Bootstrapper::do_dlsymDetour;
void* Bootstrapper::do_dlsymHook(struct dynlib* dl, struct dynlib_obj* obj, char* name, char* libName, unsigned int flags)
{
	auto td = CurrentThread();
	void* result = do_dlsymDetour->Invoke<void*>(dl, obj, name, libName, flags);

	// If the symbol is sceSysmodulePreloadModuleForLibkernel we hijack that to run our boot strapper.
	if (strstr(name, "sceSysmodulePreloadModuleForLibkernel"))
	{
		return LoadBootstrapper(td, result);
	}

	// if the symbol is scePsmUtilFinalize we hijack that to load the UI boot strapper.
	if (strstr(name, "sceVnaNotifyStatus") && strstr(td->td_proc->p_comm, "SceShellUI"))
	{
		return LoadUIBootStrapper(td, result);
	}

	return result;
}

void Bootstrapper::Init()
{
	Detour::Detour64(&do_dlsymDetour, g_KernelAddrs.do_dlsym, (void*)do_dlsymHook);
}

void Bootstrapper::Term()
{
	delete do_dlsymDetour;
}