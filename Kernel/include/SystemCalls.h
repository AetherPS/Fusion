#pragma once

typedef struct SceDbgModuleSegmentInfo {
	void* baseAddr;								///< Base address of segment
	uint32_t				size;				///< Size of segment
	int32_t					prot;				///< Protection of segment
} SceDbgModuleSegmentInfo;

struct dynlib_info_ex
{
	size_t size;
	char name[256];
	int32_t handle;
	uint16_t tls_index;
	uint16_t pad0;
	uintptr_t tls_init_addr;
	uint32_t tls_init_size;
	uint32_t tls_size;
	uint32_t tls_offset;
	uint32_t tls_align;
	uintptr_t init_proc_addr;
	uintptr_t fini_proc_addr;
	uint64_t reserved1;
	uint64_t reserved2;
	uintptr_t eh_frame_hdr_addr;
	uintptr_t eh_frame_addr;
	uint32_t eh_frame_hdr_size;
	uint32_t eh_frame_size;
	SceDbgModuleSegmentInfo segmentInfo[4];
	uint32_t segment_count;
	uint32_t ref_count;
};

int sys_dynlib_get_list(thread* td, int* handlesOut, size_t handlesOutCount, size_t* actualCount);
int sys_dynlib_get_info_ex(thread* td, int handle, int unk, dynlib_info_ex* pInfo);
caddr_t sys_mmap(thread* td, caddr_t addr, size_t len, int prot, int flags, int fd, off_t pos);
int sys_munmap(thread* td, caddr_t addr, size_t len);
int open(const char* path, int flags, int mode, struct thread* td = CurrentThread());
ssize_t read(int fd, const void* buf, size_t count, struct thread* td = CurrentThread());
ssize_t write(int fd, const void* buf, size_t count, struct thread* td = CurrentThread());
int close(int fd, struct thread* td = CurrentThread());
int stat(char* path, struct stat* buf);
int ioctl(int fd, unsigned long com, caddr_t data, struct thread* td);