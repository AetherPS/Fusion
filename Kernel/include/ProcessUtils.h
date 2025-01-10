#pragma once

proc* GetProcByPid(int pid);
proc* GetProcByName(const char* name);
int ReadWriteProcessMemory(thread* td, proc* proc, void* addr, void* data, uint32_t len, bool write);
int CreateThread(thread* td, void* entry, void* arg, char* stack, size_t stackSize);
caddr_t AllocateMemory(thread* td, size_t len, int prot, int flags);
int FreeMemory(thread* td, caddr_t addr, size_t len);