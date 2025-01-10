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

	// Save the original bytes.
	OriginalBytes = (uint8_t*)_malloc(JUMP_32SIZE);
	memcpy(OriginalBytes, address, JUMP_32SIZE);

	// Save the original function call.
	StubPtr = (void*)((uint64_t)Address + CALL_32SIZE + *(int32_t*)((uint64_t)Address + 1));

	kprintf("[DetourCall] create: Original absolute location -> %llX\n", (uint64_t)StubPtr - (uint64_t)KernelBase);

	// Create the trampoline and call it
	TrampolineSize = ((uint64_t)JUMP_64SIZE + 0x3FFFull) & ~0x3FFFull;
	TrampolinePtr = KmemAllocAt(kernel_map, (vm_ooffset_t)KernelBase, TrampolineSize);
	if (TrampolinePtr == nullptr)
	{
		kprintf("[DetourCall] create: an error has occurred allocating trampoline: %llX\n", TrampolinePtr);
		return;
	}

	// Make sure we can make the jump.
	if (!IsRelativelyClose((void*)((uint64_t)address + JUMP_32SIZE), TrampolinePtr))
	{
		kprintf("[DetourCall] create: Trampoline is not relatively close we can not write a jump 32.\n");

		kmem_free(kernel_map, TrampolinePtr, TrampolineSize);
		return;
	}

	// Set the protection for the trampoline.
	vm_map_protect(kernel_map, (vm_offset_t)TrampolinePtr, (vm_offset_t)TrampolinePtr + TrampolineSize, VM_PROT_ALL, 0);
	
	kprintf("[DetourCall] create: writing trampoline jumps\n");
	
	WriteJump64(TrampolinePtr, destination);
	WriteCall32(address, TrampolinePtr);
	
	kprintf("[DetourCall] Detour written from 0x%llX -> 0x%llX\n", address, destination);
	DetourSet = true;
}

void DetourCall::Restore()
{
	if (DetourSet)
	{
		kprintf("[DetourCall] Restoring original function bytes.\n");
		MemcpyTextSeg((void*)Address, OriginalBytes, OriginalSize);

		kmem_free(kernel_map, TrampolinePtr, TrampolineSize);

		kprintf("[DetourCall] Detour Removed.\n");
		DetourSet = false;
	}
	else
	{
		kprintf("[DetourCall] restore: cannot restore detour that was not set!\n");
	}
}