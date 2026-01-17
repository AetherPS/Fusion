#pragma once

extern uint8_t _binary_resources_Kernel_elf_compressed_start[];
extern uint8_t _binary_resources_Kernel_elf_compressed_end[];

struct installKernelArgs {
	uint64_t sycall;
	void* payload;
	size_t payloadSize;
};

extern KernelAddrs g_KernelAddrs;

void LoadKernel();
