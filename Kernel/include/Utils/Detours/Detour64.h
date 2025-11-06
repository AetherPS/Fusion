#pragma once

class Detour64 : public BaseDetour
{
public:
	template<class T>
	Detour64(void* address, T(*function))
	{
		Create(address, reinterpret_cast<void*>(function));
	}

	template<class T>
	Detour64(Detour64** inst, void* address, T(*function))
	{
		*inst = this;
		Create(address, reinterpret_cast<void*>(function));
	}

	template<class T>
	Detour64(uint64_t address, T(*function))
	{
		Create(reinterpret_cast<void*>(address), reinterpret_cast<void*>(function));
	}

	template<class T>
	Detour64(Detour64** inst, uint64_t address, T(*function))
	{
		*inst = this;
		Create(reinterpret_cast<void*>(address), reinterpret_cast<void*>(function));
	}

	~Detour64() { Disable(); }

	void Create(void* address, void* destination);
};