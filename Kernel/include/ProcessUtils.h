#pragma once

proc* GetProcByName(const char* name);
int ReadWriteProcessMemory(thread* td, proc* proc, void* addr, void* data, uint32_t len, bool write);
int CreateThread(thread* td, void* entry, void* arg, char* stack, size_t stackSize);
caddr_t AllocateMemory(thread* td, size_t len, int prot, int flags);
int FreeMemory(thread* td, caddr_t addr, size_t len);
int dynlib_dlsym(proc* p, int handle, char* symbol, char* library, unsigned int flags, void* addr);
int dynlib_get_list(proc* p, int* handles, int max_handles, int* handle_count);
int dynlib_get_info(proc* p, int handle, OrbisLibraryInfo* info);