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

	// calculate the necessary instructions size to detour
	auto instructionLength = 0;
	while (instructionLength < JUMP_32SIZE)
	{
		auto ptr = reinterpret_cast<uint64_t>(address) + instructionLength;

		hde64s hs;
		instructionLength += hde64_disasm(reinterpret_cast<void*>(ptr), &hs);

		if (hs.flags & F_ERROR)
		{
			kprintf("[Detour32] HDE has run into an error dissasembling %llX!\n", ptr);
			return;
		}
	}

#ifdef DEBUG
	kprintf("[Detour32] create: instructionLength = %i\n", instructionLength);
#endif

	Address = address;

	// save the original instructions
	OriginalSize = instructionLength;
	OriginalBytes = (uint8_t*)_malloc(OriginalSize);
	memcpy(OriginalBytes, address, OriginalSize);

#ifdef DEBUG
	kprintf("[Detour32] create: Allocate trampoline memory.\n");
#endif

	// create the trampoline and jump to it
	TrampolineSize = (JUMP_64SIZE + 0x3FFFull) & ~0x3FFFull;
	TrampolinePtr = KmemAllocAt(kernel_map, (vm_ooffset_t)KernelBase, TrampolineSize);
	if (TrampolinePtr == nullptr)
	{
		kprintf("[Detour32] create: an error has occurred allocating trampoline: %llX.\n", TrampolinePtr);
		return;
	}

#ifdef DEBUG
	kprintf("[Detour32] create: KernelBase: %llX, %llX %llX.\n", KernelBase, TrampolinePtr, TrampolineSize);
#endif

	// Make sure we can make the jump.
	if (!IsRelativelyClose((void*)((uint64_t)address + JUMP_32SIZE), TrampolinePtr))
	{
		kprintf("[Detour32] create: Trampoline is not relatively close we can not write a jump 32.\n");

		kmem_free(kernel_map, TrampolinePtr, TrampolineSize);
		return;
	}

	// Set the protection for the trampoline.
	int res = vm_map_protect(kernel_map, (vm_offset_t)TrampolinePtr, (vm_offset_t)TrampolinePtr + TrampolineSize, VM_PROT_ALL, 0);
	if (res != 0)
	{
		kprintf("[Detour32] create: vm_map_protect failed %llX.\n", res);

		kmem_free(kernel_map, TrampolinePtr, TrampolineSize);

		return;
	}

#ifdef DEBUG
	kprintf("[Detour32] create: Allocate stub memory.\n");
#endif

	// allocate the stub
	StubSize = (instructionLength + JUMP_64SIZE + 0x3FFFull) & ~0x3FFFull;
	StubPtr = (void*)kmem_alloc(kernel_map, StubSize);
	if (StubPtr == nullptr)
	{
		kmem_free(kernel_map, TrampolinePtr, TrampolineSize);

		kprintf("[Detour32] create: alloc failed.\n");
		return;
	}

	// Write the stub protection.
	vm_map_protect(kernel_map, (vm_offset_t)StubPtr, (vm_offset_t)StubPtr + StubSize, VM_PROT_ALL, 0);

	// Write original instructions to the stub.
	memcpy(StubPtr, Address, instructionLength);

	// Write the jump back to main function to the stub.
	auto stubJump = reinterpret_cast<uint64_t>(StubPtr) + instructionLength;
	auto stubReturn = reinterpret_cast<uint64_t>(Address) + instructionLength;

#ifdef DEBUG
	kprintf("[Detour32] create: writing trampoline jumps.\n");
#endif

	WriteJump64(reinterpret_cast<void*>(stubJump), reinterpret_cast<void*>(stubReturn));
	WriteJump64(TrampolinePtr, destination);

#ifdef DEBUG
	kprintf("[Detour32] create: writing detour jump.\n");
#endif

	WriteJump32(address, TrampolinePtr);

#ifdef DEBUG
	kprintf("[Detour32] Detour written from 0x%llX -> 0x%llX\n", address, destination);
#endif

	DetourSet = true;
}

void Detour32::Restore()
{
	if (DetourSet)
	{
#ifdef DEBUG
		kprintf("[Detour32] Restoring original function bytes.\n");
#endif

		MemcpyTextSeg((void*)Address, OriginalBytes, OriginalSize);

		kmem_free(kernel_map, TrampolinePtr, TrampolineSize);
		kmem_free(kernel_map, StubPtr, StubSize);

#ifdef DEBUG
		kprintf("[Detour32] Detour Removed.\n");
#endif

		DetourSet = false;
	}
	else
	{
		kprintf("[Detour32] restore: cannot restore detour that was not set!\n");
	}
}