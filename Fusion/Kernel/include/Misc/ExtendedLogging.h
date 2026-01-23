#pragma once

class ExtendedLogging
{
public:
	static void Init();
	static void Term();

private:
	static Detour* CallStackPrintfDetour;
	static void CallStackPrintfHook(void* a1, void* a2, const char* fmt, uint64_t address);
	static Detour* FaultAddressPrintfDetour;
	static void FaultAddressPrintfHook(void* a1, void* a2, const char* fmt, uint64_t address);
};