#include "Common.h"
#include "Misc.h"

int isprint(int ch)
{
	return (ch >= 32 && ch <= 126); // Check if the character is within the printable ASCII range
}

void hexdump(void* ptr, int buflen, bool showAddress)
{
	unsigned char* buf = (unsigned char*)ptr;
	int i, j;
	for (i = 0; i < buflen; i += 16)
	{
		showAddress ? kprintf("%06x: ", (uint64_t)ptr + i) : kprintf("%06x: ", i);
		for (j = 0; j < 16; j++)
			if (i + j < buflen)
				kprintf("%02x ", buf[i + j]);
			else
				kprintf("   ");
		kprintf(" ");
		for (j = 0; j < 16; j++)
			if (i + j < buflen)
				kprintf("%c", isprint(buf[i + j]) ? buf[i + j] : '.');
		kprintf("\n");
	}
}

void MemcpyTextSeg(void* dst, void* src, size_t len)
{
	cpu_disable_wp();

	memcpy(dst, src, len);

	cpu_enable_wp();
}

void* AllocateForMap(vm_map_t map, vm_ooffset_t offset, vm_offset_t size, vm_prot_t prot, vm_prot_t max)
{
	// Round up to the next page size.
	size = round_page(size);

	vm_map_lock(map);

	uint64_t memoryAddress = 0;
	auto res = vm_map_findspace(map, offset, size, &memoryAddress);
	if (res != 0 || memoryAddress == 0)
	{
		vm_map_unlock(map);

		kprintf("%s: an error has occurred allocating: %d, %llu.\n", __FUNCTION__, res, memoryAddress);
		return nullptr;
	}

	res = vm_map_insert(map, 0, 0, memoryAddress, memoryAddress + size, prot, max, 0);
	if (res != 0)
	{
		vm_map_delete(map, memoryAddress, memoryAddress + size);

		vm_map_unlock(map);

		kprintf("%s: vm_map_insert an error has occurred allocating: %d, %llu.\n", __FUNCTION__, res, memoryAddress);
		return nullptr;
	}

	vm_map_unlock(map);

	return (void*)(uintptr_t)memoryAddress;
}

void* KmemAllocAt(vm_map_t map, vm_ooffset_t offset, vm_offset_t size)
{
	auto tempStart = map->header.start;
	map->header.start = offset;
	auto tempAddress = (void*)kmem_alloc(map, size);
	map->header.start = tempStart;

	return tempAddress;
}

char* strrchr(char* str, char ch) {
	char* lastOccurrence = 0;

	// Iterate through the string
	while (*str != '\0')
	{
		// If the character matches, update the last occurrence pointer
		if (*str == ch)
		{
			lastOccurrence = str;
		}

		// Move to the next character
		str++;
	}

	// Return the pointer to the last occurrence or NULL if not found
	return lastOccurrence;
}

char* FileNameFromPath(char* path)
{
	// Find the position of the last directory separator
	char* lastSeparator = strrchr(path, '/');

	// If no forward slash is found, try backslash for Windows paths
	if (!lastSeparator)
	{
		lastSeparator = strrchr(path, '\\');
	}

	// If no separator is found, the path itself is the file name
	if (!lastSeparator)
	{
		return path;
	}

	// Return the substring after the last separator
	return lastSeparator + 1;
}

bool GetSandboxPath(thread* td, char* sandboxPath)
{
	if (!td)
	{
		kprintf("GetSandboxPath(): Thread was null.\n");
		return false;
	}

	if (!td->td_proc)
	{
		kprintf("GetSandboxPath(): Process was null.\n");
		return false;
	}

	if (td->td_proc->p_fd->fd_rdir != rootvnode)
	{
		strcpy(sandboxPath, "/");
		return true;
	}

	if (!td->td_proc->p_fd)
	{
		kprintf("GetSandboxPath(): fd was null.\n");
		return false;
	}

	if (!td->td_proc->p_fd->fd_jdir)
	{
		kprintf("GetSandboxPath(): jail dir was null.\n");
		return false;
	}

	auto shellcore = GetProcByName("SceShellCore");
	if (shellcore == nullptr)
	{
		kprintf("GetSandboxPath(): shellcore was null.\n");
		return false;
	}

	mtx_unlock_flags(&shellcore->p_lock, 0);

	char* bufPath = nullptr;
	char* freePath = nullptr;
	auto res = vn_fullpath(shellcore->p_threads.tqh_first, td->td_proc->p_fd->fd_jdir, &bufPath, &freePath);
	if (res != 0)
	{
		kprintf("GetSandboxPath(): vn_fullpath failed. %d\n", res);
		return false;
	}

	// If we got the path delete the free path now we dont need it.
	delete freePath;

	// Copy out the sandbox path.
	if (bufPath != nullptr)
		strcpy(sandboxPath, bufPath);

	return true;
}

bool DoesFileExist(const char* path)
{
	// int fd = open(path, O_RDONLY, 0);
	// if (fd < 0)
	// 	return false;
	// close(fd);
	// return true;

	// TODO: use kern_open instead of open
	return false;
}

int MkDir(const char* path, int mode)
{
	return kern_mkdir(CurrentThread(), (char*)path, 0, mode);
}