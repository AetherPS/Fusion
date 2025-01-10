#pragma once

class DetourCall : public BaseDetour
{
public:
	template<class T>
	DetourCall(void* address, T(*function))
	{
		Create(address, reinterpret_cast<void*>(function));
	}

	template<class T>
	DetourCall(DetourCall** inst, void* address, T(*function))
	{
		*inst = this;
		Create(address, reinterpret_cast<void*>(function));
	}

	template<class T>
	DetourCall(uint64_t address, T(*function))
	{
		Create(reinterpret_cast<void*>(address), reinterpret_cast<void*>(function));
	}

	template<class T>
	DetourCall(DetourCall** inst, uint64_t address, T(*function))
	{
		*inst = this;
		Create(reinterpret_cast<void*>(address), reinterpret_cast<void*>(function));
	}

	~DetourCall() { Restore(); }

	void Create(void* address, void* destination);
	void Restore();

private:
	size_t TrampolineSize;
	void* TrampolinePtr;
};