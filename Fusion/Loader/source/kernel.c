#include "common.h"
#include "kernel.h"
#include "patches.h"

void JailbreakProc(uint64_t kernelBase, struct thread* td)
{
	struct ucred* cred = td->td_proc->p_ucred;
	struct filedesc* fd = td->td_proc->p_fd;

	cred->cr_prison = *(struct prison**)(kernelBase + addr_prison0); //prison0
	fd->fd_rdir = *(struct vnode**)(kernelBase + addr_rootvnode); //rootvnode
	fd->fd_jdir = *(struct vnode**)(kernelBase + addr_rootvnode); //rootvnode

	cred->cr_uid = 0;
	cred->cr_ruid = 0;
	cred->cr_rgid = 0;
	cred->cr_groups[0] = 0;

	cred->cr_sceAuthID = 0x3801000000000013;
	cred->cr_sceCaps[0] = 0xFFFFFFFFFFFFFFFF;
	cred->cr_sceCaps[1] = 0xFFFFFFFFFFFFFFFF;
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

int InstallKernelSyscall(struct thread* td, struct installKernelArgs* args)
{
	uint64_t kernelBase = kernel_getbase();

	ResolveKernelFunctions(kernelBase);
	JailbreakProc(kernelBase, td);						// Jailbreak the current process.
	InstallPatches(kernelBase);							// Install kernel patches.
	InstallKernelElf(args->payload, args->payloadSize); // Install the kernel ELF.
	
	return 0;
}

void LoadKernel()
{
	klog("Installing Kernel ELF\n");
	syscall(11, InstallKernelSyscall, _binary_resources_Kernel_elf_start, _binary_resources_Kernel_elf_end - _binary_resources_Kernel_elf_start);
	klog("Done.\n");
}