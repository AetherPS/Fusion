#include "common.h"
#include "resolver.h"

void* M_TEMP;
void* (*kernel_malloc)(unsigned long size, void* type, int flags);
void (*kernel_free)(void* addr, void* type);
void (*kernel_memcpy)(void* dst, const void* src, size_t len);
void* (*kernel_memset)(void* ptr, int value, size_t num);
int (*kernel_memcmp)(const void* ptr1, const void* ptr2, size_t num);
void(*kernel_printf)(const char* fmt, ...);
vm_offset_t(*kmem_alloc)(vm_map_t map, vm_size_t size);
void(*kmem_free)(void* map, void* addr, size_t size);
vm_map_t kernel_map;

void* kernel_alloc(uint32_t size) {
	return kernel_malloc(size, M_TEMP, 2);
}

void kernel_dealloc(void* addr) {
	kernel_free(addr, M_TEMP);
}

void ResolveKernelFunctions()
{
	M_TEMP = (void*)(g_KernelAddrs.M_TEMP);
	kernel_malloc = (void* (*)(unsigned long, void*, int))(g_KernelAddrs.malloc);
	kernel_free = (void (*)(void*, void*))(g_KernelAddrs.free);
	kernel_memcpy = (void (*)(void*, const void*, size_t))(g_KernelAddrs.memcpy);
	kernel_memset = (void* (*)(void*, int, size_t))(g_KernelAddrs.memset);
	kernel_memcmp = (int (*)(const void*, const void*, size_t))(g_KernelAddrs.memcmp);
	kernel_printf = (void (*)(const char*, ...))(g_KernelAddrs.printf);

	kmem_alloc = (vm_offset_t(*)(vm_map_t, vm_size_t))(g_KernelAddrs.kmem_alloc);
	kmem_free = (void(*)(void*, void*, size_t))(g_KernelAddrs.kmem_free);
	kernel_map = *(vm_map_t*)(g_KernelAddrs.kernel_map);
}
