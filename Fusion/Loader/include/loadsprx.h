#pragma once

#define STACK_SIZE 0x80000

#pragma pack(push, 1)
struct SprxLoaderHeader
{
	uint32_t magic;
	uint64_t entry;

	uint64_t sceKernelLoadStartModule;
	uint64_t scePthreadExit;

	int32_t ModuleHandle;
	char Path[4096];
};
#pragma pack(pop)

extern uint8_t _binary_resources_LoaderShellCode_bin_start[];
extern uint8_t _binary_resources_LoaderShellCode_bin_end[];

#pragma pack(push, 1)
struct ThreadShellCodeHeader
{
	uint32_t magic;
	uint64_t entry;

	uint64_t thr_initial;
	uint64_t scePthreadCreate;
	uint64_t scePthreadJoin;
	uint64_t scePthreadExit;
	uint64_t ThreadEntry;
	uint8_t ShellCodeComplete;
};
#pragma pack(pop)

extern uint8_t _binary_resources_ThreadShellCode_bin_start[];
extern uint8_t _binary_resources_ThreadShellCode_bin_end[];

int StartPThread(int processId, uint64_t entryPoint);
int LoadSprx(const char* processName, const char* path);