#include "common.h"
#include "patches.h"

void InstallPatches()
{
	uint8_t* kmem;

	cpu_disable_wp();

	// Patch memcpy: bypass stack bounds check (jcc -> jmp)
	*(uint8_t*)(g_KernelAddrs.patch_memcpy) = 0xEB;

	// Patch kmem_alloc to allow VM_PROT_ALL for kernel excutable memory allocations.
	*(uint8_t*)(g_KernelAddrs.patch_kmem_alloc1) = VM_PROT_ALL;
	*(uint8_t*)(g_KernelAddrs.patch_kmem_alloc2) = VM_PROT_ALL;

	// Disable ASLR
	kmem = (uint8_t*)g_KernelAddrs.patch_ASLR;
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	// Patch copyin/copyout: Allow userland + kernel addresses in both params
	// copyin
	kmem = (uint8_t*)g_KernelAddrs.patch_copyin1;
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	kmem = (uint8_t*)g_KernelAddrs.patch_copyin2;
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;

	// copyout
	kmem = (uint8_t*)g_KernelAddrs.patch_copyout1;
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	kmem = (uint8_t*)g_KernelAddrs.patch_copyout2;
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;

	// Patch copyinstr
	kmem = (uint8_t*)g_KernelAddrs.patch_copyinstr1;
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	kmem = (uint8_t*)g_KernelAddrs.patch_copyinstr2;
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;

	kmem = (uint8_t*)g_KernelAddrs.patch_copyinstr3;
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	// swword_lwpid check to see if child_tid/parent_tid is in kernel memory
	kmem = (uint8_t*)g_KernelAddrs.patch_swword_lwpid1;
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	kmem = (uint8_t*)g_KernelAddrs.patch_swword_lwpid2;
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	// ptrace Patches.
	kmem = (uint8_t*)g_KernelAddrs.patch_ptrace1;
	kmem[0] = 0xEB;

	kmem = (uint8_t*)g_KernelAddrs.patch_ptrace2;
	kmem[0] = 0xE9;
	kmem[1] = 0x7C;
	kmem[2] = 0x02;
	kmem[3] = 0x00;
	kmem[4] = 0x00;

	// sys_dynlib_get_info gives full module path.
	kmem = (uint8_t*)g_KernelAddrs.patch_dynlibPath1;
	kmem[0] = 0x4C;
	kmem[1] = 0x89;
	kmem[2] = 0xF8;
	kmem[3] = 0x90;
	kmem[4] = 0x90;

	kmem = (uint8_t*)g_KernelAddrs.patch_dynlibPath2;
	kmem[0] = 0x00;

	// flatz disable pfs signature check
	kmem = (uint8_t*)g_KernelAddrs.patch_disablepfsSig;
	kmem[0] = 0x31;
	kmem[1] = 0xC0;
	kmem[2] = 0xC3;

	// flatz enable debug RIFs
	kmem = (uint8_t*)g_KernelAddrs.patch_debugRif1;
	kmem[0] = 0xB0;
	kmem[1] = 0x01;
	kmem[2] = 0xC3;

	kmem = (uint8_t*)g_KernelAddrs.patch_debugRif2;
	kmem[0] = 0xB0;
	kmem[1] = 0x01;
	kmem[2] = 0xC3;

	// Patch debug setting errors
	kmem = (uint8_t*)g_KernelAddrs.patch_debugSettings1;
	kmem[0] = 0x00;
	kmem[1] = 0x00;
	kmem[2] = 0x00;
	kmem[3] = 0x00;

	kmem = (uint8_t*)g_KernelAddrs.patch_debugSettings2;
	kmem[0] = 0x00;
	kmem[1] = 0x00;
	kmem[2] = 0x00;
	kmem[3] = 0x00;

	// Enable mount for unprivileged user
	kmem = (uint8_t*)g_KernelAddrs.patch_mount;
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;
	kmem[3] = 0x90;
	kmem[4] = 0x90;
	kmem[5] = 0x90;

	// Patch setuid: Don't run kernel exploit more than once/privilege escalation
	kmem = (uint8_t*)g_KernelAddrs.patch_setuid;
	kmem[0] = 0xB8;
	kmem[1] = 0x00;
	kmem[2] = 0x00;
	kmem[3] = 0x00;
	kmem[4] = 0x00;

	// Patch sys_mmap: Allow RWX (read-write-execute) mapping
	kmem = (uint8_t*)g_KernelAddrs.patch_sysmap;
	kmem[0] = 0x37;
	kmem[3] = 0x37;

	// Patch sys_dynlib_dlsym: Allow from anywhere
	kmem = (uint8_t*)g_KernelAddrs.patch_dynlib_dlsym1;
	kmem[0] = 0xEB;
	kmem[1] = 0x4C;

	kmem = (uint8_t*)g_KernelAddrs.patch_dynlib_dlsym2;
	kmem[0] = 0x48;
	kmem[1] = 0x31;
	kmem[2] = 0xC0;
	kmem[3] = 0xC3;

	// Enable display_dump
	kmem = (uint8_t*)g_KernelAddrs.patch_display_dump;
	kmem[0] = 0xEB;

	// Enable *all* debugging logs (in vprintf)
	// Patch by: SiSTRo
	kmem = (uint8_t*)g_KernelAddrs.patch_debuglogs;
	kmem[0] = 0xEB;
	kmem[1] = 0x3B;

	// ============================== DECI Patches ==============================

	// skip devkit/testkit/dipsw check in fuse_loader CreditL LM
	kmem = (uint8_t*)g_KernelAddrs.patch_fuseLoader;
	kmem[0] = 0xEB;
	kmem[1] = 0x1B;

	// Mount Fuse filesystem as root. Credit: Z80 & Faultz
	kmem = (uint8_t*)g_KernelAddrs.patch_fuseroot1;
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	kmem = (uint8_t*)g_KernelAddrs.patch_fuseroot2;
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	// TargetId of Devkit.
	*(uint8_t*)(g_KernelAddrs.QAFlags + 0x3D) = 0x81;
	*(uint8_t*)(g_KernelAddrs.QAFlags + 0x3E) = 0x02;

	// Feature gates?
	*(uint8_t*)(g_KernelAddrs.QAFlags + 0x36) |= 0x14;

	// Enable UL/SL debugger
	*(uint8_t*)(g_KernelAddrs.QAFlags + 0x59) |= 0x3;

	// Enable PSM int dev & int dev
	*(uint8_t*)(g_KernelAddrs.QAFlags + 0x58) |= 0x14;

	// Debug menu
	*(uint8_t*)(g_KernelAddrs.QAFlags + 0x5A) |= 0x1;

	// Store Mode
	*(uint8_t*)(g_KernelAddrs.QAFlags + 0x78) |= 0x1;

	// Patch to remove devkit check in dmamini_initialize_ioctl
	kmem = (uint8_t*)g_KernelAddrs.patch_dmamini0;
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	kmem = (uint8_t*)g_KernelAddrs.patch_dmamini1;
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	// patch sceKernelIsAssistMode to always return true in mdbg_basic_callback
	for (int i = 0; i < 22; i++)
	{
		if (g_KernelAddrs.mdbgAssistMode[i] == 0) break;

		kmem = (uint8_t*)g_KernelAddrs.mdbgAssistMode[i];
		kmem[0] = 0xB8;
		kmem[1] = 0x01;
		kmem[2] = 0x00;
		kmem[3] = 0x00;
		kmem[4] = 0x00;
	}

	// ==========================================================================

	// remove panic: mpage
	kmem = (uint8_t*)g_KernelAddrs.patch_mpage_panic;
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;
	kmem[3] = 0x90;
	kmem[4] = 0x90;
	kmem[5] = 0x90;

	// remove panic: vputx: negative ref count
	kmem = (uint8_t*)g_KernelAddrs.patch_vputx_panic;
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;
	kmem[3] = 0x90;
	kmem[4] = 0x90;
	kmem[5] = 0x90;

	// Patch to remove vm_fault: fault on nofault entry, addr %llx
	kmem = (uint8_t*)g_KernelAddrs.patch_vm_fault_panic;
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;
	kmem[3] = 0x90;
	kmem[4] = 0x90;
	kmem[5] = 0x90;

	// ==========================================================================
	cpu_enable_wp();
}