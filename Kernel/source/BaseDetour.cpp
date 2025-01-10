#include "Common.h"
#include "BaseDetour.h"

#ifndef INT32_MIN
#define INT32_MIN (-2147483647 - 1)
#endif

#ifndef INT32_MAX
#define INT32_MAX 2147483647
#endif

bool BaseDetour::IsRelativelyClose(void* address1, void* address2)
{
	const uintptr_t addr1 = reinterpret_cast<uintptr_t>(address1);
	const uintptr_t addr2 = reinterpret_cast<uintptr_t>(address2);
	const int64_t diff = static_cast<int64_t>(addr1 - addr2);
	return diff >= INT32_MIN && diff <= INT32_MAX;
}

uintptr_t BaseDetour::GetRelativeAddress(void* address, void* destination)
{
	return size_t(destination) - (size_t(address) + JUMP_32SIZE);
}

void BaseDetour::WriteJump32(void* address, void* destination)
{
	uint8_t jmp_bytes[JUMP_32SIZE] = { 0xE9, 0x90, 0x90, 0x90, 0x90 };
	*reinterpret_cast<uint32_t*>(jmp_bytes + 1) = GetRelativeAddress(address, destination);

	MemcpyTextSeg(address, jmp_bytes, sizeof(jmp_bytes));
}

void BaseDetour::WriteJump64(void* address, void* destination)
{
	uint8_t jmp_bytes[JUMP_64SIZE] = { 0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
	*reinterpret_cast<uint64_t*>(jmp_bytes + 6) = reinterpret_cast<uintptr_t>(destination);

	MemcpyTextSeg(address, jmp_bytes, sizeof(jmp_bytes));
}

void BaseDetour::WriteCall32(void* address, void* destination)
{
	uint8_t call_bytes[CALL_32SIZE] = { 0xE8, 0x90, 0x90, 0x90, 0x90 };
	*reinterpret_cast<uint32_t*>(call_bytes + 1) = GetRelativeAddress(address, destination);

	MemcpyTextSeg(address, call_bytes, sizeof(call_bytes));
}

void* memoryReserveAddress = nullptr;
size_t memoryReserveSize = PAGE_SIZE;
size_t currentMemoryOffset = 0;

void* BaseDetour::ReserveMemory(size_t ammount)
{
	if (memoryReserveAddress == nullptr)
	{
		memoryReserveAddress = AllocateForMap(kernel_map, (vm_ooffset_t)KernelBase, memoryReserveSize, VM_PROT_ALL, VM_PROT_ALL);
		currentMemoryOffset = 0;

		if (memoryReserveAddress == nullptr)
		{
			kprintf("Failed to allocate reserve memory.\n");
			return nullptr;
		}
	}

	if (currentMemoryOffset + ammount >= memoryReserveSize)
	{
		kprintf("Reserve pool is empty.\n");
		return nullptr;
	}
	
	currentMemoryOffset += ammount;

	return (void*)((uint64_t)memoryReserveAddress + (currentMemoryOffset - ammount));
}