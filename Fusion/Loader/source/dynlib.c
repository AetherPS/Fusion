#include "common.h"
#include "dynlib.h"

// libSceLibcInternal
int (*vsprintf)(char* s, const char* format, va_list arg) = NULL;
char* (*strcpy)(char* destination, const char* source) = NULL;
void* (*memcpy)(void* destination, const void* source, size_t size) = NULL;
void* (*malloc)(size_t size) = NULL;
void (*free)(void* ptr) = NULL;
void* (*realloc)(void* ptr, size_t size) = NULL;
char* (*strdup)(const char* s) = NULL;
char* (*strstr)(char* s, char* s2) = NULL;
size_t(*strlen)(const char* s) = NULL;
char* (*strcat)(char*, const char*) = NULL;
int (*sprintf)(char*, const char*, ...) = NULL;

// libkernel
int (*sceKernelUsleep)(unsigned int microseconds) = NULL;
int (*sceKernelOpen)(const char* path, int flags, SceKernelMode mode) = NULL;
int (*sceKernelClose)(int fd) = NULL;
int (*sceKernelUnlink)(const char* path) = NULL;
int (*sceKernelWrite)(int fd, const void* data, size_t size) = NULL;
int (*sceKernelMkdir)(const char* path, SceKernelMode mode) = NULL;
uint64_t(*sceKernelGetProcessTime)(void) = NULL;
int (*ioctl)(int fd, unsigned long request, ...) = NULL;
int (*getpid)(void) = NULL;
int (*sysctl)(int* name, unsigned int namelen, void* oldp, size_t* oldlenp, void* newp, size_t newlen) = NULL;

int sys_dynlib_dlsym(int loadedModuleID, const char* name, void* destination)
{
	return syscall(591, loadedModuleID, name, destination);
}

int sys_dynlib_load_prx(const char* name, int* idDestination)
{
	return syscall(594, name, 0, idDestination, 0);
}

static int resolve_symbol(int module, const char* name, void** ptr)
{
	int ret = sys_dynlib_dlsym(module, name, ptr);

	if (ret != 0)
	{
		klog("[Fusion] dlsym failed for %s (module %d) - error: %d\n", name, module, ret);
		return -1;
	}

	if (*ptr == NULL)
	{
		klog("[Fusion] %s resolved to NULL (module %d)\n", name, module);
		return -1;
	}

	return 0;
}

int ResolveDynlib()
{
	// libSceLibcInternal (module 2)
	if (resolve_symbol(2, "vsprintf", (void**)&vsprintf) != 0) return -1;
	if (resolve_symbol(2, "strcpy", (void**)&strcpy) != 0) return -1;
	if (resolve_symbol(2, "memcpy", (void**)&memcpy) != 0) return -1;
	if (resolve_symbol(2, "malloc", (void**)&malloc) != 0) return -1;
	if (resolve_symbol(2, "free", (void**)&free) != 0) return -1;
	if (resolve_symbol(2, "realloc", (void**)&realloc) != 0) return -1;
	if (resolve_symbol(2, "strdup", (void**)&strdup) != 0) return -1;
	if (resolve_symbol(2, "strstr", (void**)&strstr) != 0) return -1;
	if (resolve_symbol(2, "strlen", (void**)&strlen) != 0) return -1;
	if (resolve_symbol(2, "strcat", (void**)&strcat) != 0) return -1;
	if (resolve_symbol(2, "sprintf", (void**)&sprintf) != 0) return -1;

	klog("[Fusion] libSceLibcInternal (module 2) resolved successfully\n");

	// libkernel (module 8193)
	if (resolve_symbol(8193, "sceKernelUsleep", (void**)&sceKernelUsleep) != 0) return -1;
	if (resolve_symbol(8193, "sceKernelOpen", (void**)&sceKernelOpen) != 0) return -1;
	if (resolve_symbol(8193, "sceKernelClose", (void**)&sceKernelClose) != 0) return -1;
	if (resolve_symbol(8193, "sceKernelWrite", (void**)&sceKernelWrite) != 0) return -1;
	if (resolve_symbol(8193, "sceKernelUnlink", (void**)&sceKernelUnlink) != 0) return -1;
	if (resolve_symbol(8193, "sceKernelMkdir", (void**)&sceKernelMkdir) != 0) return -1;
	if (resolve_symbol(8193, "sceKernelGetProcessTime", (void**)&sceKernelGetProcessTime) != 0) return -1;
	if (resolve_symbol(8193, "ioctl", (void**)&ioctl) != 0) return -1;
	if (resolve_symbol(8193, "getpid", (void**)&getpid) != 0) return -1;
	if (resolve_symbol(8193, "sysctl", (void**)&sysctl) != 0) return -1;

	klog("[Fusion] libkernel (module 8193) resolved successfully\n");
	klog("[Fusion] All symbols resolved successfully\n");

	return 0;
}