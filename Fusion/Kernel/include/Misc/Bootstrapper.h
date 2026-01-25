#pragma once

#pragma pack(push, 1)
struct BootStrapperShellCodeHeader
{
	uint64_t entry;
	uint64_t sceKernelLoadStartModule;
	uint64_t sceSysmodulePreloadModuleForLibkernel;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct UIBootStrapperShellCodeHeader
{
	uint64_t entry;
	uint64_t sceKernelLoadStartModule;
	uint64_t Original;
};
#pragma pack(pop)

class Bootstrapper
{
public:
	static void Init();
	static void Term();

private:
	static Detour* do_dlsymDetour;
	static void* do_dlsymHook(struct dynlib* dl, struct dynlib_obj* obj, char* name, char* libName, unsigned int flags);
};