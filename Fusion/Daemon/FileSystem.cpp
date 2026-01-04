#include "stdafx.h"
#include "FileSystem.h"

bool FileSystem::Exists(const std::string filePath)
{
	int fd = sceKernelOpen(filePath.c_str(), O_RDONLY, 0);
	if (fd < 0)
	{
		// File does not exist or cannot be opened
		return false;
	}

	sceKernelClose(fd);
	return true;
}

size_t FileSystem::GetSize(const std::string filePath)
{
	int fd = sceKernelOpen(filePath.c_str(), O_RDONLY, 0);
	if (fd < 0)
	{
		return fd;
	}

	OrbisKernelStat stat;
	auto res = sceKernelFstat(fd, &stat);
	if (res != 0)
	{
		sceKernelClose(fd);
		return res;
	}

	sceKernelClose(fd);
	return static_cast<size_t>(stat.st_size);
}

int FileSystem::Rename(std::string from, std::string to)
{
	int res = sceKernelRename(from.c_str(), to.c_str());

	if (res != 0)
	{
		return res;
	}

	return 0;
}

int FileSystem::Remove(std::string filePath)
{
	int res = sceKernelUnlink(filePath.c_str());
	if (res != 0)
	{
		return res;
	}

	return 0;
}

bool FileSystem::IsDirectoryEmpty(const std::string& dirPath)
{
	int fd = sceKernelOpen(dirPath.c_str(), O_RDONLY | O_DIRECTORY, 0);
	if (fd < 0)
	{
		return false;
	}

	const size_t bufferSize = 0x4000;
	char buffer[bufferSize];

	ssize_t bytesRead = sceKernelGetdents(fd, buffer, bufferSize);
	if (bytesRead < 0)
	{
		sceKernelClose(fd);
		return false;
	}

	bool isEmpty = true;
	size_t offset = 0;
	while (offset < static_cast<size_t>(bytesRead))
	{
		dirent* entry = reinterpret_cast<dirent*>(buffer + offset);
		offset += entry->d_reclen;

		// Skip "." and ".."
		if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
		{
			isEmpty = false;
			break;
		}
	}

	sceKernelClose(fd);
	return isEmpty;
}

std::vector<dirent> FileSystem::GetDentList(const std::string& basePath)
{
	std::vector<dirent> entries;
	int fd = sceKernelOpen(basePath.c_str(), O_RDONLY | O_DIRECTORY, 0);
	if (fd < 0)
	{
		return entries;
	}

	const size_t bufferSize = 0x4000;
	char buffer[bufferSize];

	ssize_t bytesRead;
	while ((bytesRead = sceKernelGetdents(fd, buffer, bufferSize)) > 0)
	{
		size_t offset = 0;
		while (offset < static_cast<size_t>(bytesRead))
		{
			dirent* _dirent = reinterpret_cast<dirent*>(buffer + offset);
			offset += _dirent->d_reclen;

			// Add a copy to vector
			entries.push_back(*_dirent);
		}
	}

	sceKernelClose(fd);
	return entries;
}

bool FileSystem::MakeDir(const char* Dir, ...)
{
	char buffer[PATH_MAX] = { 0 };
	va_list args;
	va_start(args, Dir);
	vsnprintf(buffer, sizeof(buffer), Dir, args);
	va_end(args);

	int res = sceKernelMkdir(buffer, 0777);
	if (res != 0)
	{
		// Directory already exists
		if (res == 0x80020011)
		{
			klog("%s: Directory already exists: %s", __FUNCTION__, buffer);
			return true;
		}

		return false;
	}

	return true;
}

void FileSystem::CopyFile(const char* File, const char* Destination)
{
	int src = -1, dst = -1;
	OrbisKernelStat Stats;

	// Open source file
	src = sceKernelOpen(File, O_RDONLY, 0);
	if (src < 0)
	{
		return;
	}

	// Open destination file
	dst = sceKernelOpen(Destination, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (dst < 0)
	{
		sceKernelClose(src);
		return;
	}

	// Get file stats
	int fstatResult = sceKernelFstat(src, &Stats);
	if (fstatResult < 0)
	{
		sceKernelClose(src);
		sceKernelClose(dst);
		return;
	}

	if (Stats.st_size <= 0)
	{
		sceKernelClose(src);
		sceKernelClose(dst);
		return;
	}

	// Allocate buffer
	char* FileData = (char*)malloc(Stats.st_size);
	if (!FileData)
	{
		sceKernelClose(src);
		sceKernelClose(dst);
		return;
	}

	// Read loop
	ssize_t totalRead = 0;
	while (totalRead < Stats.st_size)
	{
		ssize_t r = sceKernelRead(src, FileData + totalRead, Stats.st_size - totalRead);
		if (r <= 0)
		{
			free(FileData);
			sceKernelClose(src);
			sceKernelClose(dst);
			return;
		}
		totalRead += r;
	}

	// Write loop
	ssize_t totalWritten = 0;
	while (totalWritten < totalRead)
	{
		ssize_t w = sceKernelWrite(dst, FileData + totalWritten, totalRead - totalWritten);
		if (w <= 0)
		{
			free(FileData);
			sceKernelClose(src);
			sceKernelClose(dst);
			return;
		}
		totalWritten += w;
	}

	free(FileData);
	sceKernelClose(src);
	sceKernelClose(dst);
}


int FileSystem::Read(const std::string& filePath, void* data, size_t length)
{
	if (!data) return -1;

	int fd = sceKernelOpen(filePath.c_str(), O_RDONLY, 0);
	if (fd < 0)
	{
		return fd;
	}

	size_t totalRead = 0;
	while (totalRead < length)
	{
		ssize_t r = sceKernelRead(fd, static_cast<char*>(data) + totalRead, length - totalRead);
		if (r < 0)
		{
			sceKernelClose(fd);
			return static_cast<int>(r);
		}
		else if (r == 0) // EOF
		{
			break;
		}

		totalRead += r;
	}

	int closeResult = sceKernelClose(fd);
	if (closeResult < 0)
	{
		return closeResult;
	}

	return 0;
}

int FileSystem::Write(const std::string& filePath, const void* data, size_t length)
{
	int fd = sceKernelOpen(filePath.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (fd < 0)
	{
		return fd;
	}

	size_t totalWritten = 0;
	while (totalWritten < length)
	{
		ssize_t w = sceKernelWrite(fd, static_cast<const char*>(data) + totalWritten, length - totalWritten);
		if (w <= 0)
		{
			sceKernelClose(fd);
			return (w < 0) ? static_cast<int>(w) : -1;
		}
		totalWritten += w;
	}

	int closeResult = sceKernelClose(fd);
	if (closeResult < 0)
	{
		return closeResult;
	}

	return 0;
}