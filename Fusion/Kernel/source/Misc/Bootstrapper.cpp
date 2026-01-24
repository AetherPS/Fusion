#include "Common.h"
#include "Bootstrapper.h"

extern uint8_t _binary_Bootstrapper_start[];
extern uint8_t _binary_Bootstrapper_end[];

Detour* Bootstrapper::do_dlsymDetour;
void* Bootstrapper::do_dlsymHook(struct dynlib* dl, struct dynlib_obj* obj, char* name, char* libName, unsigned int flags)
{
	void* result = do_dlsymDetour->Invoke<void*>(dl, obj, name, libName, flags);

	// If the symbol is sceSysmodulePreloadModuleForLibkernel we hijack that to run our boot strapper.
	if (strstr(name, "sceSysmodulePreloadModuleForLibkernel"))
	{
		auto td = CurrentThread();
		uint64_t shellCodeMemory = 0;
		int res = AllocateMemory(td->td_proc, "Fusion Bootstrapper", (uint64_t)&_binary_Bootstrapper_end - (uint64_t)&_binary_Bootstrapper_start, VM_PROT_ALL, &shellCodeMemory);

		if (res != 0 || shellCodeMemory <= 0)
		{
			printf("Bootstrapper failed to allocate shellcode memory for reason %llX.\n", res);
			return result;
		}

		auto shellCodeHeader = (BootStrapperShellCodeHeader*)_binary_Bootstrapper_start;
		dynlib_dlsym(td->td_proc, 8193, "sceKernelLoadStartModule", nullptr, 0, (void**)&shellCodeHeader->sceKernelLoadStartModule);
		shellCodeHeader->sceSysmodulePreloadModuleForLibkernel = (uint64_t)result;

		if (ReadWriteProcessMemory(td, td->td_proc, (void*)shellCodeMemory, _binary_Bootstrapper_start, (uint64_t)&_binary_Bootstrapper_end - (uint64_t)&_binary_Bootstrapper_start, true) != 0)
		{
			printf("Bootstrapper failed to write shellcode.\n");
			return result;
		}

		return (void*)(shellCodeMemory + shellCodeHeader->entry);
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