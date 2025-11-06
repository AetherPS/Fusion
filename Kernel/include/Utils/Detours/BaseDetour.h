#pragma once
#define JUMP_32SIZE 5
#define JUMP_64SIZE 14
#define CALL_32SIZE JUMP_32SIZE

class BaseDetour
{
public:
	template <typename result, typename... Args>
	result Invoke(Args... args)
	{
		result(*f)(Args... args) = decltype(f)(StubAddress);
		return f(args...);
	}

	inline ~BaseDetour()
	{
		_free(OriginalBytes);
	}

	void Enable();
	void Disable();

protected:
	bool IsRelativelyClose(void* address1, void* address2);
	uintptr_t GetRelativeAddress(void* address, void* destination);
	void WriteJump32(void* address, void* destination);
	void WriteJump64(void* address, void* destination);
	void WriteCall32(void* address, void* destination);
	int GetInstructionSize();
	void SaveOriginalBytes(size_t size);
	bool AllocateTrampoline();
	bool InitializeStub(int instructionLength, void* from, void* to);

	void (*WriteMethod)(void*, void*);
	void* StubAddress;
	void* Address;
	void* Destination;
	void* TrampolineAddress;
	uint8_t* OriginalBytes;
	size_t OriginalSize;
	bool DetourSet;
};