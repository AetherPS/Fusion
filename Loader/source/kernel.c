#include "common.h"
#include "kernel.h"

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

void kernel_resolve(uint64_t kernbase) {
	RESOLVE(M_TEMP, kernbase, addr_M_TEMP);
	RESOLVE(kernel_malloc, kernbase, addr_malloc);
	RESOLVE(kernel_free, kernbase, addr_free);
	RESOLVE(kernel_memcpy, kernbase, addr_memcpy);
	RESOLVE(kernel_memset, kernbase, addr_memset);
	RESOLVE(kernel_memcmp, kernbase, addr_memcmp);
	RESOLVE(kernel_printf, kernbase, addr_kprintf);

	RESOLVE(kmem_alloc, kernbase, addr_kmem_alloc);
	RESOLVE(kmem_free, kernbase, addr_kmem_free);
	kernel_map = *(vm_map_t*)(kernbase + addr_kernel_map);
}
