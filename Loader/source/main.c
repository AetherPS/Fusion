#include "common.h"

extern uint8_t _binary_resources_Kernel_elf_start[];
extern uint8_t _binary_resources_Kernel_elf_end[];

struct installKernelArgs {
	uint64_t sycall;
	void* payload;
	size_t psize;
};

int install_kernel(struct thread* td, struct installKernelArgs* args)
{
	return 0;
}


int _main(void)
{
	ResolveDynlib();

	klog("Installing Kernel ELF\n");
	syscall(11, install_kernel, _binary_resources_Kernel_elf_start, _binary_resources_Kernel_elf_end - _binary_resources_Kernel_elf_start);

	klog("All Done\n");

	return 0;
}