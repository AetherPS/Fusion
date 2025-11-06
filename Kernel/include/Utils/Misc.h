#pragma once

int isprint(int ch);
void hexdump(void* ptr, int buflen, bool showAddress);
void MemcpyTextSeg(void* dst, void* src, size_t len);
void* AllocateForMap(vm_map_t map, vm_ooffset_t offset, vm_offset_t size, vm_prot_t prot, vm_prot_t max);
void* KmemAllocAt(vm_map_t map, vm_ooffset_t offset, vm_offset_t size);
bool GetSandboxPath(thread* td, char* sandboxPath);
int MkDir(const char* path, int mode);