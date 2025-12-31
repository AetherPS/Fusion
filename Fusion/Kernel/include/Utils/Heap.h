#pragma once

void* _malloc(long unsigned int size);
void _free(void* addr);
void* operator new(long unsigned int size);
void* operator new(long unsigned int size, void* addr);
void* operator new[](long unsigned int size);
void operator delete(void* addr) noexcept;
void operator delete[](void* addr) noexcept;