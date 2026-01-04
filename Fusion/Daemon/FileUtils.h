#pragma once

#define	MNT_UPDATE	0x00010000 /* not real mount, just update */
#define	MNT_FORCE	0x00080000 /* force unmount or readonly */

void BuildIovec(OrbisKernelIovec** iov, int* iovlen, const char* name, const void* val, size_t len);
int nmount(struct OrbisKernelIovec* iov, uint32_t niov, int flags);
int mount(const char* type, const char* dir, int flags, void* data);
int unmount(const char* dir, int flags);
int MountLargeFs(const char* device, const char* mountpoint, const char* fstype, const char* mode, unsigned int flags);
int MountTmpfs(const char* mountpoint);
int MountNullfs(const char* source, const char* target);
int RemountReadWrite(const char* device, const char* dir);
void DisableUpdates();