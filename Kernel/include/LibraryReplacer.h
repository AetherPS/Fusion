#pragma once

class LibraryReplacer
{
public:
	static void Init();
	static void Term();

private:
	static Detour32* load_prxDetour;
	static int load_prxHook(dynlib* dyn, char* path, int32_t flags, int32_t* handleOut);
	static int LoadIfExists(dynlib* dyn, char* path, int32_t flags, int32_t* handleOut);
};
