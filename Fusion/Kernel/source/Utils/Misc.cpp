#include "Common.h"
#include "Misc.h"

void* KmemAllocAt(vm_map_t map, vm_ooffset_t offset, vm_offset_t size)
{
	auto tempStart = map->header.start;
	map->header.start = offset;
	auto tempAddress = (void*)kmem_alloc(map, size);
	map->header.start = tempStart;

	return tempAddress;
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

char* basename(char* path)
{
	return dynlib_is_host_path(path) ? dynlib_basename_host(path) : dynlib_basename(path);
}