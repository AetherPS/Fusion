#include "common.h"
#include "kernel.h"
#include "patches.h"
#include "compressedblob.h"

KernelAddrs g_KernelAddrs;

void JailbreakProc(uint64_t kernelBase, struct thread* td)
{
	struct ucred* cred = td->td_proc->p_ucred;
	struct filedesc* fd = td->td_proc->p_fd;

	cred->cr_prison = *(struct prison**)(g_KernelAddrs.prison0);
	fd->fd_rdir = *(struct vnode**)(g_KernelAddrs.rootvnode);
	fd->fd_jdir = *(struct vnode**)(g_KernelAddrs.rootvnode);

	cred->cr_uid = 0;
	cred->cr_ruid = 0;
	cred->cr_rgid = 0;
	cred->cr_groups[0] = 0;

	cred->cr_sceAuthID = 0x3801000000000013;
	cred->cr_sceCaps[0] = 0xFFFFFFFFFFFFFFFF;
	cred->cr_sceCaps[1] = 0xFFFFFFFFFFFFFFFF;
}

int InstallKernelElf(void* payload, size_t size, uint64_t kernelBase)
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
	int (*payload_entry)(uint64_t kernelBase, KernelAddrs* addrs);

	if ((r = load_elf(payload, size, payloadbase, msize, (void**)&payload_entry)))
	{
		return r;
	}

	kernel_printf("Starting Kernel. (Entry: %llX, ELFBase: %llX, Size: %i)\n", payload_entry, payloadbase, s);

	if (payload_entry(kernelBase, &g_KernelAddrs))
	{
		return 1;
	}

	return 0;
}

int InstallKernelSyscall(struct thread* td, struct installKernelArgs* args)
{
	uint64_t kernelBase = (uint64_t)(Readmsr(0xC0000082) - 0x1C0);

	/* Detect firmware and load offsets */
	const char* detectedFirmware = InitializeFirmwareOffsets(&g_KernelAddrs, kernelBase);
	if (detectedFirmware == 0)
	{
		/* Failed to detect firmware */
		return 1;
	}

	ResolveKernelFunctions();		// Resolve the Kernel funcs for the loader.
	JailbreakProc(kernelBase, td);	// Jailbreak the current process.
	InstallPatches();				// Install kernel patches.

	kernel_printf("Detected firmware: %s\n", detectedFirmware);

	/* Install the kernel ELF */
	return InstallKernelElf(args->payload, args->payloadSize, kernelBase);
}

void LoadKernel()
{
	klog("Installing Kernel ELF\n");

	// Decompress the Kernel.elf
	size_t decompressedSize;
	uint8_t* decompressedKernel = DecompressBlob(_binary_resources_Kernel_elf_compressed_start, &decompressedSize);

	if (!decompressedKernel)
	{
		klog("Failed to decompress Kernel.elf\n");
		return;
	}

	// Install the decompressed kernel
	syscall(11, InstallKernelSyscall, decompressedKernel, decompressedSize);

	// Free the decompressed buffer
	free(decompressedKernel);

	klog("Done.\n");
}