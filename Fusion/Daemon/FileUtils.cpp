#include "stdafx.h"
#include "FileUtils.h"

void BuildIovec(OrbisKernelIovec** iov, int* iovlen, const char* name, const void* val, size_t len)
{
	int i;

	if (*iovlen < 0)
		return;

	i = *iovlen;
	*iov = (OrbisKernelIovec*)realloc(*iov, sizeof * *iov * (i + 2));
	if (*iov == NULL)
	{
		*iovlen = -1;
		return;
	}

	(*iov)[i].base = strdup(name);
	(*iov)[i].len = strlen(name) + 1;
	++i;

	(*iov)[i].base = (void*)val;
	if (len == (size_t)-1)
	{
		if (val != NULL)
			len = strlen((const char*)val) + 1;
		else
			len = 0;
	}
	(*iov)[i].len = (int)len;

	*iovlen = ++i;
}

int nmount(struct OrbisKernelIovec* iov, uint32_t niov, int flags)
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
	struct OrbisKernelIovec* iov = NULL;
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

int MountTmpfs(const char* mountpoint)
{
	struct OrbisKernelIovec* iov = NULL;
	int iovlen = 0;

	BuildIovec(&iov, &iovlen, "fstype", "tmpfs", -1);
	BuildIovec(&iov, &iovlen, "fspath", mountpoint, -1);
	BuildIovec(&iov, &iovlen, "from", "tmpfs", -1);

	return nmount(iov, iovlen, 0);
}

int MountNullfs(const char* source, const char* target)
{
	struct OrbisKernelIovec* iov = NULL;
	int iovlen = 0;

	BuildIovec(&iov, &iovlen, "fstype", "nullfs", -1);
	BuildIovec(&iov, &iovlen, "fspath", target, -1);
	BuildIovec(&iov, &iovlen, "from", source, -1);

	return nmount(iov, iovlen, 0);
}

int RemountReadWrite(const char* device, const char* dir)
{
	return MountLargeFs(device, dir, "exfatfs", "511", MNT_UPDATE);
}

void DisableUpdates()
{
	sceKernelUnlink("/update/PS4UPDATE.PUP.net.temp");
	sceKernelRmdir("/update/PS4UPDATE.PUP.net.temp");
	sceKernelMkdir("/update/PS4UPDATE.PUP.net.temp", 777);

	sceKernelUnlink("/update/PS4UPDATE.PUP");
	sceKernelRmdir("/update/PS4UPDATE.PUP");
	sceKernelMkdir("/update/PS4UPDATE.PUP", 777);
}