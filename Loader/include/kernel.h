#pragma once

void* M_TEMP;

uint64_t kernel_getbase();
void* kernel_alloc(uint32_t size);
void kernel_dealloc(void* addr);

void* (*kernel_malloc)(unsigned long size, void* type, int flags);
void (*kernel_free)(void* addr, void* type);
void (*kernel_memcpy)(void* dst, const void* src, size_t len);
void* (*kernel_memset)(void* ptr, int value, size_t num);
int (*kernel_memcmp)(const void* ptr1, const void* ptr2, size_t num);
void(*kernel_printf)(const char* fmt, ...);
vm_offset_t(*kmem_alloc)(vm_map_t map, vm_size_t size);
void(*kmem_free)(void* map, void* addr, size_t size);
vm_map_t kernel_map;

void kernel_resolve(uint64_t kernbase);
