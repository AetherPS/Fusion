#include "common.h"
#include "kernel.h"
#include "patches.h"

void JailbreakProc(uint64_t kernelBase, struct proc* p)
{
	struct ucred* cred = p->p_ucred;
	struct filedesc* fd = p->p_fd;
	cred->cr_prison = *(void**)(kernelBase + addr_prison0); //prison0
	fd->fd_rdir = *(void**)(kernelBase + addr_rootvnode); //rootvnode
	fd->fd_jdir = *(void**)(kernelBase + addr_rootvnode); //rootvnode
	cred->cr_uid = 0;
	cred->cr_ruid = 0;
	cred->cr_rgid = 0;
	cred->cr_groups[0] = 0;
}

int InstallKernelElf(void* payload, size_t size)
{
	size_t msize = 0;
	if (elf_mapped_size(payload, &msize))
	{
		return 1;
	}

	int s = (msize + 0x3FFFull) & ~0x3FFFull;
	void* payloadbase = (void*)kmem_alloc(kernel_map, s);
	if (!payloadbase)
	{
		return 1;
	}

	int r = 0;
	int (*payload_entry)(void* p);

	if ((r = load_elf(payload, size, payloadbase, msize, (void**)&payload_entry)))
	{
		return r;
	}

	kernel_printf("Starting Kernel. (Entry: %llX, ELFBase: %llX, Size: %i)\n", payload_entry, payloadbase, s);

	if (payload_entry(0))
	{
		return 1;
	}

	return 0;
}

int installKernelSyscall(struct thread* td, struct installKernelArgs* args)
{
	uint64_t kernelBase = kernel_getbase();

	ResolveKernelFunctions(kernelBase);
	JailbreakProc(kernelBase, td->td_proc);		// Jailbreak the current process.
	InstallPatches(kernelBase);					// Install kernel patches.

	// Install the kernel ELF.
	return InstallKernelElf(args->payload, args->payloadSize);
}

void LoadKernel()
{
	klog("Installing Kernel ELF\n");

	// Check if Fusion Driver is already loaded.
	if (FileExist("/dev/Fusion"))
	{
		klog("Fusion Driver already loaded, skipping kernel installation.\n");
		return;
	}

	syscall(11, installKernelSyscall, _binary_resources_Kernel_elf_start, _binary_resources_Kernel_elf_end - _binary_resources_Kernel_elf_start);
}