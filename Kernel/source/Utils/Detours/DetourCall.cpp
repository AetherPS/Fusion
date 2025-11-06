#include "Common.h"
#include "BaseDetour.h"
#include "DetourCall.h"

void DetourCall::Create(void* address, void* destination)
{
	if (address == 0 || destination == nullptr)
	{
		kprintf("[DetourCall] create: \"address\" or \"destination\" NULL (%llX -> %llX)\n", address, destination);
		return;
	}

	if (*(uint8_t*)(address) != 0xE8)
	{
		kprintf("[DetourCall] Create: Instruction not a call instruction. The Call Detour is to be used to replace a relative call.\n");
		return;
	}

	Address = address;
	StubAddress = (void*)((uint64_t)Address + CALL_32SIZE + *(int32_t*)((uint64_t)Address + 1));
	Destination = destination;
	SaveOriginalBytes(JUMP_32SIZE);

	if (!AllocateTrampoline())
	{
		return;
	}
	
	WriteJump64(TrampolineAddress, destination);
	WriteCall32(address, TrampolineAddress);
	
#ifdef DEBUG
	kprintf("[DetourCall] Detour written from 0x%llX -> 0x%llX\n", address, destination);
#endif

	DetourSet = true;
}