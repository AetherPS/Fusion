#include "common.h"
#include "dynlib.h"

// libSceLibcInternal
int(*vsprintf)(char* s, const char* format, va_list arg);
char* (*strcpy)(char* destination, const char* source);

// libkernel
int (*sceKernelDebugOutText)(int dbg_channel, const char* text, ...);
int (*sceKernelOpen)(const char* path, int flags, SceKernelMode mode);
int (*sceKernelClose)(int fd);
int (*sceKernelUnlink)(const char* path);
int (*sceKernelWrite)(int fd, const void* data, size_t size);
int (*sceKernelMkdir)(const char* path, SceKernelMode mode);
int(*sceKernelLoadStartModule)(const char* name, size_t args, const void* argp, unsigned int flags, void* pOpt, void* pRes);
int (*ioctl)(int fd, unsigned long request, ...);
int (*sceKernelStat)(const char* path, SceKernelStat* sb);

void klog(const char* fmt, ...)
{
	char Buffer[0x200];

	//Create full string from va list.
	va_list args;
	va_start(args, fmt);
	vsprintf(Buffer, fmt, args);
	va_end(args);

	sceKernelDebugOutText(0, Buffer);
}

int sys_dynlib_dlsym(int loadedModuleID, const char* name, void* destination)
{
	return syscall(591, loadedModuleID, name, destination);
}

int sys_dynlib_load_prx(const char* name, int* idDestination)
{
	return syscall(594, name, 0, idDestination, 0);
}

void ResolveDynlib()
{
	int handle;
	sys_dynlib_load_prx("libSceLibcInternal.sprx", &handle);
	sys_dynlib_dlsym(handle, "vsprintf", &vsprintf);
	sys_dynlib_dlsym(handle, "strcpy", &strcpy);

	if (sys_dynlib_load_prx("libkernel.sprx", &handle))
	{
		if (sys_dynlib_load_prx("libkernel_web.sprx", &handle))
		{
			sys_dynlib_load_prx("libkernel_sys.sprx", &handle);
		}
	}
		
	sys_dynlib_dlsym(handle, "sceKernelDebugOutText", &sceKernelDebugOutText);
	sys_dynlib_dlsym(handle, "sceKernelOpen", &sceKernelOpen);
	sys_dynlib_dlsym(handle, "sceKernelClose", &sceKernelClose);
	sys_dynlib_dlsym(handle, "sceKernelWrite", &sceKernelWrite);
	sys_dynlib_dlsym(handle, "sceKernelUnlink", &sceKernelUnlink);
	sys_dynlib_dlsym(handle, "sceKernelMkdir", &sceKernelMkdir);
	sys_dynlib_dlsym(handle, "sceKernelLoadStartModule", &sceKernelLoadStartModule);
	sys_dynlib_dlsym(handle, "ioctl", &ioctl);
	sys_dynlib_dlsym(handle, "sceKernelStat", &sceKernelStat);
}