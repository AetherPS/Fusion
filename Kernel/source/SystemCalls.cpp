#include "Common.h"
#include "SystemCalls.h"

int sys_dynlib_get_list(thread* td, int* handlesOut, size_t handlesOutCount, size_t* actualCount)
{
	struct dynlib_get_list_args
	{
		uint64_t handlesOut;
		uint64_t handlesOutCount;
		uint64_t actualCount;
	};

	// clear errors
	td->td_retval[0] = 0;

	// Set up Params
	dynlib_get_list_args uap;
	uap.handlesOut = (uint64_t)handlesOut;
	uap.handlesOutCount = handlesOutCount;
	uap.actualCount = (uint64_t)actualCount;

	// Call System call
	auto _sys_dynlib_get_list = (int(*)(thread*, dynlib_get_list_args*))sysvec->sv_table[592].sy_call;
	auto errorno = _sys_dynlib_get_list(td, &uap);
	if (errorno)
		return -errorno;

	// success
	return td->td_retval[0];
}

int sys_dynlib_get_info_ex(thread* td, int handle, int unk, dynlib_info_ex* pInfo)
{
	struct dynlib_get_info_ex_args
	{
		uint64_t handle;
		uint64_t unk;
		uint64_t pInfo;
	};

	// clear errors
	td->td_retval[0] = 0;

	// Set up Params
	dynlib_get_info_ex_args uap;
	uap.handle = handle;
	uap.unk = unk;
	uap.pInfo = (uint64_t)pInfo;

	// Call System call
	auto _sys_dynlib_get_info_ex = (int(*)(thread*, dynlib_get_info_ex_args*))sysvec->sv_table[608].sy_call;
	auto errorno = _sys_dynlib_get_info_ex(td, &uap);
	if (errorno)
		return -errorno;

	// success
	return td->td_retval[0];
}

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

int open(const char* path, int flags, int mode, struct thread* td)
{
	int error;
	td->td_retval[0] = 0;

	error = kern_open(td, path, 1, flags, mode);
	if (error)
		return -error;

	return (int)td->td_retval[0];
}

ssize_t read(int fd, const void* buf, size_t count, struct thread* td)
{
	struct sys_read_args {
		uint64_t fd;
		uint64_t buf;
		uint64_t count;
	};

	td->td_retval[0] = 0;

	sys_read_args uap;
	uap.fd = fd;
	uap.buf = (uint64_t)buf;
	uap.count = count;

	int errorno = ((int(*)(thread* td, sys_read_args* uap))sysvec->sv_table[3].sy_call)(td, &uap);
	if (errorno)
		return -errorno;

	return (ssize_t)td->td_retval[0];
}


ssize_t write(int fd, const void* buf, size_t count, struct thread* td)
{
	struct sys_write_args {
		uint64_t fd;
		uint64_t buf;
		uint64_t count;
	};

	auto sys_write = (int(*)(thread* td, sys_write_args* uap))sysvec->sv_table[4].sy_call;

	td->td_retval[0] = 0;

	sys_write_args uap;
	uap.fd = fd;
	uap.buf = (uint64_t)buf;
	uap.count = count;

	int errorno = sys_write(td, &uap);
	if (errorno)
		return -errorno;

	return (ssize_t)td->td_retval[0];
}

int close(int fd, struct thread* td)
{
	struct sys_close_args {
		uint64_t fd;
	};

	auto sys_close = (int(*)(thread * td, sys_close_args * uap))sysvec->sv_table[6].sy_call;

	td->td_retval[0] = 0;

	sys_close_args uap;
	uap.fd = fd;

	int errorno = sys_close(td, &uap);
	if (errorno)
		return -errorno;

	return (int)td->td_retval[0];
}

int stat(char* path, struct stat* buf)
{
	struct stat_args {
		char* path;         // Pointer to the file path
		struct stat* ub;    // Pointer to a struct stat
	};

	stat_args uap;
	auto sys_stat = (int(*)(thread*, stat_args*))sysvec->sv_table[188].sy_call;

	// Use shellcore.
	auto shellcore = GetProcByName("SceShellCore");
	thread* td = shellcore->p_threads.tqh_first;

	// clear errors
	td->td_retval[0] = 0;

	// Set up Params
	uap.path = (char*)path;
	uap.ub = buf;

	// Call System call
	int errorno = sys_stat(td, &uap);
	if (errorno)
		return -errorno;

	// success
	return td->td_retval[0];
}

int ioctl(int fd, unsigned long com, caddr_t data, struct thread* td)
{
	struct ioctl_args {
		int fd;         // File descriptor
		unsigned long com;     // IOCTL command
		caddr_t data;   // Pointer to data
	};

	ioctl_args uap;
	auto sys_ioctl = (int(*)(thread*, ioctl_args*))sysvec->sv_table[54].sy_call;

	// clear errors
	td->td_retval[0] = 0;

	// Set up Params
	uap.fd = fd;
	uap.com = com;
	uap.data = data;

	// Call System call
	int errorno = sys_ioctl(td, &uap);
	if (errorno)
		return -errorno;

	// success
	return td->td_retval[0];
}