#include "Common.h"
#include "LibraryReplacer.h"

#define DEBUG

Detour32* LibraryReplacer::load_prxDetour;

int LibraryReplacer::load_prxHook(dynlib* dyn, char* path, int32_t flags, int32_t* handleOut)
{
	auto td = CurrentThread();

	// Safety Checks.
	if (!td || !td->td_proc)
		return load_prxDetour->Invoke<int>(dyn, path, flags, handleOut);

	auto processName = td->td_proc->p_comm;
	auto titleId = td->td_proc->titleId;
	auto fileName = FileNameFromPath(path);

#ifdef DEBUG
	kprintf("load_prx: %s %d, %s\n", titleId, td->td_proc->p_pid, fileName);
#endif

	// Get the path of the title specific replacement library.
	char prxReplacePath[0x200];
	sprintf(prxReplacePath, "/data/Fusion/ReplacementLibs/%s/%s", titleId, fileName);

	// Get the path of the any process replacement library.
	char prxReplacePathAll[0x200];
	sprintf(prxReplacePathAll, "/data/Fusion/ReplacementLibs/Any/%s", fileName);

	// Load the replaced library if it exists.
	auto res = LoadIfExists(dyn, prxReplacePath, flags, handleOut);

	if (res == 0)
	{
		kprintf("Replaced \"%s\" on %s(%d)", fileName, titleId, td->td_proc->p_pid);
		return 0;
	}
	
	res = LoadIfExists(dyn, prxReplacePathAll, flags, handleOut);

	if (res == 0)
	{
		kprintf("Replaced \"%s\" on %s(%d)", fileName, titleId, td->td_proc->p_pid);
		return 0;
	}

	// All others just go as normal.
	return load_prxDetour->Invoke<int>(dyn, path, flags, handleOut);
}

int LibraryReplacer::LoadIfExists(dynlib* dyn, char* path, int32_t flags, int32_t* handleOut)
{
	if (!fileExists(path))
		return -1;

	int handleOutInternal = 0;
	auto res = load_prxDetour->Invoke<int>(dyn, path, flags, &handleOutInternal);

	if (handleOut != nullptr)
	{
		*handleOut = handleOutInternal;
	}

	if (handleOutInternal < 0)
		return -1;

	return res;
}

void LibraryReplacer::Init()
{
	kern_mkdir(CurrentThread(), "/data/Fusion", 0, 0777);
	kern_mkdir(CurrentThread(), "/data/Fusion/ReplacementLibs", 0, 0777);
	kern_mkdir(CurrentThread(), "/data/Fusion/ReplacementLibs/Any", 0, 0777);

	load_prxDetour = new Detour32(&load_prxDetour, KernelBase + addr_load_prx, load_prxHook);
}

void LibraryReplacer::Term()
{
	delete load_prxDetour;
}