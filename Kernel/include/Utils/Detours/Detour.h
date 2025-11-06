#pragma once

enum DetourType
{
	DetourType_Unknown,
	DetourType_Jump32,
	DetourType_Jump64,
	DetourType_Call32
};

class Detour
{
public:
	static void DetourCall(Detour** detourOut, void* address, void* destination);
	static void Detour32(Detour** detourOut, void* address, void* destination);
	static void Detour64(Detour** detourOut, void* address, void* destination);

	Detour(DetourType type, void* address, void* destination, void* stubAddress, void* trampolineAddress, uint8_t* originalBytes, size_t originalSize)
		: Type(type), Address(address), Destination(destination),
		  StubAddress(stubAddress), TrampolineAddress(trampolineAddress),
		  OriginalBytes(originalBytes), OriginalSize(originalSize), DetourSet(false)
	{
	}

	~Detour()
	{
		if (OriginalBytes != nullptr)
			_free(OriginalBytes);
	}

	template <typename result, typename... Args>
	result Invoke(Args... args)
	{
		result(*f)(Args... args) = decltype(f)(StubAddress);
		return f(args...);
	}

	void Enable();
	void Disable();

private:
	DetourType Type;
	int InstructionLength;
	void* Address;
	void* Destination;
	void* StubAddress;
	void* TrampolineAddress;
	uint8_t* OriginalBytes;
	size_t OriginalSize;
	bool DetourSet;

	static void* InitializeStub(int instructionLength, void* address);
	static Detour* CreateDetour(DetourType type, void* address, void* destination);
};