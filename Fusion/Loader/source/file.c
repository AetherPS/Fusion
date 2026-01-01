#include "common.h"
#include "file.h"

bool FileExist(const char* path)
{
	int fd = sceKernelOpen(path, SCE_KERNEL_O_RDONLY, 0);
	if (fd <= 0)
	{
		return false;
	}

	sceKernelClose(fd);
	return true;
}

bool CreateDirectory(const char* path)
{
	int result = sceKernelMkdir(path, 0777);
	if (result == 0 || result == 0x80020011) // SCE_KERNEL_ERROR_EEXIST
	{
		return true;
	}
	else
	{
		return false;
	}
}

int RemoveFile(const char* path)
{
	return sceKernelUnlink(path);
}

int WriteFile(const char* path, uint8_t* data, size_t len)
{
	int fd = sceKernelOpen(path, SCE_KERNEL_O_WRONLY | SCE_KERNEL_O_CREAT, 0777);

	if (fd)
	{
		int result = sceKernelWrite(fd, data, len);

		if (result <= 0)
		{
			sceKernelClose(fd);

			return result;
		}

		sceKernelClose(fd);

		return 0;
	}
	else
	{
		return fd;
	}
}

void BuildIovec(struct iovec** iov, int* iovlen, const char* name, const void* val, size_t len)
{
	int i;

	if (*iovlen < 0)
		return;

	i = *iovlen;
	*iov = (struct iovec*)realloc(*iov, sizeof * *iov * (i + 2));
	if (*iov == NULL)
	{
		*iovlen = -1;
		return;
	}

	(*iov)[i].iov_base = strdup(name);
	(*iov)[i].iov_len = strlen(name) + 1;
	++i;

	(*iov)[i].iov_base = (void*)val;
	if (len == (size_t)-1)
	{
		if (val != NULL)
			len = strlen((const char*)val) + 1;
		else
			len = 0;
	}
	(*iov)[i].iov_len = (int)len;

	*iovlen = ++i;
}

int nmount(struct iovec* iov, uint32_t niov, int flags)
{
	return syscall(378, iov, niov, flags);
}

int mount(const char* type, const char* dir, int flags, void* data)
{
	return syscall(21, type, dir, flags, data);
}

int unmount(const char* dir, int flags)
{
	return syscall(22, dir, flags);
}

int MountLargeFs(const char* device, const char* mountpoint, const char* fstype, const char* mode, unsigned int flags)
{
	struct iovec* iov = NULL;
	int iovlen = 0;

	BuildIovec(&iov, &iovlen, "fstype", fstype, -1);
	BuildIovec(&iov, &iovlen, "fspath", mountpoint, -1);
	BuildIovec(&iov, &iovlen, "from", device, -1);
	BuildIovec(&iov, &iovlen, "large", "yes", -1);
	BuildIovec(&iov, &iovlen, "timezone", "static", -1);
	BuildIovec(&iov, &iovlen, "async", "", -1);
	BuildIovec(&iov, &iovlen, "ignoreacl", "", -1);

	if (mode)
	{
		BuildIovec(&iov, &iovlen, "dirmask", mode, -1);
		BuildIovec(&iov, &iovlen, "mask", mode, -1);
	}

	return nmount(iov, iovlen, flags);
}

int RemountReadWrite(const char* device, const char* dir)
{
	return MountLargeFs(device, dir, "exfatfs", "511", MNT_UPDATE);
}
