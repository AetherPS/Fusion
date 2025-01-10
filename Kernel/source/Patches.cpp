#include "Common.h"
#include "Patches.h"

void InstallPatches()
{
    uint8_t* kmem;

    if (!KernelBase)
        return;

    cpu_disable_wp();

#if defined(SOFTWARE_VERSION_900)

	// Disable ASLR
	kmem = (uint8_t*)KernelBase + 0x0005F824;
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	// Patch copyin/copyout: Allow userland + kernel addresses in both params
	// copyin
	kmem = (uint8_t*)KernelBase + 0x002716F7;
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	kmem = (uint8_t*)KernelBase + 0x00271703;
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;

	// copyout
	kmem = (uint8_t*)KernelBase + 0x00271602;
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	kmem = (uint8_t*)KernelBase + 0x0027160E;
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;

	// Patch copyinstr
	kmem = (uint8_t*)KernelBase + 0x00271BA3;
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	kmem = (uint8_t*)KernelBase + 0x00271BAF;
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;

	kmem = (uint8_t*)KernelBase + 0x00271BE0;
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	// swword_lwpid check to see if child_tid/parent_tid is in kernel memory
	kmem = (uint8_t*)KernelBase + 0x2719C2;
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	kmem = (uint8_t*)KernelBase + 0x2719D1;
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	// ptrace Patches.
	kmem = (uint8_t*)KernelBase + 0x41F4E5;
	kmem[0] = 0xEB;

	kmem = (uint8_t*)KernelBase + 0x41F9D1;
	kmem[0] = 0xE9;
	kmem[1] = 0x7C;
	kmem[2] = 0x02;
	kmem[3] = 0x00;
	kmem[4] = 0x00;

	// sys_dynlib_get_info gives full module path.
	kmem = (uint8_t*)KernelBase + 0x23C19F;
	kmem[0] = 0x4C;
	kmem[1] = 0x89;
	kmem[2] = 0xF8;
	kmem[3] = 0x90;
	kmem[4] = 0x90;

	kmem = (uint8_t*)KernelBase + 0x23C1AA;
	kmem[0] = 0x00;

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

	// flatz disable pfs signature check
	kmem = (uint8_t*)KernelBase + 0x006885C0;
	kmem[0] = 0x31;
	kmem[1] = 0xC0;
	kmem[2] = 0xC3;

	// flatz enable debug RIFs
	kmem = (uint8_t*)KernelBase + 0x00650430;
	kmem[0] = 0xB0;
	kmem[1] = 0x01;
	kmem[2] = 0xC3;

	kmem = (uint8_t*)KernelBase + 0x00650460;
	kmem[0] = 0xB0;
	kmem[1] = 0x01;
	kmem[2] = 0xC3;

	// Patch debug setting errors
	kmem = (uint8_t*)KernelBase + 0x004E8E48;
	kmem[0] = 0x00;
	kmem[1] = 0x00;
	kmem[2] = 0x00;
	kmem[3] = 0x00;

	kmem = (uint8_t*)KernelBase + 0x004EA12F;
	kmem[0] = 0x00;
	kmem[1] = 0x00;
	kmem[2] = 0x00;
	kmem[3] = 0x00;

	// Enable *all* debugging logs (in vprintf)
	// Patch by: SiSTRo
	kmem = (uint8_t*)KernelBase + 0x000B7B17;
	kmem[0] = 0xEB;
	kmem[1] = 0x3B;

	// Enable mount for unprivileged user
	kmem = (uint8_t*)KernelBase + 0x0004ADE7;
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;
	kmem[3] = 0x90;
	kmem[4] = 0x90;
	kmem[5] = 0x90;

	// Patch setuid: Don't run kernel exploit more than once/privilege escalation
	kmem = (uint8_t*)KernelBase + 0x000019FF;
	kmem[0] = 0xB8;
	kmem[1] = 0x00;
	kmem[2] = 0x00;
	kmem[3] = 0x00;
	kmem[4] = 0x00;

	// Patch sys_mmap: Allow RWX (read-write-execute) mapping
	kmem = (uint8_t*)KernelBase + 0x0016632A;
	kmem[0] = 0x37;
	kmem[3] = 0x37;

	// Patch sys_dynlib_dlsym: Allow from anywhere
	kmem = (uint8_t*)KernelBase + 0x0023B67F;
	kmem[0] = 0xEB;
	kmem[1] = 0x4C;

	kmem = (uint8_t*)KernelBase + 0x00221B40;
	kmem[0] = 0x48;
	kmem[1] = 0x31;
	kmem[2] = 0xC0;
	kmem[3] = 0xC3;

	// Enable display_dump
	kmem = (uint8_t*)KernelBase + 0x0028A75;
	kmem[0] = 0xEB;

	// skip devkit/testkit/dipsw check in fuse_loader
	kmem = (uint8_t*)KernelBase + 0x049074E;
	kmem[0] = 0xEB;
	kmem[1] = 0x1B;

	// Patch sceSblRcMgrIsAllowSLDebugger
	kmem = (uint8_t*)KernelBase + 0x317D30;
	kmem[0] = 0x48;
	kmem[1] = 0xC7;
	kmem[2] = 0xC0;
	kmem[3] = 0x01;
	kmem[4] = 0x00;
	kmem[5] = 0x00;
	kmem[6] = 0x00;
	kmem[7] = 0xC3;

	// Patch sceSblRcMgrIsAllowULDebugger
	kmem = (uint8_t*)KernelBase + 0x318030;
	kmem[0] = 0x48;
	kmem[1] = 0xC7;
	kmem[2] = 0xC0;
	kmem[3] = 0x01;
	kmem[4] = 0x00;
	kmem[5] = 0x00;
	kmem[6] = 0x00;
	kmem[7] = 0xC3;

	// Patch sceSblRcMgrIsSoftwagnerQafForAcmgr
	kmem = (uint8_t*)KernelBase + 0x318110;
	kmem[0] = 0x48;
	kmem[1] = 0xC7;
	kmem[2] = 0xC0;
	kmem[3] = 0x01;
	kmem[4] = 0x00;
	kmem[5] = 0x00;
	kmem[6] = 0x00;
	kmem[7] = 0xC3;

	char _mov_eax_1[] = { 0xB8, 0x01, 0x00, 0x00, 0x00 };

	uint64_t kernelBase = (uint64_t)KernelBase;
	uint64_t assistmode_patch_addrs[] = {
		kernelBase + 0x75CD84,
		kernelBase + 0x75CF68,
		kernelBase + 0x75E146,
		kernelBase + 0x75EE50,
		kernelBase + 0x75EE76,
		kernelBase + 0x75EE9C,
		kernelBase + 0x75EEC2,
		kernelBase + 0x75EEE8,
		kernelBase + 0x75EF0E,
		kernelBase + 0x75EF34,
		kernelBase + 0x75EF5A,
		kernelBase + 0x75EF80,
		kernelBase + 0x75EFA6,
		kernelBase + 0x75EFCC,
		kernelBase + 0x75EFF2,
		kernelBase + 0x75F018,
		kernelBase + 0x75F03E,
		kernelBase + 0x75F064,
		kernelBase + 0x75F09A,
		kernelBase + 0x75F0C0,
		kernelBase + 0x75F0E6,
		kernelBase + 0x75F10C,
	};

	for (int i = 0; i < sizeof(assistmode_patch_addrs) / sizeof(assistmode_patch_addrs[0]); i++)
	{
		memcpy((void*)assistmode_patch_addrs[i], _mov_eax_1, sizeof(_mov_eax_1));
	}

#elif defined(SOFTWARE_VERSION_1100)

	// Disable ASLR
	kmem = (uint8_t*)KernelBase + 0x3B11A4;
	kmem[0] = 0xEB;
	kmem[1] = 0x00;

	// swword_lwpid check to see if child_tid/parent_tid is in kernel memory
	kmem = (uint8_t*)KernelBase + 0x2DE302;
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	kmem = (uint8_t*)KernelBase + 0x2DE311;
	kmem[0] = 0x90;
	kmem[1] = 0x90;

	// ptrace Patches.
	kmem = (uint8_t*)KernelBase + 0x384285;
	kmem[0] = 0xEB;

	kmem = (uint8_t*)KernelBase + 0x384771;
	kmem[0] = 0xE9;
	kmem[1] = 0x7C;
	kmem[2] = 0x02;
	kmem[3] = 0x00;
	kmem[4] = 0x00;

	// sys_dynlib_get_info gives full module path.
	kmem = (uint8_t*)KernelBase + 0x1E597F;
	kmem[0] = 0x4C;
	kmem[1] = 0x89;
	kmem[2] = 0xF8;
	kmem[3] = 0x90;
	kmem[4] = 0x90;

	kmem = (uint8_t*)KernelBase + 0x1E598A;
	kmem[0] = 0x00;

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

	// flatz disable pfs signature check
	kmem = (uint8_t*)KernelBase + 0x684EB0;
	kmem[0] = 0x31;
	kmem[1] = 0xC0;
	kmem[2] = 0xC3;

	// flatz enable debug RIFs
	kmem = (uint8_t*)KernelBase + 0x64BFD0;
	kmem[0] = 0xB0;
	kmem[1] = 0x01;
	kmem[2] = 0xC3;

	kmem = (uint8_t*)KernelBase + 0x64C000;
	kmem[0] = 0xB0;
	kmem[1] = 0x01;
	kmem[2] = 0xC3;

	// Patch debug setting errors
	kmem = (uint8_t*)KernelBase + 0x4EE328;
	kmem[0] = 0x00;
	kmem[1] = 0x00;
	kmem[2] = 0x00;
	kmem[3] = 0x00;

	kmem = (uint8_t*)KernelBase + 0x4EF3EE;
	kmem[0] = 0x00;
	kmem[1] = 0x00;
	kmem[2] = 0x00;
	kmem[3] = 0x00;

	// // Enable *all* debugging logs (in vprintf)
	// // Patch by: SiSTRo
	// kmem = (uint8_t*)KernelBase + 0x2FCCC3;
	// kmem[0] = 0xEB;
	// kmem[1] = 0x3B;

	// Enable mount for unprivileged user
	kmem = (uint8_t*)KernelBase + 0x388B37;
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;
	kmem[3] = 0x90;
	kmem[4] = 0x90;
	kmem[5] = 0x90;

	// Patch sys_mmap: Allow RWX (read-write-execute) mapping
	kmem = (uint8_t*)KernelBase + 0x15626A;
	kmem[0] = 0x37;
	kmem[3] = 0x37;

	// Patch sys_dynlib_dlsym: Allow from anywhere
	kmem = (uint8_t*)KernelBase + 0x1E4CA8;
	kmem[0] = 0xE9;
	kmem[1] = 0x3A;
	kmem[2] = 0x01;
	kmem[3] = 0x00;
	kmem[4] = 0x00;

	kmem = (uint8_t*)KernelBase + 0x1E4C33;
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;
	kmem[3] = 0x90;
	kmem[4] = 0x90;
	kmem[5] = 0x90;

	kmem = (uint8_t*)KernelBase + 0x1E4C43;
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;
	kmem[3] = 0x90;
	kmem[4] = 0x90;
	kmem[5] = 0x90;

	kmem = (uint8_t*)KernelBase + 0x1E4C63;
	kmem[0] = 0x90;
	kmem[1] = 0xE9;

	kmem = (uint8_t*)KernelBase + 0x88CE0;
	kmem[0] = 0x48;
	kmem[1] = 0x31;
	kmem[2] = 0xC0;
	kmem[3] = 0xC3;

	// Patch Mprotect to allow changing to RWX
	kmem = (uint8_t*)KernelBase + 0x35C8EC;
	kmem[0] = 0x90;
	kmem[1] = 0x90;
	kmem[2] = 0x90;
	kmem[3] = 0x90;
	kmem[4] = 0x90;
	kmem[5] = 0x90;

	// Enable display_dump
	kmem = (uint8_t*)KernelBase + 0x442B5;
	kmem[0] = 0xEB;

#endif

    cpu_enable_wp();
}