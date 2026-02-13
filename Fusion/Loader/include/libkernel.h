#pragma once

extern "C"
{
#define SIGKILL 9

/* Memory protection flags */
#define SCE_KERNEL_PROT_CPU_READ   0x01
#define SCE_KERNEL_PROT_CPU_WRITE  0x02
#define SCE_KERNEL_PROT_CPU_EXEC   0x04
#define SCE_KERNEL_PROT_GPU_READ   0x10
#define SCE_KERNEL_PROT_GPU_WRITE  0x20

/* VM protection flags (BSD-style) */
#define VM_PROT_NONE    0x00
#define VM_PROT_READ    0x01
#define VM_PROT_WRITE   0x02
#define VM_PROT_EXECUTE 0x04
#define VM_PROT_ALL     (VM_PROT_READ | VM_PROT_WRITE | VM_PROT_EXECUTE)

/* File open flags */
#define SCE_KERNEL_O_RDONLY   0x0000
#define SCE_KERNEL_O_WRONLY   0x0001
#define SCE_KERNEL_O_RDWR     0x0002
#define SCE_KERNEL_O_NONBLOCK 0x0004
#define SCE_KERNEL_O_APPEND   0x0008
#define SCE_KERNEL_O_CREAT    0x0200
#define SCE_KERNEL_O_TRUNC    0x0400
#define SCE_KERNEL_O_EXCL     0x0800
#define SCE_KERNEL_O_DIRECT   0x00010000
#define SCE_KERNEL_O_DIRECTORY 0x00020000
#define SCE_KERNEL_O_FSYNC    0x00000080

/* sysctl names */
#define CTL_KERN    1
#define KERN_PROC   14
#define KERN_PROC_PID   1
#define KERN_PROC_PATHNAME  12

	int kill(int pid, int how);
	int sysctl(const int* name, unsigned int namelen, void* oldp, size_t* oldlenp, const void* newp, size_t newlen);
	int sysctlbyname(const char* name, void* oldp, size_t* oldlenp, const void* newp, size_t newlen);
	int ioctl(int fd, unsigned long request, ...);
	int sceKernelOpen(const char* path, int flags, int mode);
	int sceKernelClose(int fd);
	ssize_t sceKernelRead(int fd, void* buf, size_t nbytes);
	ssize_t sceKernelWrite(int fd, const void* buf, size_t nbytes);
	off_t sceKernelLseek(int fd, off_t offset, int whence);
	int sceKernelMkdir(const char* path, int mode);
	int sceKernelRmdir(const char* path);
	int sceKernelUnlink(const char* path);
	int sceKernelRename(const char* from, const char* to);
	int sceKernelStat(const char* path, void* sb);
	int sceKernelFstat(int fd, void* sb);
	int scePthreadCreate(void* thread, const void* attr, void* (*entry)(void*), void* arg, const char* name);
	void scePthreadExit(void* value);
	int scePthreadJoin(void* thread, void** value);
	int scePthreadDetach(void* thread);
	uint64_t sceKernelGetProcessTime(void);
	unsigned int sceKernelSleep(unsigned int seconds);
	int sceKernelUsleep(unsigned int microseconds);
	int sceKernelGettimeofday(void* tp);
	int sceKernelLoadStartModule(const char* name, size_t argc, const void* argv, uint32_t flags, const void* option, int* result);
	int sceKernelStopUnloadModule(int handle, size_t argc, const void* argv, uint32_t flags, const void* option, int* result);
	int sceKernelDlsym(int handle, const char* symbol, void** addrp);
	void sceKernelDebugOutText(int channel, const char* text);
	int sceKernelGetSystemSwVersion(void* data);
	int sceKernelGetRandomNumber(void* buf, size_t size);
	int sceKernelGetProcessName(int pid, char* out);
}