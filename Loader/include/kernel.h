#pragma once

extern uint8_t _binary_resources_Kernel_elf_start[];
extern uint8_t _binary_resources_Kernel_elf_end[];

struct installKernelArgs {
	uint64_t sycall;
	void* payload;
	size_t psize;
};



uint64_t kernel_getbase();
void* kernel_alloc(uint32_t size);
void kernel_dealloc(void* addr);

extern void* M_TEMP;
extern void* (*kernel_malloc)(unsigned long size, void* type, int flags);
extern void (*kernel_free)(void* addr, void* type);
extern void (*kernel_memcpy)(void* dst, const void* src, size_t len);
extern void* (*kernel_memset)(void* ptr, int value, size_t num);
extern int (*kernel_memcmp)(const void* ptr1, const void* ptr2, size_t num);
extern void(*kernel_printf)(const char* fmt, ...);
extern vm_offset_t(*kmem_alloc)(vm_map_t map, vm_size_t size);
extern void(*kmem_free)(void* map, void* addr, size_t size);
extern vm_map_t kernel_map;

void kernel_resolve(uint64_t kernbase);
int install_kernel(struct thread* td, struct installKernelArgs* args);
void LoadKernel();
