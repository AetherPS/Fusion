#include "Common.h"
#include "SystemCalls.h"

caddr_t sys_mmap(thread* td, caddr_t addr, size_t len, int prot, int flags, int fd, off_t pos)
{
	struct mmap_args {
		caddr_t addr;
		uint64_t len;
		uint64_t prot;
		uint64_t flags;
		uint64_t fd;
		uint64_t pos;
	};

	// clear errors
	td->td_retval[0] = 0;

	// Set up Params
	mmap_args uap;
	uap.addr = (caddr_t)addr;
	uap.len = len;
	uap.prot = prot;
	uap.flags = flags;
	uap.fd = fd;
	uap.pos = pos;

	// Call System call
	auto _sys_mmap = (int(*)(thread*, mmap_args*))sysvec->sv_table[477].sy_call;
	auto errorno = _sys_mmap(td, &uap);
	if (errorno)
		return (caddr_t)(int64_t)-errorno;

	// success
	return (caddr_t)td->td_retval[0];
}

int sys_munmap(thread* td, caddr_t addr, size_t len)
{
	struct munmap_args {
		void* addr;   // Starting address of the memory region to unmap
		size_t len;   // Length of the memory region to unmap
	};

	// clear errors
	td->td_retval[0] = 0;

	// Set up Params
	munmap_args uap;
	uap.addr = addr;
	uap.len = len;

	// Call System call
	auto _sys_munmap = (int(*)(thread*, munmap_args*))sysvec->sv_table[73].sy_call;
	auto errorno = _sys_munmap(td, &uap);
	if (errorno)
		return -errorno;

	// success
	return td->td_retval[0];
}

