#pragma once

int(*vsprintf)(char* s, const char* format, va_list arg);
char* (*strcpy)(char* destination, const char* source);
int (*sceKernelDebugOutText)(int dbg_channel, const char* text, ...);
int (*sceKernelOpen)(const char* path, int flags, SceKernelMode mode);
int (*sceKernelClose)(int d);
int (*ioctl)(int fd, unsigned long request, ...);
int (*sceKernelStat)(const char* path, SceKernelStat* sb);

void klog(const char* fmt, ...);
int sys_dynlib_dlsym(int loadedModuleID, const char* name, void* destination);
int sys_dynlib_load_prx(const char* name, int* idDestination);
void ResolveDynlib();