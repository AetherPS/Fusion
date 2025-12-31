#include "common.h"
#include "dynlib.h"

// libSceLibcInternal
int(*vsprintf)(char* s, const char* format, va_list arg);
char* (*strcpy)(char* destination, const char* source);
void* (*memcpy)(void* destination, const void* source, size_t size);
void* (*malloc)(size_t size);
void (*free)(void* ptr);

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

void ResolveDynlib()
{
	// libSceLibcInternal is always module 2
	sys_dynlib_dlsym(2, "vsprintf", &vsprintf);
	sys_dynlib_dlsym(2, "strcpy", &strcpy);
	sys_dynlib_dlsym(2, "memcpy", &memcpy);
	sys_dynlib_dlsym(2, "malloc", &malloc);
	sys_dynlib_dlsym(2, "free", &free);

	// Libkernel is always module 8193
	sys_dynlib_dlsym(8193, "sceKernelDebugOutText", &sceKernelDebugOutText);
	sys_dynlib_dlsym(8193, "sceKernelOpen", &sceKernelOpen);
	sys_dynlib_dlsym(8193, "sceKernelClose", &sceKernelClose);
	sys_dynlib_dlsym(8193, "sceKernelWrite", &sceKernelWrite);
	sys_dynlib_dlsym(8193, "sceKernelUnlink", &sceKernelUnlink);
	sys_dynlib_dlsym(8193, "sceKernelMkdir", &sceKernelMkdir);
	sys_dynlib_dlsym(8193, "sceKernelLoadStartModule", &sceKernelLoadStartModule);
	sys_dynlib_dlsym(8193, "ioctl", &ioctl);
	sys_dynlib_dlsym(8193, "sceKernelStat", &sceKernelStat);
}