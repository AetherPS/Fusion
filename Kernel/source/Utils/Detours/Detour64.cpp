#include "Common.h"
#include "BaseDetour.h"
#include "Detour64.h"

void Detour64::Create(void* address, void* destination)
{
	if (address == 0 || destination == nullptr)
	{
		kprintf("[Detour64] create: \"address\" or \"destination\" NULL (%llX -> %llX)\n", address, destination);
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
	kprintf("[Detour64] create: instructionLength = %i\n", instructionLength);
#endif

	SaveOriginalBytes(instructionLength);

	if(!InitializeStub(instructionLength, Address, Destination))
	{
		return;
	}

	DetourSet = true;
}
