#pragma once
#include <DriverDefinitions.h>

#define DRIVER_PATH "/dev/Fusion"

int MakeDriverRequest(unsigned long request, void* input);

int Resolve(int processId, int libHandle, const char* library, const char* symbol, unsigned int flags, uint64_t* addr);