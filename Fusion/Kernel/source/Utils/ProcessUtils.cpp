#include "Common.h"
#include "ProcessUtils.h"

int ReadWriteProcessMemory(thread* td, proc* proc, void* addr, void* data, uint32_t len, bool write)
{
	if (proc == nullptr)
	{
		printf("ReadWriteProcessMemory(): Invalid Process.\n");
		return EINVAL;
	}

	if (addr == nullptr)
	{
		printf("ReadWriteProcessMemory(): Invalid address.\n");
		return EINVAL;
	}

	if (data == nullptr || len <= 0)
	{
		printf("ReadWriteProcessMemory(): Invalid data.\n");
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
		printf("ReadWriteProcessMemory(): proc_rwmem failed with the error %llX\n", res);
		return res;
	}

	return res;
}

int CreateThread(thread* td, void* entry, void* arg, char* stack, size_t stackSize)
{
	return create_thread(td, 0, entry, arg, stack, stackSize, 0, 0, 0, 0, 0);
}

uint64_t AllocateMemory(proc* p, size_t len, int prot, int flags)
{
	vm_offset_t mappedAddress = 0;
	auto pageLength = round_page(len);

	auto vmspace = p->p_vmspace;
	if (!vmspace)
	{
		printf("%s: Process has no vmspace.\n", __FUNCTION__);
		return 0;
	}

	vm_map_t map = &vmspace->vm_map;

	vm_map_lock(map);
	{
		int res = vm_map_findspace(map, map->header.start, pageLength, &mappedAddress);
		if (res != 0 || mappedAddress == 0)
		{
			vm_map_unlock(map);
			printf("%s: vm_map_findspace an error has occurred allocating: %d.\n", __FUNCTION__, res);
			return 0;
		}

		res = vm_map_insert(map, 0, 0, mappedAddress, mappedAddress + pageLength, prot, prot, 0);
		if (res != 0)
		{
			vm_map_unlock(map);
			printf("%s: vm_map_insert an error has occurred allocating: %d, %llu.\n", __FUNCTION__, res, mappedAddress);
			return 0;
		}
	}
	vm_map_unlock(map);

	return mappedAddress;
}

int FreeMemory(proc* p, uint64_t addr, size_t len)
{
	auto vmspace = p->p_vmspace;
	if (!vmspace)
	{
		printf("%s: Process has no vmspace.\n", __FUNCTION__);
		return EINVAL;
	}

	vm_map_t map = &vmspace->vm_map;

	vm_map_lock(map);
	int res = vm_map_delete(map, addr, addr + round_page(len));
	vm_map_unlock(map);

	return res;
}

int dynlib_dlsym(proc* p, int handle, char* symbol, char* library, unsigned int flags, void** addr)
{
	dynlib* dynlib = p->p_dynlib;
	if (!dynlib)
	{
		printf("this is not dynamic linked program.\n");
		return EPERM;
	}

	sx_xlock(&dynlib->bind_lock, 0);
	{
		dynlib_obj* main_obj = dynlib->main_obj;
		if (!main_obj)
		{
			sx_xunlock(&dynlib->bind_lock);

			printf("this is not dynamic linked program.\n");
			return EPERM;
		}

		dynlib_obj* obj = find_obj_by_handle(dynlib, handle);
		if (!obj) 
		{
			sx_xunlock(&dynlib->bind_lock);

			printf("Could not find obj by handle %d.\n", handle);
			return ESRCH;
		}

		*addr = do_dlsym(dynlib, obj, symbol, library, flags);
		if (!*addr) {
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
		printf("this is not dynamic linked program.\n");
		return EPERM;
	}

	sx_xlock(&dynlib->bind_lock, 0);
	{
		dynlib_obj* main_obj = dynlib->main_obj;
		if (!main_obj)
		{
			sx_xunlock(&dynlib->bind_lock);
			printf("this is not dynamic linked program.\n");
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
		printf("this is not dynamic linked program.\n");
		return EPERM;
	}

	sx_xlock(&dynlib->bind_lock, 0);
	{
		dynlib_obj* main_obj = dynlib->main_obj;
		if (!main_obj)
		{
			sx_xunlock(&dynlib->bind_lock);

			printf("this is not dynamic linked program.\n");
			return EPERM;
		}

		dynlib_obj* obj = find_obj_by_handle(dynlib, handle);
		if (!obj) 
		{
			sx_xunlock(&dynlib->bind_lock);

			printf("Could not find obj by handle %d.\n", handle);
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

int GetLibraries(proc* p, OrbisLibraryInfo* libInfos, int maxLibs, int* libCount)
{
	int handles[256];
	int numModules = 0;
	auto res = dynlib_get_list(p, handles, 256, &numModules);
	if (res != 0)
	{
		printf("%s: dynlib_get_list failed with the error %llX\n", __FUNCTION__, res);
		return res;
	}

	if (numModules > maxLibs)
		numModules = maxLibs;

	for (int i = 0; i < numModules; i++)
	{
		res = dynlib_get_info(p, handles[i], &libInfos[i]);
		if (res != 0)
		{
			printf("%s: dynlib_get_info failed with the error %llX\n", __FUNCTION__, res);
			return res;
		}
	}

	*libCount = (int)numModules;
	return 0;
}
