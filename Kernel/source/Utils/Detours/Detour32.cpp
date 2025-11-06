#include "Common.h"
#include "BaseDetour.h"
#include "Detour32.h"

void Detour32::Create(void* address, void* destination)
{
	if (address == 0 || destination == nullptr)
	{
		kprintf("[Detour32] create: \"address\" or \"destination\" NULL (%llX -> %llX)\n", address, destination);
		return;
	}

	Address = address;
	Destination = destination;

	// calculate the necessary instructions size to detour
	auto instructionLength = GetInstructionSize();
	if (instructionLength < 0)
	{
		return;
	}

#ifdef DEBUG
	kprintf("[Detour32] create: instructionLength = %i\n", instructionLength);
#endif

	SaveOriginalBytes(instructionLength);

	if (!AllocateTrampoline())
	{
		return;
	}

	if (!InitializeStub(instructionLength, TrampolineAddress, Destination))
	{
		return;
	}

	WriteJump32(address, TrampolineAddress);

#ifdef DEBUG
	kprintf("[Detour32] Detour written from 0x%llX -> 0x%llX\n", address, destination);
#endif

	DetourSet = true;
}
