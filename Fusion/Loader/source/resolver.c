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

uint64_t kernel_getbase() {
	return __readmsr(0xC0000082) - addr_Xfast_syscall;
}

void* kernel_alloc(uint32_t size) {
	return kernel_malloc(size, M_TEMP, 2);
}

void kernel_dealloc(void* addr) {
	kernel_free(addr, M_TEMP);
}

#define RESOLVE(name, base, offset) \
	name = (void *)(base + offset) \

void ResolveKernelFunctions(uint64_t kernelBase) 
{
	RESOLVE(M_TEMP, kernelBase, addr_M_TEMP);
	RESOLVE(kernel_malloc, kernelBase, addr_malloc);
	RESOLVE(kernel_free, kernelBase, addr_free);
	RESOLVE(kernel_memcpy, kernelBase, addr_memcpy);
	RESOLVE(kernel_memset, kernelBase, addr_memset);
	RESOLVE(kernel_memcmp, kernelBase, addr_memcmp);
	RESOLVE(kernel_printf, kernelBase, addr_printf);

	RESOLVE(kmem_alloc, kernelBase, addr_kmem_alloc);
	RESOLVE(kmem_free, kernelBase, addr_kmem_free);
	kernel_map = *(vm_map_t*)(kernelBase + addr_kernel_map);
}
