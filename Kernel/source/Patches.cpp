#include "Common.h"
#include "Patches.h"

void InstallPatches()
{
    uint8_t* kmem;

    if (!KernelBase)
        return;

    cpu_disable_wp();

	// Disable ASLR
	kmem = (uint8_t*)KernelBase + patch_ASLR;
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	// Patch copyin/copyout: Allow userland + kernel addresses in both params
	// copyin
	kmem = (uint8_t*)KernelBase + patch_copyin1;
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	kmem = (uint8_t*)KernelBase + patch_copyin2;
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;

	// copyout
	kmem = (uint8_t*)KernelBase + patch_copyout1;
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	kmem = (uint8_t*)KernelBase + patch_copyout2;
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;

	// Patch copyinstr
	kmem = (uint8_t*)KernelBase + patch_copyinstr1;
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	kmem = (uint8_t*)KernelBase + patch_copyinstr2;
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;

	kmem = (uint8_t*)KernelBase + patch_copyinstr3;
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	// swword_lwpid check to see if child_tid/parent_tid is in kernel memory
	kmem = (uint8_t*)KernelBase + patch_swword_lwpid1;
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	kmem = (uint8_t*)KernelBase + patch_swword_lwpid2;
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	// ptrace Patches.
	kmem = (uint8_t*)KernelBase + patch_ptrace1;
	kmem[0] = 0xEB;

	kmem = (uint8_t*)KernelBase + patch_ptrace2;
	kmem[0] = 0xE9;
	kmem[1] = 0x7C;
	kmem[2] = 0x02;
	kmem[3] = 0x00;
	kmem[4] = 0x00;

	// sys_dynlib_get_info gives full module path.
	kmem = (uint8_t*)KernelBase + patch_dynlibPath1;
	kmem[0] = 0x4C;
	kmem[1] = 0x89;
	kmem[2] = 0xF8;
	kmem[3] = 0x90;
	kmem[4] = 0x90;

	kmem = (uint8_t*)KernelBase + patch_dynlibPath2;
	kmem[0] = 0x00;

	// flatz disable pfs signature check
	kmem = (uint8_t*)KernelBase + patch_disablepfsSig;
	kmem[0] = 0x31;
	kmem[1] = 0xC0;
	kmem[2] = 0xC3;

	// flatz enable debug RIFs
	kmem = (uint8_t*)KernelBase + patch_debugRif1;
	kmem[0] = 0xB0;
	kmem[1] = 0x01;
	kmem[2] = 0xC3;

	kmem = (uint8_t*)KernelBase + patch_debugRif2;
	kmem[0] = 0xB0;
	kmem[1] = 0x01;
	kmem[2] = 0xC3;

	// Patch debug setting errors
	kmem = (uint8_t*)KernelBase + patch_debugSettings1;
	kmem[0] = 0x00;
	kmem[1] = 0x00;
	kmem[2] = 0x00;
	kmem[3] = 0x00;

	kmem = (uint8_t*)KernelBase + patch_debugSettings2;
	kmem[0] = 0x00;
	kmem[1] = 0x00;
	kmem[2] = 0x00;
	kmem[3] = 0x00;

	// Enable mount for unprivileged user
	kmem = (uint8_t*)KernelBase + patch_mount;
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;
	kmem[3] = 0x90;
	kmem[4] = 0x90;
	kmem[5] = 0x90;

	// Patch setuid: Don't run kernel exploit more than once/privilege escalation
	kmem = (uint8_t*)KernelBase + patch_setuid;
	kmem[0] = 0xB8;
	kmem[1] = 0x00;
	kmem[2] = 0x00;
	kmem[3] = 0x00;
	kmem[4] = 0x00;

	// Patch sys_mmap: Allow RWX (read-write-execute) mapping
	kmem = (uint8_t*)KernelBase + patch_sysmap;
	kmem[0] = 0x37;
	kmem[3] = 0x37;

	// Patch sys_dynlib_dlsym: Allow from anywhere
	kmem = (uint8_t*)KernelBase + patch_dynlib_dlsym1;
	kmem[0] = 0xEB;
	kmem[1] = 0x4C;

	kmem = (uint8_t*)KernelBase + patch_dynlib_dlsym2;
	kmem[0] = 0x48;
	kmem[1] = 0x31;
	kmem[2] = 0xC0;
	kmem[3] = 0xC3;

	// Enable display_dump
	kmem = (uint8_t*)KernelBase + patch_display_dump;
	kmem[0] = 0xEB;

	// Enable *all* debugging logs (in vprintf)
	// Patch by: SiSTRo
	kmem = (uint8_t*)KernelBase + patch_debuglogs;
	kmem[0] = 0xEB;
	kmem[1] = 0x3B;

	// skip devkit/testkit/dipsw check in fuse_loader CreditL LM
	kmem = (uint8_t*)KernelBase + patch_fuseLoader;
	kmem[0] = 0xEB;
	kmem[1] = 0x1B;

	// Mount Fuse filesystem as root. Credi: Z80 & Faultz
	kmem = (uint8_t*)KernelBase + patch_fuseroot1;
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	kmem = (uint8_t*)KernelBase + patch_fuseroot2;
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	// TargetId of Devkit.
	*(uint8_t*)(KernelBase + addr_QAFlags + 0x3D) = 0x81;
	*(uint8_t*)(KernelBase + addr_QAFlags + 0x3E) = 0x02;

	// Enable UL/SL debugger
	*(uint8_t*)(KernelBase + addr_QAFlags + 0x36) |= 0x4;
	*(uint8_t*)(KernelBase + addr_QAFlags + 0x59) |= 0x2;
	*(uint8_t*)(KernelBase + addr_QAFlags + 0x59) |= 0x1;

	// Patch to remove devkit check in dmamini_initialize_ioctl
	kmem = (uint8_t*)KernelBase + patch_dmamini0;
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	kmem = (uint8_t*)KernelBase + patch_dmamini1;
	kmem[0] = 0x90;
	kmem[1] = 0x90;

#if defined(VERSION_900)

	// remove panic: mpage
	kmem = (uint8_t*)KernelBase + 0x884BE;
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;
	kmem[3] = 0x90;
	kmem[4] = 0x90;
	kmem[5] = 0x90;

	// remove panic: vputx: negative ref count
	kmem = (uint8_t*)KernelBase + 0x235D72;
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;
	kmem[3] = 0x90;
	kmem[4] = 0x90;
	kmem[5] = 0x90;

	// Patch to remove vm_fault: fault on nofault entry, addr %llx
	kmem = (uint8_t*)KernelBase + 0x00152966;
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;
	kmem[3] = 0x90;
	kmem[4] = 0x90;
	kmem[5] = 0x90;

	// Set sceKernelIsAssistMode to return true
	kmem = (uint8_t*)KernelBase + 0x656170;
	kmem[0] = 0x48;
	kmem[1] = 0xC7;
	kmem[2] = 0xC0;
	kmem[3] = 0x01;
	kmem[4] = 0x00;
	kmem[5] = 0x00;
	kmem[6] = 0x00;
	kmem[7] = 0xC3;

#elif defined(VERSION_1100)

	// remove panic: mpage
	kmem = (uint8_t*)KernelBase + 0x36434E;
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;
	kmem[3] = 0x90;
	kmem[4] = 0x90;
	kmem[5] = 0x90;

	// remove panic: vputx: negative ref count
	kmem = (uint8_t*)KernelBase + 0x37E742;
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;
	kmem[3] = 0x90;
	kmem[4] = 0x90;
	kmem[5] = 0x90;

	// Patch to remove vm_fault: fault on nofault entry, addr %llx
	kmem = (uint8_t*)KernelBase + 0x3208A0;
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;
	kmem[3] = 0x90;
	kmem[4] = 0x90;
	kmem[5] = 0x90;

	kmem = (uint8_t*)KernelBase + 0x31E8A6;
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;
	kmem[3] = 0x90;
	kmem[4] = 0x90;
	kmem[5] = 0x90;

	// Patch Mprotect to allow changing to RWX
	kmem = (uint8_t*)KernelBase + 0x35C8EC;
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;
	kmem[3] = 0x90;
	kmem[4] = 0x90;
	kmem[5] = 0x90;

	// Set sceKernelIsAssistMode to return true
	kmem = (uint8_t*)KernelBase + 0x655360;
	kmem[0] = 0x48;
	kmem[1] = 0xC7;
	kmem[2] = 0xC0;
	kmem[3] = 0x01;
	kmem[4] = 0x00;
	kmem[5] = 0x00;
	kmem[6] = 0x00;
	kmem[7] = 0xC3;

#elif defined(VERSION_1202)

	// Set sceKernelIsAssistMode to return true
	kmem = (uint8_t*)KernelBase + 0x655620;
	kmem[0] = 0x48;
	kmem[1] = 0xC7;
	kmem[2] = 0xC0;
	kmem[3] = 0x01;
	kmem[4] = 0x00;
	kmem[5] = 0x00;
	kmem[6] = 0x00;
	kmem[7] = 0xC3;

#endif

    cpu_enable_wp();
}