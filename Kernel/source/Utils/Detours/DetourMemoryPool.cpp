#include "Common.h"
#include "DetourMemoryPool.h"

void* DetourMemoryPool::PoolStart;
void* DetourMemoryPool::PoolCurrent;
size_t DetourMemoryPool::PoolSize;


/**
 * @brief Initializes the memory pool by allocating a large block of memory.
 * @param poolSize The total size (in bytes) of the memory pool to reserve.
 */
void DetourMemoryPool::Init(size_t poolSize)
{
	PoolSize = poolSize;

	kprintf("KmemAllocAt\n");
	PoolStart = KmemAllocAt(kernel_map, (vm_ooffset_t)KernelBase, PoolSize); //AllocateForMap(kernel_map, (vm_ooffset_t)KernelBase, PoolSize, VM_PROT_ALL, VM_PROT_ALL);
	if (PoolStart == nullptr) 
	{
		kprintf("ERROR: Failed to allocate %zu bytes for DetourMemoryPool.\n", PoolSize);
		PoolSize = 0;
		return;
	}

	kprintf("vm_map_protect\n");
	int res = vm_map_protect(kernel_map, (vm_offset_t)PoolStart, (vm_offset_t)PoolStart + PoolSize, VM_PROT_ALL, 0);
	if (res != 0)
	{
		kprintf("ERROR: vm_map_protect failed %llX.\n", res);

		kmem_free(kernel_map, PoolStart, PoolSize);

		return;
	}

	kprintf("DetourMemoryPool initialized with %zu bytes at address: 0x%llX.\n", PoolSize, PoolStart);

	PoolCurrent = PoolStart;
	memset((void*)PoolStart, 0, PoolSize);
}

/**
 * @brief Cleans up the memory pool, releasing the allocated block.
 */
void DetourMemoryPool::Term() {
	if (PoolStart)
	{
		kmem_free(kernel_map, PoolStart, PoolSize);

		PoolStart = 0;
		PoolCurrent = 0;
		kprintf("DetourMemoryPool shut down and memory released.\n");
	}
	PoolSize = 0;
}

/**
 * @brief Reserves a block of memory from the pool using linear (bump) allocation.
 * The returned memory is NOT initialized.
 * @param size The size (in bytes) of the memory block to reserve.
 * @return The starting address of the reserved block, cast to a 64-bit unsigned integer.
 * Returns 0 if allocation fails (pool exhausted or not initialized).
 */
void* DetourMemoryPool::ReserveMemory(size_t size)
{
    if (!PoolStart) {
        kprintf("WARNING: DetourMemoryPool not initialized.\n");
        return 0;
    }

    if (getRemainingSize() < size) {
        kprintf("WARNING: DetourMemoryPool exhausted. Requested %zu bytes, only %zu remaining.\n", size, getRemainingSize());
        return 0;
    }

	auto allocSpace = PoolCurrent;
	PoolCurrent += size;

    return allocSpace;
}