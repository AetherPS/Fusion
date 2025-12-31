#pragma once

// libSceLibcInternal
extern int(*vsprintf)(char* s, const char* format, va_list arg);
extern char* (*strcpy)(char* destination, const char* source);
extern void* (*memcpy)(void* destination, const void* source, size_t size);
extern void* (*malloc)(size_t size);
extern void (*free)(void* ptr);

// libkernel
extern int (*sceKernelDebugOutText)(int dbg_channel, const char* text, ...);
extern int (*sceKernelOpen)(const char* path, int flags, SceKernelMode mode);
extern int (*sceKernelClose)(int fd);
extern int (*sceKernelUnlink)(const char* path);
extern int (*sceKernelWrite)(int fd, const void* data, size_t size);
extern int (*sceKernelMkdir)(const char* path, SceKernelMode mode);
extern int(*sceKernelLoadStartModule)(const char* name, size_t args, const void* argp, unsigned int flags, void* pOpt, void* pRes);
extern int (*ioctl)(int fd, unsigned long request, ...);
extern int (*sceKernelStat)(const char* path, SceKernelStat* sb);

void klog(const char* fmt, ...);
int sys_dynlib_dlsym(int loadedModuleID, const char* name, void* destination);
void ResolveDynlib();