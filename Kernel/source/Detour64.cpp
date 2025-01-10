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

	// calculate the necessary instructions size to detour
	auto instructionLength = 0;
	while (instructionLength < JUMP_64SIZE)
	{
		auto ptr = reinterpret_cast<uint64_t>(address) + instructionLength;

		hde64s hs;
		instructionLength += hde64_disasm(reinterpret_cast<void*>(ptr), &hs);

		if (hs.flags & F_ERROR)
		{
			kprintf("[Detour64] HDE has run into an error dissasembling %llX!\n", ptr);
			return;
		}
	}

	kprintf("[Detour64] create: instructionLength = %i\n", instructionLength);

	Address = address;

	// save the original instructions
	OriginalSize = instructionLength;
	OriginalBytes = (uint8_t*)_malloc(OriginalSize);
	memcpy(OriginalBytes, address, OriginalSize);

	// allocate the stub
	StubSize = (instructionLength + JUMP_64SIZE + 0x3FFFull) & ~0x3FFFull;
	StubPtr = (void*)kmem_alloc(kernel_map, StubSize);
	if (StubPtr == nullptr)
	{
		kprintf("[Detour64] create: alloc failed\n");
		return;
	}

	// Write the stub protection.
	vm_map_protect(kernel_map, (vm_offset_t)StubPtr, (vm_offset_t)StubPtr + StubSize, VM_PROT_ALL, 0);

	// write original instructions to the stub
	memcpy(StubPtr, Address, instructionLength);

	// write the jump back to main function to the stub
	auto stub_jump = reinterpret_cast<uint64_t>(StubPtr) + instructionLength;
	auto stub_return = reinterpret_cast<uint64_t>(Address) + instructionLength;

	kprintf("[Detour64] create: writing detour jumps\n");

	WriteJump64(reinterpret_cast<void*>(stub_jump), reinterpret_cast<void*>(stub_return));
	WriteJump64(Address, destination);

	kprintf("[Detour64] Detour written from 0x%llX -> 0x%llX\n", address, destination);
	DetourSet = true;
}

void Detour64::Restore()
{
	if (DetourSet)
	{
		kprintf("[Detour64] Restoring original function bytes.\n");
		MemcpyTextSeg((void*)Address, OriginalBytes, OriginalSize);

		kmem_free(kernel_map, StubPtr, StubSize);

		StubPtr = nullptr;
		StubSize = 0;
		OriginalSize = 0;
		Address = 0;

		kprintf("[Detour64] Detour Removed.\n");
		DetourSet = false;
	}
	else
	{
		kprintf("[Detour64] restore: cannot restore detour that was not set!\n");
	}
}