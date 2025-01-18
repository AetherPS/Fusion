#include "Common.h"
#include "LibraryReplacer.h"

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

	//// Get the path of the title specific replacement library.
	//char prxReplacePath[0x200];
	//sprintf(prxReplacePath, "/data/Fusion/ReplacementLibs/%s/%s", titleId, fileName);
	//
	//// Get the path of the any process replacement library.
	//char prxReplacePathAll[0x200];
	//sprintf(prxReplacePathAll, "/data/Fusion/ReplacementLibs/Any/%s", fileName);
	//
	//// Load the replaced library if it exists.
	//auto res = LoadIfExists(dyn, prxReplacePath, flags, handleOut);
	//
	//if (res == 0)
	//{
	//	kprintf("Replaced \"%s\" on %s(%d)", fileName, titleId, td->td_proc->p_pid);
	//	return 0;
	//}
	//
	//res = LoadIfExists(dyn, prxReplacePathAll, flags, handleOut);
	//
	//if (res == 0)
	//{
	//	kprintf("Replaced \"%s\" on %s(%d)", fileName, titleId, td->td_proc->p_pid);
	//	return 0;
	//}

	// All others just go as normal.
	auto res = load_prxDetour->Invoke<int>(dyn, path, flags, handleOut);

	if (handleOut != nullptr && handleOut > 0)
	{
		struct dynlib_info_ex info;
		info.size = sizeof(struct dynlib_info_ex);
		sys_dynlib_get_info_ex(td, *handleOut, 1, &info);

		uint8_t mov__eax_1__ret[6] = { 0xB8, 0x01, 0x00, 0x00, 0x00, 0xC3 };

		// if (strstr(info.name, "libScePsm.sprx"))
		// {
		// 	auto libScePsmBase = (uint64_t)info.segmentInfo[0].baseAddr;
		// 
		// 	// sceKernelGetPsmIntdevModeForRcmgr
		// 	ReadWriteProcessMemory(td, td->td_proc, (void*)(libScePsmBase + 0x512C0), mov__eax_1__ret, sizeof(mov__eax_1__ret), true);
		// 	ReadWriteProcessMemory(td, td->td_proc, (void*)(libScePsmBase + 0x1C68), mov__eax_1__ret, sizeof(mov__eax_1__ret), true);
		// 
		// 	// machdep.check_genuine_devkit_for_psm
		// 	ReadWriteProcessMemory(td, td->td_proc, (void*)(libScePsmBase + 0x512D0), mov__eax_1__ret, sizeof(mov__eax_1__ret), true);
		// 
		// 	// sceKernelIsDevelopmentMode
		// 	ReadWriteProcessMemory(td, td->td_proc, (void*)(libScePsmBase + 0x718), mov__eax_1__ret, sizeof(mov__eax_1__ret), true);
		// 
		// 	// mov     r14b, 1
		// 	uint8_t nop2[] = { 0x41, 0xB6, 0x01 };
		// 	ReadWriteProcessMemory(td, td->td_proc, (void*)(libScePsmBase + 0xBC4EA), nop2, sizeof(nop2), true);
		// 	ReadWriteProcessMemory(td, td->td_proc, (void*)(libScePsmBase + 0xBC4C2), nop2, sizeof(nop2), true);
		// 
		// 	unsigned char connectString[] = {
		// 		0x74, 0x72, 0x61, 0x6E, 0x73, 0x70, 0x6F, 0x72, 0x74, 0x3D, 0x64, 0x74,
		// 		0x5F, 0x73, 0x6F, 0x63, 0x6B, 0x65, 0x74, 0x2C, 0x61, 0x64, 0x64, 0x72,
		// 		0x65, 0x73, 0x73, 0x3D, 0x25, 0x73, 0x3A, 0x32, 0x32, 0x32, 0x32, 0x2C,
		// 		0x73, 0x75, 0x73, 0x70, 0x65, 0x6E, 0x64, 0x3D, 0x25, 0x73, 0x2C, 0x73,
		// 		0x65, 0x72, 0x76, 0x65, 0x72, 0x3D, 0x79, 0x00
		// 	};
		// 
		// 	ReadWriteProcessMemory(td, td->td_proc, (void*)(libScePsmBase + 0x21180E), connectString, sizeof(connectString), true);
		// 
		// 	// Disable diagnostics
		// 	uint8_t ret[] = { 0xc3};
		// 	ReadWriteProcessMemory(td, td->td_proc, (void*)(libScePsmBase + 0xD01C0), ret, sizeof(ret), true);
		// 	ReadWriteProcessMemory(td, td->td_proc, (void*)(libScePsmBase + 0xD0410), ret, sizeof(ret), true);
		// 	ReadWriteProcessMemory(td, td->td_proc, (void*)(libScePsmBase + 0xD0420), ret, sizeof(ret), true);
		// 	ReadWriteProcessMemory(td, td->td_proc, (void*)(libScePsmBase + 0xD0430), ret, sizeof(ret), true);
		// 	ReadWriteProcessMemory(td, td->td_proc, (void*)(libScePsmBase + 0xD0440), ret, sizeof(ret), true);
		// 	ReadWriteProcessMemory(td, td->td_proc, (void*)(libScePsmBase + 0xD0470), ret, sizeof(ret), true);
		// 	ReadWriteProcessMemory(td, td->td_proc, (void*)(libScePsmBase + 0xD0480), ret, sizeof(ret), true);
		// 	//ReadWriteProcessMemory(td, td->td_proc, (void*)(libScePsmBase + 0xCFFF0), ret, sizeof(ret), true);
		// 
		// 	auto cred = td->td_proc->p_ucred;
		// 	cred->cr_uid = 0;
		// 	cred->cr_ruid = 0;
		// 	cred->cr_rgid = 0;
		// 	cred->cr_groups[0] = 0;
		// 	cred->cr_sceAuthID = 0x3801000000000013;
		// 	cred->cr_sceCaps[0] = 0xFFFFFFFFFFFFFFFF;
		// 	cred->cr_sceCaps[1] = 0xFFFFFFFFFFFFFFFF;
		// 	cred->cr_prison = *(prison**)prison0;
		// 
		// 	kprintf("libScePsm.sprx patched.\n");
		// }

		if (strstr(info.name, "libSceDipsw.sprx"))
		{
			auto libSceDipswBase = (uint64_t)info.segmentInfo[0].baseAddr;

			// sceKernelIsDevelopmentMode
			ReadWriteProcessMemory(td, td->td_proc, (void*)(libSceDipswBase + 0x820), mov__eax_1__ret, sizeof(mov__eax_1__ret), true);

			kprintf("libScePsm.sprx patched.\n");
		}
	}

	return res;
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