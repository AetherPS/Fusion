#pragma once

bool FileExist(const char* path);
bool CreateDirectory(const char* path);
int RemoveFile(const char* path);
int WriteFile(const char* path, uint8_t* data, size_t len);