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

	PoolStart = KmemAllocAt(kernel_map, (vm_ooffset_t)KernelBase, PoolSize);
	if (PoolStart == nullptr) 
	{
		kprintf("ERROR: Failed to allocate %zu bytes for DetourMemoryPool.\n", PoolSize);
		PoolSize = 0;
		return;
	}
#ifdef DEBUG
	kprintf("DetourMemoryPool initialized with %zu bytes at address: 0x%llX.\n", PoolSize, PoolStart);
#endif

	PoolCurrent = PoolStart;
	memset(PoolStart, 0, PoolSize);
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