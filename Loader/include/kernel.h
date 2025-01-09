#pragma once

namespace Kernel
{
	void* M_TEMP;

	uint64_t GetBase();
	void* alloc(uint32_t size);
	void dealloc(void* addr);

	void* (*malloc)(unsigned long size, void* type, int flags);
	void (*free)(void* addr, void* type);
	void (*memcpy)(void* dst, const void* src, size_t len);
	void* (*memset)(void* ptr, int value, size_t num);
	int (*memcmp)(const void* ptr1, const void* ptr2, size_t num);
	void(*kprintf)(const char* fmt, ...);
	vm_offset_t(*kmem_alloc)(vm_map_t map, vm_size_t size);
	void(*kmem_free)(void* map, void* addr, size_t size);
	vm_map_t kernel_map;

	void resolve(uint64_t kernbase);
}