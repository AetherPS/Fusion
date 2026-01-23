#include "Common.h"
#include "Bootstrapper.h"

unsigned char BootstrapperShellCode[] = {
	0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x2F, 0x64, 0x61, 0x74, 0x61, 0x2F, 0x46, 0x75, 0x73, 0x69, 0x6F, 0x6E,
	0x2F, 0x50, 0x6C, 0x75, 0x67, 0x69, 0x6E, 0x73, 0x2F, 0x42, 0x6F, 0x6F,
	0x74, 0x73, 0x74, 0x72, 0x61, 0x70, 0x70, 0x65, 0x72, 0x2E, 0x73, 0x70,
	0x72, 0x78, 0x00, 0x4D, 0x31, 0xC9, 0x4D, 0x31, 0xC0, 0x48, 0x31, 0xC9,
	0x48, 0x31, 0xD2, 0x48, 0x31, 0xF6, 0x48, 0x8D, 0x3D, 0xC3, 0xFF, 0xFF,
	0xFF, 0x48, 0x8B, 0x05, 0xAC, 0xFF, 0xFF, 0xFF, 0xFF, 0xD0, 0x48, 0x8B,
	0x05, 0xAB, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0
};

Detour* Bootstrapper::do_dlsymDetour;
void* Bootstrapper::do_dlsymHook(struct dynlib* dl, struct dynlib_obj* obj, char* name, char* libName, unsigned int flags)
{
	void* result = do_dlsymDetour->Invoke<void*>(dl, obj, name, libName, flags);

	// If the symbol is sceSysmodulePreloadModuleForLibkernel we hijack that to run our boot strapper.
	if (strstr(name, "sceSysmodulePreloadModuleForLibkernel"))
	{
		auto td = CurrentThread();
		uint64_t shellCodeMemory = AllocateMemory(td->td_proc, sizeof(BootstrapperShellCode), VM_PROT_ALL, 0x1000 | 0x00040000);

		if (shellCodeMemory <= 0)
		{
			printf("Bootstrapper failed to allocate shellcode memory.\n");
			return result;
		}

		auto shellCodeHeader = (BootStrapperShellCodeHeader*)BootstrapperShellCode;
		dynlib_dlsym(td->td_proc, 8193, "sceKernelLoadStartModule", nullptr, 0, (void**)&shellCodeHeader->sceKernelLoadStartModule);
		shellCodeHeader->sceSysmodulePreloadModuleForLibkernel = (uint64_t)result;

		if (ReadWriteProcessMemory(td, td->td_proc, (void*)shellCodeMemory, BootstrapperShellCode, sizeof(BootstrapperShellCode), true) != 0)
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