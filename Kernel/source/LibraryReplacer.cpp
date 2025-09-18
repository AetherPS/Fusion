#include "Common.h"
#include "LibraryReplacer.h"

int (*sys_dynlib_load_prx)(thread* td, dynlib_load_prx_args* args) = nullptr;

int dynlib_load_prx(thread* td, const char* prxPath, int flags, int* handleOut)
{
	dynlib_load_prx_args args;
	args.prx_path = prxPath;
	args.flags = flags;
	args.handle_out = handleOut;
	args.unk = 0;

	return sys_dynlib_load_prx(td, &args);
}

int LibraryReplacer::sys_dynlib_load_prxHook(thread* td, dynlib_load_prx_args* args)
{
	auto proc = td->td_proc;

	char path[1024];
	copyin(args->prx_path, path, 1024);

	auto processName = td->td_proc->p_comm;
	auto titleId = td->td_proc->titleId;
	auto fileName = FileNameFromPath(path);

#ifdef DEBUG
	kprintf("sys_dynlib_load_prx: %s %d, %s\n", titleId, td->td_proc->p_pid, fileName);
#endif

	// Get the path of the title specific replacement library.
	char prxReplacePath[0x200];
	sprintf(prxReplacePath, "/data/Fusion/ReplacementLibs/%s/%s", titleId, fileName);

	// Get the path of the any process replacement library.
	char prxReplacePathAll[0x200];
	sprintf(prxReplacePathAll, "/data/Fusion/ReplacementLibs/Any/%s", fileName);

	// Load the replaced library if it exists.
	int loadedHandleOut = 0;
	if (!dynlib_load_prx(td, prxReplacePath, args->flags, &loadedHandleOut) || !dynlib_load_prx(td, prxReplacePathAll, args->flags, &loadedHandleOut))
	{
		copyout(&loadedHandleOut, args->handle_out, sizeof(int));

		kprintf("Replaced \"%s\" on %s(%d)\n", fileName, titleId, td->td_proc->p_pid);
		return 0;
	}

	// All others just go as normal.
	return sys_dynlib_load_prx(td, args);
}

void LibraryReplacer::Init()
{
	kern_mkdir(CurrentThread(), "/data/Fusion", 0, 0777);
	kern_mkdir(CurrentThread(), "/data/Fusion/ReplacementLibs", 0, 0777);
	kern_mkdir(CurrentThread(), "/data/Fusion/ReplacementLibs/Any", 0, 0777);

	sys_dynlib_load_prx = decltype(sys_dynlib_load_prx)((void*)sysvec->sv_table[594].sy_call);
	sysvec->sv_table[594].sy_call = (sy_call_t*)sys_dynlib_load_prxHook;
}

void LibraryReplacer::Term()
{
	sysvec->sv_table[594].sy_call = (sy_call_t*)sys_dynlib_load_prx;
}