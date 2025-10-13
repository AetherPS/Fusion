#pragma once

#include <stdint.h>

#define NULL 0

#define FALSE 0
#define TRUE 1

#define PAGE_SIZE (16 * 1024)

#define BIT(n) (1 << (n))

typedef uint64_t size_t;
typedef int64_t ssize_t;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef float float32;
typedef double float64;
typedef int8_t bool;

typedef volatile uint8_t vuint8;
typedef volatile uint16_t vuint16;
typedef volatile uint32_t vuint32;
typedef volatile uint64_t vuint64;

typedef volatile int8_t vint8;
typedef volatile int16_t vint16;
typedef volatile int32_t vint32;
typedef volatile int64_t vint64;

typedef volatile float32 vfloat32;
typedef volatile float64 vfloat64;

typedef uint8_t byte;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef volatile u8 vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;

typedef volatile s8 vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;

//typedef uint32_t __dev_t; 
typedef uint32_t blksize_t;
typedef int64_t  blkcnt_t;
typedef uint32_t dev_t;
typedef uint32_t fflags_t;
typedef uint32_t gid_t;
typedef uint32_t ino_t;
typedef uint16_t mode_t;
typedef uint16_t nlink_t;
typedef int64_t  off_t;
typedef uint32_t uid_t;
typedef int64_t time_t;
typedef long suseconds_t;

typedef unsigned int SceKernelUseconds;

typedef int vm_prot_t;
typedef uint64_t vm_offset_t;
typedef uint64_t vm_map_t;
typedef uint64_t vm_size_t;
typedef unsigned long size_t;
#define	VM_PROT_NONE	((vm_prot_t) 0x00)
#define VM_PROT_READ	((vm_prot_t) 0x01)	/* read permission */
#define VM_PROT_WRITE	((vm_prot_t) 0x02)	/* write permission */
#define VM_PROT_EXECUTE	((vm_prot_t) 0x04)	/* execute permission */
#define VM_PROT_DEFAULT	(VM_PROT_READ|VM_PROT_WRITE)
#define VM_PROT_ALL			(VM_PROT_READ | VM_PROT_WRITE | VM_PROT_EXECUTE)
#define VM_PROT_NO_CHANGE	((vm_prot_t) 0x08)
#define VM_PROT_COPY		((vm_prot_t) 0x10)
#define VM_PROT_WANTS_COPY	((vm_prot_t) 0x10)

#define JOIN_HELPER(x, y) x##y
#define JOIN(x, y) JOIN_HELPER(x, y)

#define TYPE_PAD(size) char JOIN(_pad_, __COUNTER__)[size]
#define TYPE_VARIADIC_BEGIN(name) name { union {
#define TYPE_BEGIN(name, size) name { union { TYPE_PAD(size)
#define TYPE_END(...) }; } __VA_ARGS__
#define TYPE_FIELD(field, offset) struct { TYPE_PAD(offset); field; }

#define TYPE_CHECK_SIZE(name, size) \
  _Static_assert(sizeof(name) == (size), "Size of " #name " != " #size)

#define TYPE_CHECK_FIELD_OFFSET(name, member, offset) \
  _Static_assert(offsetof(name, member) == (offset), "Offset of " #name "." #member " != " #offset)

#define TYPE_CHECK_FIELD_SIZE(name, member, size) \
  _Static_assert(sizeof(((name*)0)->member) == (size), "Size of " #name "." #member " != " #size)

struct timespec {
	time_t tv_sec;
	long tv_nsec;
};

struct timeval {
	time_t tv_sec;
	suseconds_t tv_usec;
};

struct tm {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
};

struct lock_object {
	const char* lo_name;
	uint32_t lo_flags;
	uint32_t lo_data;
	void* lo_witness;
};

struct mtx {
	struct lock_object lock_object;
	volatile void* mtx_lock;
};

struct sx {
	struct lock_object lock_object;
	volatile uintptr_t sx_lock;
};

struct auditinfo_addr {
	uint8_t useless[184];
};

struct ucred {
	uint32_t cr_ref;					// reference count		0x0000
	uint32_t cr_uid;					// effective user id	0x0004
	uint32_t cr_ruid;					// real user id			0x0008
	uint32_t useless2;					// 						0x000C
	uint32_t useless3;					//
	uint32_t cr_rgid;					// real group id
	uint32_t useless4;					//
	void* useless5;						//
	void* useless6;						//
	void* cr_prison;					// jail(2)				0x0030
	void* useless7;						//
	uint32_t useless8;					//
	void* useless9[2];					//
	void* useless10;					//
	struct auditinfo_addr cr_audit;		//
	uint32_t* cr_groups;				// groups
	uint32_t useless12;					//
};

struct filedesc {
	void* useless1[3];
	void* fd_rdir;
	void* fd_jdir;
};

struct stat {
	__dev_t   st_dev;		/* inode's device */
	ino_t	  st_ino;		/* inode's number */
	mode_t	  st_mode;		/* inode protection mode */
	nlink_t	  st_nlink;		/* number of hard links */
	uid_t	  st_uid;		/* user ID of the file's owner */
	gid_t	  st_gid;		/* group ID of the file's group */
	__dev_t   st_rdev;		/* device type */
	struct	timespec st_atim;	/* time of last access */
	struct	timespec st_mtim;	/* time of last data modification */
	struct	timespec st_ctim;	/* time of last file status change */
	off_t	  st_size;		/* file size, in bytes */
	blkcnt_t st_blocks;		/* blocks allocated for file */
	blksize_t st_blksize;		/* optimal blocksize for I/O */
	fflags_t  st_flags;		/* user defined flags for file */
	__uint32_t st_gen;		/* file generation number */
	__int32_t st_lspare;
	struct timespec st_birthtim;	/* time of file creation */
	/*
	 * Explicitly pad st_birthtim to 16 bytes so that the size of
	 * struct stat is backwards compatible.  We use bitfields instead
	 * of an array of chars so that this doesn't require a C99 compiler
	 * to compile if the size of the padding is 0.  We use 2 bitfields
	 * to cover up to 64 bits on 32-bit machines.  We assume that
	 * CHAR_BIT is 8...
	 */
	unsigned int :(8 / 2)* (16 - (int)sizeof(struct timespec));
	unsigned int :(8 / 2)* (16 - (int)sizeof(struct timespec));
};

typedef struct stat SceKernelStat;
typedef mode_t SceKernelMode;

#define SCE_KERNEL_NAME_MAX        255
#define SCE_KERNEL_PATH_MAX        1024
#define SCE_KERNEL_IOV_MAX         1024
#define SCE_KERNEL_MAXNAMLEN       255

#define SCE_KERNEL_O_RDONLY        0x0000
#define SCE_KERNEL_O_WRONLY        0x0001 
#define SCE_KERNEL_O_RDWR          0x0002

#define SCE_KERNEL_O_CREAT		   0x0200

TYPE_BEGIN(struct proc, 0x800); // XXX: random, don't use directly without fixing it
TYPE_FIELD(struct proc* p_forw, 0);
TYPE_FIELD(struct ucred* p_ucred, 0x40);
TYPE_FIELD(struct filedesc* p_fd, 0x48);
TYPE_FIELD(int pid, 0xB0);
TYPE_FIELD(char p_comm[32], 0x454);
TYPE_END();

TYPE_BEGIN(struct thread, 0x3D8); // XXX: random, don't use directly without fixing it
TYPE_FIELD(struct proc* td_proc, 8);
TYPE_FIELD(struct ucred* td_ucred, 0x130);
TYPE_FIELD(char td_name[32], 0x284);
TYPE_END();

#define CR0_WP (1 << 16) // write protect

static inline __attribute__((always_inline)) uint64_t __readmsr(uint32_t __register) {
	uint32_t __edx, __eax;

	__asm__ volatile (
		"rdmsr"
		: "=d"(__edx),
		"=a"(__eax)
		: "c"(__register)
		);

	return (((uint64_t)__edx) << 32) | (uint64_t)__eax;
}

static inline __attribute__((always_inline)) uint64_t __readcr0(void) {
	uint64_t cr0;

	__asm__ volatile (
		"movq %0, %%cr0"
		: "=r" (cr0)
		: : "memory"
		);

	return cr0;
}
static inline __attribute__((always_inline)) void __writecr0(uint64_t cr0) {
	__asm__ volatile (
		"movq %%cr0, %0"
		: : "r" (cr0)
		: "memory"
		);
}
