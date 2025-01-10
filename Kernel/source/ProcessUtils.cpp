#include "Common.h"
#include "ProcessUtils.h"
#include "SystemCalls.h"

proc* GetProcByPid(int pid)
{
	proc* currentProc = allproc;

	while (currentProc != nullptr)
	{
		if (currentProc->p_pid == pid)
		{
			return currentProc;
		}

		currentProc = currentProc->p_list.le_next;
	}

	return nullptr;
}

proc* GetProcByName(const char* name)
{
	proc* currentProc = allproc;

	while (currentProc != nullptr)
	{
		if (strstr(currentProc->p_comm, name))
		{
			return currentProc;
		}

		currentProc = currentProc->p_list.le_next;
	}

	return nullptr;
}

int ReadWriteProcessMemory(thread* td, proc* proc, void* addr, void* data, uint32_t len, bool write)
{
	if (proc == nullptr)
	{
		kprintf("ReadWriteProcessMemory(): Invalid Process.\n");
		return EINVAL;
	}

	if (addr == nullptr)
	{
		kprintf("ReadWriteProcessMemory(): Invalid address.\n");
		return EINVAL;
	}

	if (data == nullptr || len <= 0)
	{
		kprintf("ReadWriteProcessMemory(): Invalid data.\n");
		return EINVAL;
	}

	iovec IoVec;
	memset(&IoVec, 0, sizeof(IoVec));
	IoVec.iov_base = data;
	IoVec.iov_len = len;

	uio Uio;
	memset(&Uio, 0, sizeof(Uio));
	Uio.uio_iov = &IoVec;
	Uio.uio_iovcnt = 1;
	Uio.uio_offset = (uint64_t)addr;
	Uio.uio_resid = (uint64_t)len;
	Uio.uio_segflg = UIO_SYSSPACE;
	Uio.uio_rw = write ? UIO_WRITE : UIO_READ;
	Uio.uio_td = td;

	auto res = proc_rwmem(proc, &Uio);
	if (res != 0)
	{
		kprintf("ReadWriteProcessMemory(): proc_rwmem failed with the error %llX\n", res);
		return res;
	}

	return res;
}

int CreateThread(thread* td, void* entry, void* arg, char* stack, size_t stackSize)
{
	return create_thread(td, 0, entry, arg, stack, stackSize, 0, 0, 0, 0, 0);
}

caddr_t AllocateMemory(thread* td, size_t len, int prot, int flags)
{
	return sys_mmap(td, nullptr, len, prot, flags, -1, 0);
}

int FreeMemory(thread* td, caddr_t addr, size_t len)
{
	return sys_munmap(td, addr, len);
}
