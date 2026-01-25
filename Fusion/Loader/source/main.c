#include "common.h"
#include "daemon.h"

extern uint8_t _binary_resources_Kernel_elf_compressed_start[];
extern uint8_t _binary_resources_Kernel_elf_compressed_end[];

int LoadKernel()
{
	// Decompress the Kernel.elf
	size_t decompressedSize;
	uint8_t* decompressedKernel = DecompressBlob(_binary_resources_Kernel_elf_compressed_start, &decompressedSize);

	if (!decompressedKernel)
	{
		klog("Failed to decompress Kernel.elf\n");
		return 1;
	}

	kloader_load(decompressedKernel, decompressedSize);

	// Free the decompressed buffer
	free(decompressedKernel);

	return 0;
}

int _main(void)
{
	if (ResolveDynlib() != 0)
	{
		klog("[Fusion] Cant load dynlib look up failed.\n");
		return 1;
	}

	ascii();

	// Check if Fusion Driver is already loaded.
	if (FileExist("/dev/Fusion"))
	{
		klog("Fusion already loaded.\n");
		return 1;
	}

	// Decompress and load the kernel module.
	if (LoadKernel() != 0)
	{
		klog("Failed to load the kernel module.\n");
		return 1;
	}

	LoadSprx("SceShellCore", "/data/Fusion/Plugins/ShellCore.sprx");
	LoadSprx("SceSysCore", "/data/Fusion/Plugins/SysCore.sprx");
	//LoadSprx("SceShellUI", "/user/data/Fusion/Plugins/ShellUI.sprx"); // TODO AOT verison.

	return 0;
}