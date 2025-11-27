#pragma once

#define JUMP_32SIZE 5
#define JUMP_64SIZE 14
#define CALL_32SIZE JUMP_32SIZE

void* KmemAllocAt(vm_map_t map, vm_ooffset_t offset, vm_offset_t size);
int MkDir(const char* path, int mode);
void MemcpyTextSeg(void* dst, void* src, size_t len);
bool IsRelativelyClose(void* address1, void* address2);
uintptr_t GetRelativeAddress(void* address, void* destination);
void WriteJump32(void* address, void* destination);
void WriteJump64(void* address, void* destination);
void WriteCall32(void* address, void* destination);
char* basename(char* path);