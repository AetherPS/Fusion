#include "common.h"
#include "kernel.h"

#include <kloader.h>

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