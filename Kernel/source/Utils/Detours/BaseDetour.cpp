#include "Common.h"
#include "BaseDetour.h"

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

int BaseDetour::GetInstructionSize()
{
	int instructionLength = 0;
	while (instructionLength < JUMP_32SIZE)
	{
		auto ptr = reinterpret_cast<uint64_t>(Address) + instructionLength;

		hde64s hs;
		instructionLength += hde64_disasm(reinterpret_cast<void*>(ptr), &hs);

		if (hs.flags & F_ERROR)
		{
			kprintf("[Detour32] HDE has run into an error dissasembling %llX!\n", ptr);
			return -1;
		}
	}

	return instructionLength;
}

void BaseDetour::SaveOriginalBytes(size_t size)
{
	OriginalBytes = (uint8_t*)_malloc(size);
	memcpy(OriginalBytes, Address, size);
}

bool BaseDetour::AllocateTrampoline()
{
	TrampolineAddress = (void*)DetourMemoryPool::ReserveMemory(JUMP_64SIZE);
	if (TrampolineAddress == nullptr)
	{
		kprintf("[BaseDetour] AllocateTrampoline: an error has occurred allocating trampoline: %llX.\n", TrampolineAddress);
		return false;
	}

	// Make sure we can make the jump.
	if (!IsRelativelyClose((void*)((uint64_t)Address + JUMP_32SIZE), TrampolineAddress))
	{
		kprintf("[DetourCall] create: Trampoline is not relatively close we can not write a jump 32.\n");
		return false;
	}

	return true;
}

bool BaseDetour::InitializeStub(int instructionLength, void* from, void* to)
{
	StubAddress = (void*)DetourMemoryPool::ReserveMemory(instructionLength + JUMP_64SIZE);
	if (StubAddress == nullptr)
	{
		kprintf("[Detour32] create: DetourMemoryPool::ReserveMemory failed.\n");
		return false;
	}

	// write original instructions to the stub
	memcpy(StubAddress, Address, instructionLength);

	// write the jump back to main function to the stub
	auto stub_jump = reinterpret_cast<uint64_t>(StubAddress) + instructionLength;
	auto stub_return = reinterpret_cast<uint64_t>(Address) + instructionLength;

	WriteJump64(reinterpret_cast<void*>(stub_jump), reinterpret_cast<void*>(stub_return));
	WriteJump64(from, to);
}

void BaseDetour::Enable()
{
	if (DetourSet)
	{
		WriteMethod(Address, Destination);
		DetourSet = true;
	}
}

void BaseDetour::Disable()
{
	if (DetourSet)
	{
		MemcpyTextSeg((void*)Address, OriginalBytes, OriginalSize);
		DetourSet = false;
	}
}