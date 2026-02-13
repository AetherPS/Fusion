#include "common.h"
#include "kernel.h"

#include <kloader.h>

bool LoadKernel()
{
	// Check if Fusion Driver is already loaded.
	if (FileExist("/dev/Fusion"))
	{
		klog("Fusion already loaded.\n");
		return true;
	}

	// Decompress the Kernel.elf
	size_t decompressedSize;
	uint8_t* decompressedKernel = DecompressBlob(_binary_resources_Kernel_elf_compressed_start, &decompressedSize);

	if (!decompressedKernel)
	{
		klog("Failed to decompress Kernel.elf\n");
		return false;
	}

	kloader_load(decompressedKernel, decompressedSize);
	printf("-1\n");

	// Free the decompressed buffer
	free(decompressedKernel);

	return true;
}