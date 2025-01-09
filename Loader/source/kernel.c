#include "common.h"
#include "kernel.h"

namespace Kernel
{
	uint64_t GetBase() {
		return __readmsr(0xC0000082) - addr_Xfast_syscall;
	}

	void* alloc(uint32_t size) {
		return malloc(size, M_TEMP, 2);
	}

	void dealloc(void* addr) {
		free(addr, M_TEMP);
	}

#define RESOLVE(name, base, offset) \
	name = (void *)(base + offset) \

	void resolve(uint64_t kernbase) {
		RESOLVE(M_TEMP, kernbase, addr_M_TEMP);
		RESOLVE(malloc, kernbase, addr_malloc);
		RESOLVE(free, kernbase, addr_free);
		RESOLVE(memcpy, kernbase, addr_memcpy);
		RESOLVE(memset, kernbase, addr_memset);
		RESOLVE(memcmp, kernbase, addr_memcmp);
		RESOLVE(kprintf, kernbase, addr_kprintf);

		RESOLVE(kmem_alloc, kernbase, addr_kmem_alloc);
		RESOLVE(kmem_free, kernbase, addr_kmem_free);
		kernel_map = *(vm_map_t*)(kernbase + addr_kernel_map);
	}
}
