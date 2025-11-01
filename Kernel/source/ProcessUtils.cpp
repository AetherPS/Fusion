#include "Common.h"
#include "ProcessUtils.h"
#include "SystemCalls.h"

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

int dynlib_dlsym(proc* p, int handle, char* symbol, char* library, unsigned int flags, void* addr)
{
	dynlib* dynlib = p->p_dynlib;
	if (!dynlib)
	{
		kprintf("this is not dynamic linked program.\n");
		return EPERM;
	}

	sx_xlock(&dynlib->bind_lock, 0);
	{
		dynlib_obj* main_obj = dynlib->main_obj;
		if (!main_obj)
		{
			sx_xunlock(&dynlib->bind_lock);

			kprintf("this is not dynamic linked program.\n");
			return EPERM;
		}

		dynlib_obj* obj = find_obj_by_handle(dynlib, handle);
		if (!obj) {
			sx_xunlock(&dynlib->bind_lock);
			return ESRCH;
		}

		addr = do_dlsym(dynlib, obj, symbol, library, flags);
		if (!addr) {
			sx_xunlock(&dynlib->bind_lock);
			return ESRCH;
		}
	}
	sx_xunlock(&dynlib->bind_lock);

	return 0;
}

int dynlib_get_list(proc* p, int* handles, int max_handles, int* handle_count)
{
	dynlib* dynlib = p->p_dynlib;
	if (!dynlib)
	{
		kprintf("this is not dynamic linked program.\n");
		return EPERM;
	}

	sx_xlock(&dynlib->bind_lock, 0);
	{
		dynlib_obj* main_obj = dynlib->main_obj;
		if (!main_obj)
		{
			sx_xunlock(&dynlib->bind_lock);
			kprintf("this is not dynamic linked program.\n");
			return EPERM;
		}

		int count = 0;
		dynlib_obj* obj;
		SLIST_FOREACH(obj, &dynlib->objs, entries)
		{
			if (count < max_handles)
			{
				handles[count] = obj->handle;
			}
			count++;
		}
		*handle_count = count;
	}
	sx_xunlock(&dynlib->bind_lock);
	return 0;
}

int dynlib_get_info(proc* p, int handle, OrbisLibraryInfo* info)
{
	dynlib* dynlib = p->p_dynlib;
	if (!dynlib)
	{
		kprintf("this is not dynamic linked program.\n");
		return EPERM;
	}

	sx_xlock(&dynlib->bind_lock, 0);
	{
		dynlib_obj* main_obj = dynlib->main_obj;
		if (!main_obj)
		{
			sx_xunlock(&dynlib->bind_lock);

			kprintf("this is not dynamic linked program.\n");
			return EPERM;
		}

		dynlib_obj* obj = find_obj_by_handle(dynlib, handle);
		if (!obj) {
			sx_xunlock(&dynlib->bind_lock);
			return ESRCH;
		}

		info->Handle = obj->handle;
		strcpy(info->Path, obj->path);
		info->MapBase = (uint64_t)obj->map_base;
		info->MapSize = obj->map_size;
		info->TextSize = obj->text_size;
		info->DataBase = (uint64_t)obj->database;
		info->DataSize = obj->data_size;
	}
	sx_xunlock(&dynlib->bind_lock);

	return 0;
}
