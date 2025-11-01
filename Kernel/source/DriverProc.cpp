#include "Common.h"
#include "DriverProc.h"
#include "ProcessUtils.h"
#include "SystemCalls.h"

int DriverProc::OnIoctl(cdev* dev, unsigned long cmd, caddr_t data, int fflag, thread* td)
{
	switch (cmd)
	{
	case CMD_PROC_JAILBREAK:
		return Jailbreak(data, td);

	case CMD_PROC_JAIL:
		return RestoreJail(data, td);

	case CMD_PROC_READ_WRITE_MEMORY:
		return ProcessReadWrite(data, td);

	case CMD_PROC_ALLOC_MEMORY: // Deprecated, use sceDebugCreateScratchExecutableArea or sceDebugDestroyScratchDataArea
		return ProcessAlloc(data, td);

	case CMD_PROC_FREE_MEMORY: // Deprecated, use sceDebugDestroyScratchExecutableArea or sceDebugDestroyScratchDataArea
		return ProcessFree(data);

	case CMD_PROC_START_THREAD:
		return StartThread(data);

	case CMD_PROC_RESOLVE:
		return Resolve(data);

	case CMD_PROC_GET_AUTHID:
		return GetAuthId(data, td);

	case CMD_PROC_SET_AUTHID:
		return SetAuthId(data, td);

	default:
		kprintf("[Proc] Not Implemented. :(\n");
		break;
	}
}

int DriverProc::Jailbreak(caddr_t data, thread* td)
{
	Input_Jailbreak input;
	int res = copyin(data, &input, sizeof(Input_Jailbreak));
	if (res != 0)
	{
		kprintf("%s: copyin failed with error %d\n", __FUNCTION__, res);
		return res;
	}

	proc* selectedProc = pfind(input.ProcessId);
	if (selectedProc == nullptr)
	{
		kprintf("%s: Failed to find Process with the pid %i\n", __FUNCTION__, input.ProcessId);
		return EINVAL;
	}

	auto cred = selectedProc->p_ucred;
	auto fd = selectedProc->p_fd;

	if (fd == nullptr || cred == nullptr)
	{
		kprintf("%s: Failed to get jail info.\n", __FUNCTION__);
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
	copyout(&backupJail, input.Jail, sizeof(JailBackup));

	if (input.AuthId != -1)
	{
		kprintf("%s: Jailbreak with AuthID: %llx\n", __FUNCTION__, input.AuthId);
	}

	// Jailbreak time.
	cred->cr_uid = 0;
	cred->cr_ruid = 0;
	cred->cr_rgid = 0;
	cred->cr_groups[0] = 0;
	cred->cr_sceAuthID = input.AuthId == -1 ? 0x3801000000000013 : input.AuthId;
	cred->cr_sceCaps[0] = 0xFFFFFFFFFFFFFFFF;
	cred->cr_sceCaps[1] = 0xFFFFFFFFFFFFFFFF;
	cred->cr_prison = *(prison**)prison0;
	fd->fd_rdir = fd->fd_jdir = *(vnode**)rootvnode;
	if (input.NullRandPath)
	{
		selectedProc->p_randomized_path_len = 0;
		memset(selectedProc->p_randomized_path, 0, 0x100);
	}

	return 0;
}

int DriverProc::RestoreJail(caddr_t data, thread* td)
{
	Input_RestoreJail input;
	int res = copyin(data, &input, sizeof(Input_RestoreJail));
	if (res != 0)
	{
		kprintf("%s: copyin failed with error %d\n", __FUNCTION__, res);
		return res;
	}

	proc* selectedProc = pfind(input.ProcessId);
	if (selectedProc == nullptr)
	{
		kprintf("%s: Failed to find Process with the pid %i\n", __FUNCTION__, input.ProcessId);
		return EINVAL;
	}

	auto cred = selectedProc->p_ucred;
	auto fd = selectedProc->p_fd;

	if (fd == nullptr || cred == nullptr)
	{
		kprintf("%s: Failed to get jail info.\n", __FUNCTION__);
		return EINVAL;
	}

	// Restore the jail.
	cred->cr_uid = input.Jail.cr_uid;
	cred->cr_ruid = input.Jail.cr_ruid;
	cred->cr_rgid = input.Jail.cr_rgid;
	cred->cr_groups[0] = input.Jail.cr_groups;
	cred->cr_sceAuthID = input.Jail.cr_sceAuthID;
	cred->cr_sceCaps[0] = input.Jail.cr_sceCaps[0];
	cred->cr_sceCaps[1] = input.Jail.cr_sceCaps[1];
	cred->cr_prison = (prison*)input.Jail.cr_prison;
	fd->fd_jdir = (vnode*)input.Jail.fd_jdir;
	fd->fd_rdir = (vnode*)input.Jail.fd_rdir;
	selectedProc->p_randomized_path_len = strlen(selectedProc->p_randomized_path);
	strcpy(selectedProc->p_randomized_path, input.Jail.RandomizedPath);

	return 0;
}

int DriverProc::GetProccessModuleList(caddr_t data, thread* td)
{
	Input_LibraryList input;
	int res = copyin(data, &input, sizeof(Input_LibraryList));
	if (res != 0)
	{
		kprintf("%s: copyin failed with error %d\n", __FUNCTION__, res);
		return res;
	}

	proc* selectedProc = pfind(input.ProcessId);
	if (selectedProc == nullptr)
	{
		kprintf("%s: Failed to find Process with the pid %i\n", __FUNCTION__, input.ProcessId);
		return EINVAL;
	}

	int numModules;
	int handles[256];
	res = dynlib_get_list(selectedProc, handles, 256, &numModules);
	if (res != 0)
	{
		kprintf("%s: dynlib_get_list failed with error %d\n", __FUNCTION__, res);
		return res;
	}

	auto libTemp = (OrbisLibraryInfo*)_malloc(sizeof(OrbisLibraryInfo) * numModules);
	if (!libTemp)
	{
		kprintf("%s: Failed to allocate memory for libTemp.\n", __FUNCTION__);
		return -1;
	}

	for (int i = 0; i < numModules; i++)
	{
		int res = dynlib_get_info(selectedProc, handles[i], &libTemp[i]);
		if (res != 0)
		{
			kprintf("%s: dynlib_get_info failed with error %d for handle %d\n", __FUNCTION__, res, handles[i]);
			_free(libTemp);
			return res;
		}
	}

	// Write the data out to userland.
	copyout(libTemp, input.LibraryListOut, sizeof(OrbisLibraryInfo) * numModules);
	copyout(&numModules, input.LibraryCount, sizeof(int));

	// Free our memory.
	_free(libTemp);

	return 0;
}

int DriverProc::ProcessReadWrite(caddr_t data, thread* td)
{
	Input_ReadWriteMemory input;
	int res = copyin(data, &input, sizeof(Input_ReadWriteMemory));
	if (res != 0)
	{
		kprintf("%s: copyin failed with error %d\n", __FUNCTION__, res);
		return res;
	}

	proc* selectedProc = pfind(input.ProcessId);
	if (selectedProc == nullptr)
	{
		kprintf("%s: Failed to find Process with the pid %i\n", __FUNCTION__, input.ProcessId);
		return EINVAL;
	}

	return ReadWriteProcessMemory(td, selectedProc, (void*)input.ProcessAddress, (void*)input.DataAddress, input.Length, input.IsWrite);
}

int DriverProc::ProcessAlloc(caddr_t data, thread* td)
{
	Input_AllocMemory input;
	int res = copyin(data, &input, sizeof(Input_AllocMemory));
	if (res != 0)
	{
		kprintf("%s: copyin failed with error %d\n", __FUNCTION__, res);
		return res;
	}

	proc* selectedProc = pfind(input.ProcessId);
	if (selectedProc == nullptr)
	{
		kprintf("%s: Failed to find Process with the pid %i\n", __FUNCTION__, input.ProcessId);
		return EINVAL;
	}

	auto selectedThread = selectedProc->p_threads.tqh_first;
	if (selectedThread == nullptr)
	{
		kprintf("%s: Failed to find Thread for Process with the pid %i\n", __FUNCTION__, input.ProcessId);
		return EINVAL;
	}

	auto outAddress = AllocateMemory(selectedThread, input.Length, input.Protection, input.Flags);

	// Copy the result out.
	copyout(&outAddress, input.OutAddress, sizeof(outAddress));

	return 0;
}

int DriverProc::ProcessFree(caddr_t data)
{
	Input_FreeMemory input;
	int res = copyin(data, &input, sizeof(Input_FreeMemory));
	if (res != 0)
	{
		kprintf("%s: copyin failed with error %d\n", __FUNCTION__, res);
		return res;
	}

	proc* selectedProc = pfind(input.ProcessId);
	if (selectedProc == nullptr)
	{
		kprintf("%s: Failed to find Process with the pid %i\n", __FUNCTION__, input.ProcessId);
		return EINVAL;
	}

	auto selectedThread = selectedProc->p_threads.tqh_first;
	if (selectedThread == nullptr)
	{
		kprintf("%s: Failed to find Thread for Process with the pid %i\n", __FUNCTION__, input.ProcessId);
		return EINVAL;
	}

	return FreeMemory(selectedThread, (caddr_t)input.ProcessAddress, input.Length);
}

int DriverProc::StartThread(caddr_t data)
{
	Input_StartThreadInfo input;
	int res = copyin(data, &input, sizeof(Input_StartThreadInfo));
	if (res != 0)
	{
		kprintf("%s: copyin failed with error %d\n", __FUNCTION__, res);
		return res;
	}

	proc* selectedProc = pfind(input.ProcessId);
	if (selectedProc == nullptr)
	{
		kprintf("%s: Failed to find Process with the pid %i\n", __FUNCTION__, input.ProcessId);
		return EINVAL;
	}

	auto selectedThread = selectedProc->p_threads.tqh_first;
	if (selectedThread == nullptr)
	{
		kprintf("%s: Failed to find Thread for Process with the pid %i\n", __FUNCTION__, input.ProcessId);
		return EINVAL;
	}

	return CreateThread(selectedThread, (void*)input.ThreadEntry, nullptr, (char*)input.StackMemory, input.StackSize);
}

int DriverProc::Resolve(caddr_t data)
{
	Input_ResolveInfo input;
	int res = copyin(data, &input, sizeof(Input_ResolveInfo));
	if (res != 0)
	{
		kprintf("%s: copyin failed with error %d\n", __FUNCTION__, res);
		return res;
	}

	proc* selectedProc = pfind(input.ProcessId);
	if (selectedProc == nullptr)
	{
		kprintf("%s: Failed to find Process with the pid %i\n", __FUNCTION__, input.ProcessId);
		return EINVAL;
	}

	uint64_t addr = 0;
	res = dynlib_dlsym(selectedProc, input.Handle, input.Symbol, (*input.Library != '\0') ? input.Library : NULL, input.Flags, (void**)&addr);

	if (addr <= 0)
		return EINVAL;

	// Copy the result out.
	copyout(&addr, input.Result, sizeof(addr));

	return res;
}

int DriverProc::GetAuthId(caddr_t data, thread* td)
{
	Input_AuthId input;
	int res = copyin(data, &input, sizeof(Input_AuthId));
	if (res != 0)
	{
		kprintf("%s: copyin failed with error %d\n", __FUNCTION__, res);
		return res;
	}

	proc* selectedProc = pfind(input.ProcessId);
	if (selectedProc == nullptr)
	{
		kprintf("%s: Failed to find Process with the pid %i\n", __FUNCTION__, input.ProcessId);
		return EINVAL;
	}

	auto cred = selectedProc->p_ucred;
	input.AuthId = cred->cr_sceAuthID;

	copyout(&input, data, sizeof(Input_AuthId));

	return 0;
}

int DriverProc::SetAuthId(caddr_t data, thread* td)
{
	Input_AuthId input;
	int res = copyin(data, &input, sizeof(Input_AuthId));
	if (res != 0)
	{
		kprintf("%s: copyin failed with error %d\n", __FUNCTION__, res);
		return res;
	}

	proc* selectedProc = pfind(input.ProcessId);
	if (selectedProc == nullptr)
	{
		kprintf("%s: Failed to find Process with the pid %i\n", __FUNCTION__, input.ProcessId);
		return EINVAL;
	}

	auto cred = selectedProc->p_ucred;
	cred->cr_sceAuthID = input.AuthId;

	return 0;
}