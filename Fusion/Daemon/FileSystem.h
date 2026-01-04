#pragma once

class FileSystem
{
public:
	static bool Exists(const std::string filePath);
	static size_t GetSize(const std::string filePath);
	static int Rename(std::string from, std::string to);
	static int Remove(std::string filePath);
	static bool IsDirectoryEmpty(const std::string& dirPath);
	static std::vector<dirent> GetDentList(const std::string& basePath);
	static bool MakeDir(const char* Dir, ...);
	static void CopyFile(const char* File, const char* Destination);

	static int Read(const std::string& filePath, void* data, size_t length);
	static int Write(const std::string& filePath, const void* data, size_t length);

private:

};