#pragma once
#include <DriverDefinitions.h>

#define DRIVER_PATH "/dev/Fusion"

int MakeDriverRequest(unsigned long request, void* input);

int Jailbreak(int processId, struct JailBackup* backup, uint64_t authId, bool nullSandboxPath);
int RestoreJail(int processId, struct JailBackup backup);
int ReadWriteMemory(int processId, uint64_t addr, void* data, size_t len, bool write);
int AllocateMemory(int processId, uint64_t* outAddress, size_t length, int protection);
int FreeMemory(int processId, uint64_t processAddress, size_t length);
int StartThread(int processId, uint64_t threadEntry, uint64_t stackMemory, size_t stackSize);
int Resolve(int processId, int libHandle, const char* library, const char* symbol, unsigned int flags, uint64_t* addr);