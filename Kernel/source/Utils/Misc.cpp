#include "Common.h"
#include "Misc.h"

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

void* KmemAllocAt(vm_map_t map, vm_ooffset_t offset, vm_offset_t size)
{
	auto tempStart = map->header.start;
	map->header.start = offset;
	auto tempAddress = (void*)kmem_alloc(map, size);
	map->header.start = tempStart;

	return tempAddress;
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

int MkDir(const char* path, int mode)
{
	return kern_mkdir(CurrentThread(), (char*)path, 0, mode);
}

void MemcpyTextSeg(void* dst, void* src, size_t len)
{
	cpu_disable_wp();

	memcpy(dst, src, len);

	cpu_enable_wp();
}

bool IsRelativelyClose(void* address1, void* address2)
{
	const uintptr_t addr1 = reinterpret_cast<uintptr_t>(address1);
	const uintptr_t addr2 = reinterpret_cast<uintptr_t>(address2);
	const int64_t diff = static_cast<int64_t>(addr1 - addr2);
	return diff >= INT32_MIN && diff <= INT32_MAX;
}

uintptr_t GetRelativeAddress(void* address, void* destination)
{
	return size_t(destination) - (size_t(address) + JUMP_32SIZE);
}

void WriteJump32(void* address, void* destination)
{
	uint8_t jmp_bytes[JUMP_32SIZE] = { 0xE9, 0x90, 0x90, 0x90, 0x90 };
	*reinterpret_cast<uint32_t*>(jmp_bytes + 1) = GetRelativeAddress(address, destination);

	MemcpyTextSeg(address, jmp_bytes, sizeof(jmp_bytes));
}

void WriteJump64(void* address, void* destination)
{
	uint8_t jmp_bytes[JUMP_64SIZE] = { 0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
	*reinterpret_cast<uint64_t*>(jmp_bytes + 6) = reinterpret_cast<uintptr_t>(destination);

	MemcpyTextSeg(address, jmp_bytes, sizeof(jmp_bytes));
}

void WriteCall32(void* address, void* destination)
{
	uint8_t call_bytes[CALL_32SIZE] = { 0xE8, 0x90, 0x90, 0x90, 0x90 };
	*reinterpret_cast<uint32_t*>(call_bytes + 1) = GetRelativeAddress(address, destination);

	MemcpyTextSeg(address, call_bytes, sizeof(call_bytes));
}