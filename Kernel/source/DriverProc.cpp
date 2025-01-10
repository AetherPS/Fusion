#include "Common.h"
#include "DriverProc.h"
#include "ProcessUtils.h"
#include "SystemCalls.h"

int DriverProc::OnIoctl(cdev* dev, unsigned long cmd, caddr_t data, int fflag, thread* td)
{
	switch (cmd)
	{
	case CMD_PROC_MODULE_LIST:
		return GetProccessModuleList(data, td);

	case CMD_PROC_READ_WRITE_MEMORY:
		return ProcessReadWrite(data, td);

	case CMD_PROC_ALLOC_MEMORY:
		return ProcessAlloc(data, td);

	case CMD_PROC_FREE_MEMORY:
		return ProcessFree(data);

	case CMD_PROC_START_THREAD:
		return StartThread(data);

	case CMD_PROC_RESOLVE:
		return Resolve(data);

	case CMD_PROC_JAILBREAK:
		return Jailbreak(data, td);

	case CMD_PROC_JAIL:
		return RestoreJail(data, td);

	case CMD_PROC_SANDBOX_PATH:
		return SandBoxPath(data, td);

	default:
		kprintf("[Proc] Not Implemented. :(\n");
		break;
	}
}

int DriverProc::GetProccessModuleList(caddr_t data, thread* td)
{
	auto args = (Input_LibraryList*)data;
	if (args == nullptr)
	{
		kprintf("GetProccessModuleList(): Data pointer invalid...\n");
		return EINVAL;
	}

	proc* selectedProc = GetProcByPid(args->ProcessId);

	if (selectedProc == nullptr)
	{
		kprintf("GetProccessModuleList(): Failed to find Process with the pid %i\n", args->ProcessId);
		return EINVAL;
	}

	size_t numModules;
	int handles[256];
	sys_dynlib_get_list(selectedProc->p_threads.tqh_first, handles, 256, &numModules);

	auto libTemp = (OrbisLibraryInfo*)_malloc(sizeof(OrbisLibraryInfo) * numModules);
	if (!libTemp)
	{
		kprintf("GetProccessModuleList(): Failed to allocate memory for libTemp.\n");
		return -1;
	}

	for (int i = 0; i < numModules; i++)
	{
		struct dynlib_info_ex info;
		info.size = sizeof(struct dynlib_info_ex);
		sys_dynlib_get_info_ex(selectedProc->p_threads.tqh_first, handles[i], 1, &info);

		libTemp[i].Handle = handles[i];
		strncpy(libTemp[i].Path, info.name, 256);
		libTemp[i].MapBase = (uint64_t)info.segmentInfo[0].baseAddr;
		libTemp[i].MapSize = info.segmentInfo[0].size + info.segmentInfo[1].size;
		libTemp[i].TextSize = info.segmentInfo[0].size;
		libTemp[i].DataBase = (uint64_t)info.segmentInfo[1].baseAddr;
		libTemp[i].DataSize = info.segmentInfo[1].size;
	}

	// Write the data out to userland.
	copyout(libTemp, args->LibraryListOut, sizeof(OrbisLibraryInfo) * numModules);
	copyout(&numModules, args->LibraryCount, sizeof(int));

	// Free our memory.
	_free(libTemp);

	return 0;
}

int DriverProc::ProcessReadWrite(caddr_t data, thread* td)
{
	auto args = (Input_ReadWriteMemory*)data;
	if (args == nullptr)
	{
		kprintf("ProcessReadWrite(): Data pointer invalid...\n");
		return EINVAL;
	}

	proc* selectedProc = GetProcByPid(args->ProcessId);

	if (selectedProc == nullptr)
	{
		kprintf("ProcessReadWrite(): Failed to find Process with the pid %i\n", args->ProcessId);
		return EINVAL;
	}

	return ReadWriteProcessMemory(td, selectedProc, (void*)args->ProcessAddress, (void*)args->DataAddress, args->Length, args->IsWrite);
}

int DriverProc::ProcessAlloc(caddr_t data, thread* td)
{
	auto args = (Input_AllocMemory*)data;
	if (args == nullptr)
	{
		kprintf("ProcessAlloc(): Data pointer invalid...\n");
		return EINVAL;
	}

	proc* selectedProc = GetProcByPid(args->ProcessId);
	if (selectedProc == nullptr)
	{
		kprintf("ProcessAlloc(): Failed to find Process with the pid %i\n", args->ProcessId);
		return EINVAL;
	}

	auto outAddress = AllocateMemory(selectedProc->p_threads.tqh_first, args->Length, args->Protection, args->Flags);

	// Copy the result out.
	copyout(&outAddress, args->OutAddress, sizeof(outAddress));

	return 0;
}

int DriverProc::ProcessFree(caddr_t data)
{
	auto args = (Input_FreeMemory*)data;
	if (args == nullptr)
	{
		kprintf("ProcessFree(): Data pointer invalid...\n");
		return EINVAL;
	}

	proc* selectedProc = GetProcByPid(args->ProcessId);
	if (selectedProc == nullptr)
	{
		kprintf("ProcessFree(): Failed to find Process with the pid %i\n", args->ProcessId);
		return EINVAL;
	}

	return FreeMemory(selectedProc->p_threads.tqh_first, (caddr_t)args->ProcessAddress, args->Length);
}

int DriverProc::StartThread(caddr_t data)
{
	auto args = (Input_StartThreadInfo*)data;
	if (args == nullptr)
	{
		kprintf("StartThread(): Data pointer invalid...\n");
		return EINVAL;
	}

	proc* selectedProc = GetProcByPid(args->ProcessId);
	if (selectedProc == nullptr)
	{
		kprintf("StartThread(): Failed to find Process with the pid %i\n", args->ProcessId);
		return EINVAL;
	}

	return CreateThread(selectedProc->p_threads.tqh_first, (void*)args->ThreadEntry, nullptr, (char*)args->StackMemory, args->StackSize);
}

int DriverProc::Resolve(caddr_t data)
{
	auto args = (Input_ResolveInfo*)data;
	if (args == nullptr)
	{
		kprintf("Resolve(): Data pointer invalid...\n");
		return EINVAL;
	}

	proc* selectedProc = GetProcByPid(args->ProcessId);
	if (selectedProc == nullptr)
	{
		kprintf("Resolve(): Failed to find Process with the pid %i\n", args->ProcessId);
		return EINVAL;
	}

	uint64_t addr;
	sys_dynlib_dlsym(selectedProc->p_threads.tqh_first, args->handle, args->Symbol, (void**)&addr);

	if (addr <= 0)
		return -1;

	// Copy the result out.
	copyout(&addr, args->Result, sizeof(addr));

	return 0;
}

int DriverProc::Jailbreak(caddr_t data, thread* td)
{
	auto args = (Input_Jailbreak*)data;
	if (args == nullptr)
	{
		kprintf("Jailbreak(): Data pointer invalid...\n");
		return EINVAL;
	}

	proc* selectedProc = GetProcByPid(args->ProcessId);
	if (selectedProc == nullptr)
	{
		kprintf("Jailbreak(): Failed to find Process with the pid %i\n", args->ProcessId);
		return EINVAL;
	}

	auto cred = selectedProc->p_ucred;
	auto fd = selectedProc->p_fd;

	if (fd == nullptr || cred == nullptr)
	{
		kprintf("Jailbreak(): Failed to get jail info.\n");
		return EINVAL;
	}

	// Backup the jail.
	JailBackup backupJail;
	backupJail.cr_uid = cred->cr_uid;
	backupJail.cr_ruid = cred->cr_ruid;
	backupJail.cr_rgid = cred->cr_rgid;
	backupJail.cr_groups = cred->cr_groups[0];
	backupJail.cr_sceAuthID = cred->cr_sceAuthID;
	backupJail.cr_sceCaps[0] = cred->cr_sceCaps[0];
	backupJail.cr_sceCaps[1] = cred->cr_sceCaps[1];
	backupJail.cr_prison = cred->cr_prison;
	backupJail.fd_jdir = fd->fd_jdir;
	backupJail.fd_rdir = fd->fd_rdir;
	strcpy(backupJail.RandomizedPath, selectedProc->p_randomized_path);

	// Copy the jail out to userland.
	copyout(&backupJail, args->Jail, sizeof(JailBackup));

	if (args->AuthId != -1)
	{
		kprintf("Jailbreak(): Jailbreak with AuthID: %llx\n", args->AuthId);
	}

	// Jailbreak time.
	cred->cr_uid = 0;
	cred->cr_ruid = 0;
	cred->cr_rgid = 0;
	cred->cr_groups[0] = 0;
	cred->cr_sceAuthID = args->AuthId == -1 ? 0x3801000000000013 : args->AuthId;
	cred->cr_sceCaps[0] = 0xFFFFFFFFFFFFFFFF;
	cred->cr_sceCaps[1] = 0xFFFFFFFFFFFFFFFF;
	cred->cr_prison = *(prison**)prison0;
	fd->fd_rdir = fd->fd_jdir = *(vnode**)rootvnode;
	if (args->NullRandPath)
		memset(selectedProc->p_randomized_path, 0, 0x100);

	return 0;
}

int DriverProc::RestoreJail(caddr_t data, thread* td)
{
	auto args = (Input_RestoreJail*)data;
	if (args == nullptr)
	{
		kprintf("RestoreJail(): Data pointer invalid...\n");
		return EINVAL;
	}

	proc* selectedProc = GetProcByPid(args->ProcessId);
	if (selectedProc == nullptr)
	{
		kprintf("RestoreJail(): Failed to find Process with the pid %i\n", args->ProcessId);
		return EINVAL;
	}

	auto cred = selectedProc->p_ucred;
	auto fd = selectedProc->p_fd;

	// Restore the jail.
	cred->cr_uid = args->Jail.cr_uid;
	cred->cr_ruid = args->Jail.cr_ruid;
	cred->cr_rgid = args->Jail.cr_rgid;
	cred->cr_groups[0] = args->Jail.cr_groups;
	cred->cr_sceAuthID = args->Jail.cr_sceAuthID;
	cred->cr_sceCaps[0] = args->Jail.cr_sceCaps[0];
	cred->cr_sceCaps[1] = args->Jail.cr_sceCaps[1];
	cred->cr_prison = (prison*)args->Jail.cr_prison;
	fd->fd_jdir = (vnode*)args->Jail.fd_jdir;
	fd->fd_rdir = (vnode*)args->Jail.fd_rdir;
	strcpy(selectedProc->p_randomized_path, args->Jail.RandomizedPath);

	return 0;
}

int DriverProc::SandBoxPath(caddr_t data, thread* td)
{
	auto args = (Input_SandboxPath*)data;
	if (args == nullptr)
	{
		kprintf("SandBoxPath(): Data pointer invalid...\n");
		return EINVAL;
	}

	proc* selectedProc = GetProcByPid(args->ProcessId);
	if (selectedProc == nullptr)
	{
		kprintf("SandBoxPath(): Failed to find Process with the pid %i\n", args->ProcessId);
		return EINVAL;
	}

	char sandBoxPath[0x400];
	if (!GetSandboxPath(selectedProc->p_threads.tqh_first, sandBoxPath))
	{
		kprintf("SandBoxPath(): Failed to get the sandbox path.\n");
		return EINVAL;
	}

	copyout(sandBoxPath, args->Buffer, args->BufferSize);

	return 0;
}