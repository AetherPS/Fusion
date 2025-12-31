#include "Common.h"
#include "Detour.h"

void Detour::DetourCall(Detour** detourOut, void* address, void* destination)
{
	*detourOut = CreateDetour(DetourType_Call32, address, destination);
	(*detourOut)->Enable();
}

void Detour::Detour32(Detour** detourOut, void* address, void* destination)
{
	*detourOut = CreateDetour(DetourType_Jump32, address, destination);
	(*detourOut)->Enable();
}

void Detour::Detour64(Detour** detourOut, void* address, void* destination)
{
	*detourOut = CreateDetour(DetourType_Jump64, address, destination);
	(*detourOut)->Enable();
}

Detour* Detour::CreateDetour(DetourType type, void* address, void* destination)
{
	int instructionLength = 0;
	while (instructionLength < ((type == DetourType_Jump64) ? JUMP_64SIZE : JUMP_32SIZE))
	{
		auto ptr = reinterpret_cast<uint64_t>(address) + instructionLength;

		hde64s hs;
		instructionLength += hde64_disasm(reinterpret_cast<void*>(ptr), &hs);

		if (hs.flags & F_ERROR)
		{
			printf("[Detour] HDE has run into an error dissasembling %llX!\n", ptr);
			return nullptr;
		}
	}

#ifdef DEBUG
	kprintf("[Detour] InstructionLength = %i\n", instructionLength);
#endif

	auto originalBytes = (uint8_t*)_malloc(instructionLength);
	memcpy(originalBytes, address, instructionLength);

	void* trampolineAddress = nullptr;
	if (type != DetourType_Jump64)
	{
		trampolineAddress = DetourMemoryPool::ReserveMemory(JUMP_64SIZE);
		if (trampolineAddress == nullptr)
		{
			printf("[Detour] An error has occurred allocating trampoline: %llX.\n", trampolineAddress);
			return nullptr;
		}

		// Make sure we can make the jump.
		if (!IsRelativelyClose((void*)((uint64_t)address + JUMP_32SIZE), trampolineAddress))
		{
			printf("[Detour] Trampoline is not relatively close we can not write a jump 32.\n");
			return nullptr;
		}

		WriteJump64(trampolineAddress, destination);
	}

	void* stubAddress = nullptr;
	switch (type)
	{
	case DetourType_Call32:
		stubAddress = (void*)((uint64_t)address + CALL_32SIZE + *(int32_t*)((uint64_t)address + 1));
		break;

	case DetourType_Jump32:
	case DetourType_Jump64:
		stubAddress = DetourMemoryPool::ReserveMemory(instructionLength + JUMP_64SIZE);
		if (stubAddress == nullptr)
		{
			printf("[Detour] DetourMemoryPool::ReserveMemory failed.\n");
			return nullptr;
		}

		// write original instructions to the stub
		memcpy(stubAddress, address, instructionLength);

		// write the jump back to main function to the stub
		auto stubFrom = reinterpret_cast<uint64_t>(stubAddress) + instructionLength;
		auto stubTo = reinterpret_cast<uint64_t>(address) + instructionLength;

		WriteJump64(reinterpret_cast<void*>(stubFrom), reinterpret_cast<void*>(stubTo));
		break;
	}

	return new Detour(type, address, destination, stubAddress, trampolineAddress, originalBytes, instructionLength);
}

void Detour::Enable()
{
	if (!DetourSet)
	{
		switch (Type)
		{
		case DetourType_Jump32:
			WriteJump32(Address, TrampolineAddress);
			break;
		case DetourType_Jump64:
			WriteJump64(Address, Destination);
			break;
		case DetourType_Call32:
			WriteCall32(Address, TrampolineAddress);
			break;
		}
		DetourSet = true;
	}
}

void Detour::Disable()
{
	if (DetourSet)
	{
		MemcpyTextSeg((void*)Address, OriginalBytes, OriginalSize);
		DetourSet = false;
	}
}