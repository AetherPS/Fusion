#include "common.h"
#include "kernel.h"
#include "config.h"

#include <kloader.h>

bool LoadKernel()
{
	// Check if Fusion is already loaded using marker file
	// (Don't check /dev/Fusion as Driver feature can be disabled)
	if (IsAlreadyLoaded())
	{
		klog("Fusion kernel already loaded.\n");
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