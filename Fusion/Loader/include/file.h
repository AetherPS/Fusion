#pragma once

#define	MNT_UPDATE	0x00010000 /* not real mount, just update */
#define	MNT_FORCE	0x00080000 /* force unmount or readonly */

bool FileExist(const char* path);
bool CreateDirectory(const char* path);
int RemoveFile(const char* path);
int WriteFile(const char* path, uint8_t* data, size_t len);

void BuildIovec(struct iovec** iov, int* iovlen, const char* name, const void* val, size_t len);
int nmount(struct iovec* iov, uint32_t niov, int flags);
int mount(const char* type, const char* dir, int flags, void* data);
int unmount(const char* dir, int flags);
int MountLargeFs(const char* device, const char* mountpoint, const char* fstype, const char* mode, unsigned int flags);
int RemountReadWrite(const char* device, const char* dir);