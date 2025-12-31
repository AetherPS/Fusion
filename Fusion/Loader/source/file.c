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