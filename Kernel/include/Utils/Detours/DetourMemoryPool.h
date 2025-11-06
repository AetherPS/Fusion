#pragma once

class DetourMemoryPool
{
private:
	static void* PoolStart;
	static void* PoolCurrent;
	static size_t PoolSize;

	static size_t getRemainingSize() 
	{
		if (!PoolStart) return 0;

		return PoolSize - ((uint64_t)PoolCurrent - (uint64_t)PoolStart);
	}

public:
	static void Init(size_t poolSize);
	static void Term();
	static void* ReserveMemory(size_t size);
};