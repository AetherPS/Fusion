#include "Common.h"
#include "DriverProc.h"

int DriverProc::OnIoctl(cdev* dev, unsigned long cmd, caddr_t data, int fflag, thread* td)
{
	switch (cmd)
	{
	case CMD_PROC_JAILBREAK:
		return Jailbreak(data);

	case CMD_PROC_JAIL:
		return RestoreJail(data);

	case CMD_PROC_MODULE_LIST:
		return GetProccessModuleList(data);

	case CMD_PROC_READ_WRITE_MEMORY:
		return ProcessReadWrite(data);

	case CMD_PROC_ALLOC_MEMORY:
		return ProcessAlloc(data);

	case CMD_PROC_FREE_MEMORY:
		return ProcessFree(data);

	case CMD_PROC_START_THREAD:
		return StartThread(data);

	case CMD_PROC_RESOLVE:
		return Resolve(data);

	case CMD_PROC_GET_AUTHID:
		return GetAuthId(data);

	case CMD_PROC_SET_AUTHID:
		return SetAuthId(data);

	default:
		kprintf("Driver Command %d Not Implemented. :(\n", cmd);
		return ENOSYS;
	}
}

int DriverProc::Jailbreak(caddr_t data)
{
	Input_Jailbreak input;
	thread* td = nullptr;
	proc* p = nullptr;

	int res = FusionDriver::GetProcessThreadInput(data, &input, &td, &p);
	if (res != 0)
		return res;

	auto cred = p->p_ucred;
	auto fd = p->p_fd;

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
	strcpy(backupJail.RandomizedPath, p->p_randomized_path);

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
		p->p_randomized_path_len = 0;
		memset(p->p_randomized_path, 0, 0x100);
	}

	mtx_unlock_flags(&p->p_lock, 0);

	return 0;
}

int DriverProc::RestoreJail(caddr_t data)
{
	Input_RestoreJail input;
	thread* td = nullptr;
	proc* p = nullptr;

	int res = FusionDriver::GetProcessThreadInput(data, &input, &td, &p);
	if (res != 0)
		return res;

	auto cred = p->p_ucred;
	auto fd = p->p_fd;

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
	p->p_randomized_path_len = strlen(p->p_randomized_path);
	strcpy(p->p_randomized_path, input.Jail.RandomizedPath);

	mtx_unlock_flags(&p->p_lock, 0);

	return 0;
}

int DriverProc::GetProccessModuleList(caddr_t data)
{
	Input_LibraryList input;
	thread* td = nullptr;
	proc* p = nullptr;

	int res = FusionDriver::GetProcessThreadInput(data, &input, &td, &p);
	if (res != 0)
		return res;

	mtx_unlock_flags(&p->p_lock, 0);

	auto libTemp = (OrbisLibraryInfo*)_malloc(sizeof(OrbisLibraryInfo) * input.MaxOutCount);
	if (!libTemp)
	{
		kprintf("%s: Failed to allocate memory for libTemp.\n", __FUNCTION__);
		return -1;
	}

	int realCount = 0;
	res = GetLibraries(p, libTemp, input.MaxOutCount, &realCount);
	if (res != 0)
	{
		kprintf("%s: GetLibraries failed with the error %d\n", __FUNCTION__, res);
		_free(libTemp);
		return res;
	}

	copyout(libTemp, input.LibraryListOut, sizeof(OrbisLibraryInfo) * input.MaxOutCount);
	_free(libTemp);

	// Copy the number of modules out if the pointer is valid.
	if (input.LibraryCount != nullptr)
	{
		copyout(&realCount, input.LibraryCount, sizeof(int));
	}

	return 0;
}

int DriverProc::ProcessReadWrite(caddr_t data)
{
	Input_ReadWriteMemory input;
	thread* td = nullptr;
	proc* p = nullptr;

	int res = FusionDriver::GetProcessThreadInput(data, &input, &td, &p);
	if (res != 0)
		return res;

	mtx_unlock_flags(&p->p_lock, 0);

	return ReadWriteProcessMemory(td, p, (void*)input.ProcessAddress, (void*)input.DataAddress, input.Length, input.IsWrite);
}

int DriverProc::ProcessAlloc(caddr_t data)
{
	Input_AllocMemory input;
	thread* td = nullptr;
	proc* p = nullptr;

	int res = FusionDriver::GetProcessThreadInput(data, &input, &td, &p);
	if (res != 0)
		return res;

	auto outAddress = AllocateMemory(p, input.Length, input.Protection, input.Flags);

	mtx_unlock_flags(&p->p_lock, 0);

	// Copy the result out.
	copyout(&outAddress, input.OutAddress, sizeof(outAddress));

	return 0;
}

int DriverProc::ProcessFree(caddr_t data)
{
	Input_FreeMemory input;
	thread* td = nullptr;
	proc* p = nullptr;

	int res = FusionDriver::GetProcessThreadInput(data, &input, &td, &p);
	if (res != 0)
		return res;

	res = FreeMemory(p, input.ProcessAddress, input.Length);

	mtx_unlock_flags(&p->p_lock, 0);

	return res;
}

int DriverProc::StartThread(caddr_t data)
{
	Input_StartThreadInfo input;
	thread* td = nullptr;
	proc* p = nullptr;

	int res = FusionDriver::GetProcessThreadInput(data, &input, &td, &p);
	if (res != 0)
		return res;

	mtx_unlock_flags(&p->p_lock, 0);

	return CreateThread(td, (void*)input.ThreadEntry, nullptr, (char*)input.StackMemory, input.StackSize);
}

int DriverProc::Resolve(caddr_t data)
{
	Input_ResolveInfo input;
	thread* td = nullptr;
	proc* p = nullptr;

	int res = FusionDriver::GetProcessThreadInput(data, &input, &td, &p);
	if (res != 0)
		return res;

	mtx_unlock_flags(&p->p_lock, 0);

	res = dynlib_dlsym(p, input.Handle, input.Symbol, (*input.Library != '\0') ? input.Library : NULL, input.Flags, (void**)&input.Address);

	copyout(&input, data, sizeof(Input_ResolveInfo));

	return res;
}

int DriverProc::GetAuthId(caddr_t data)
{
	Input_AuthId input;
	thread* td = nullptr;
	proc* p = nullptr;

	int res = FusionDriver::GetProcessThreadInput(data, &input, &td, &p);
	if (res != 0)
		return res;

	input.AuthId = p->p_ucred->cr_sceAuthID;

	copyout(&input, data, sizeof(Input_AuthId));

	mtx_unlock_flags(&p->p_lock, 0);

	return 0;
}

int DriverProc::SetAuthId(caddr_t data)
{
	Input_AuthId input;
	thread* td = nullptr;
	proc* p = nullptr;

	int res = FusionDriver::GetProcessThreadInput(data, &input, &td, &p);
	if (res != 0)
		return res;

	p->p_ucred->cr_sceAuthID = input.AuthId;

	mtx_unlock_flags(&p->p_lock, 0);

	return 0;
}