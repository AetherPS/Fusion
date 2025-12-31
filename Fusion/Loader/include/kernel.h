#pragma once

extern uint8_t _binary_resources_Kernel_elf_start[];
extern uint8_t _binary_resources_Kernel_elf_end[];

struct installKernelArgs {
	uint64_t sycall;
	void* payload;
	size_t payloadSize;
};

void JailbreakProc(uint64_t kernelBase, struct proc* p);
int InstallKernelElf(void* payload, size_t size);
int installKernelSyscall(struct thread* td, struct installKernelArgs* args);
void LoadKernel();
