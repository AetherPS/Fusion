#pragma once

typedef mode_t SceKernelMode;

#define SCE_KERNEL_NAME_MAX        255
#define SCE_KERNEL_PATH_MAX        1024
#define SCE_KERNEL_IOV_MAX         1024
#define SCE_KERNEL_MAXNAMLEN       255

#define SCE_KERNEL_O_RDONLY        0x0000
#define SCE_KERNEL_O_WRONLY        0x0001 
#define SCE_KERNEL_O_RDWR          0x0002

#define SCE_KERNEL_O_CREAT		   0x0200

// libSceLibcInternal
extern int(*vsprintf)(char* s, const char* format, va_list arg);
extern char* (*strcpy)(char* destination, const char* source);
extern void* (*memcpy)(void* destination, const void* source, size_t size);
extern void* (*malloc)(size_t size);
extern void (*free)(void* ptr);
extern void* (*realloc)(void* ptr, size_t size);
extern char* (*strdup)(const char* s);
extern char* (*strstr)(char* s, char* s2);
extern size_t(*strlen)(const char* s);
extern char* (*strcat)(char*, const char*);
extern int (*sprintf)(char*, const char*, ...);

// libkernel
extern int (*sceKernelUsleep)(unsigned int microseconds);
extern int (*sceKernelOpen)(const char* path, int flags, SceKernelMode mode);
extern int (*sceKernelClose)(int fd);
extern int (*sceKernelUnlink)(const char* path);
extern int (*sceKernelWrite)(int fd, const void* data, size_t size);
extern int (*sceKernelMkdir)(const char* path, SceKernelMode mode);
extern uint64_t (*sceKernelGetProcessTime)(void);
extern int (*ioctl)(int fd, unsigned long request, ...);
extern int (*getpid)(void);
extern int (*sysctl)(int* name, unsigned int namelen, void* oldp, size_t* oldlenp, void* newp, size_t newlen);

int sys_dynlib_dlsym(int loadedModuleID, const char* name, void* destination);
int sys_dynlib_load_prx(const char* name, int* idDestination);
int ResolveDynlib();