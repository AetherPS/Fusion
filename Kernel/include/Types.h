#pragma once
#include "Common.h"
#include "elf.h"

#define PAGE_SIZE 0x4000
#define	NULL	0

#define	QMD_TRACE_ELEM(elem)
#define	QMD_TRACE_HEAD(head)
#define	QMD_SAVELINK(name, link)
#define	TRACEBUF
#define	TRASHIT(x)

#define	STAILQ_HEAD(name, type)						\
struct name {								\
	struct type *stqh_first;/* first element */			\
	struct type **stqh_last;/* addr of last next element */		\
}

#define	SLIST_HEAD(name, type)						\
struct name {								\
	struct type *slh_first;	/* first element */			\
}

#define SLIST_ENTRY(type)                               \
struct {                                                \
    struct type *sle_next;  /* Pointer to the next item */  \
}

#define	SLIST_FIRST(head)	((head)->slh_first)
#define	SLIST_NEXT(elm, field)	((elm)->field.sle_next)

#define	SLIST_FOREACH(var, head, field)					\
	for ((var) = SLIST_FIRST((head));				\
	    (var);							\
	    (var) = SLIST_NEXT((var), field))

#define	TAILQ_ENTRY(type)						\
struct {								\
	struct type *tqe_next;	/* next element */			\
	struct type **tqe_prev;	/* address of previous next element */	\
	TRACEBUF							\
}

#define	TAILQ_HEAD(name, type)						\
struct name {								\
	struct type *tqh_first;	/* first element */			\
	struct type **tqh_last;	/* addr of last next element */		\
	TRACEBUF							\
}

#define	LIST_ENTRY(type)						\
struct {								\
	struct type *le_next;	/* next element */			\
	struct type **le_prev;	/* address of previous next element */	\
}

#define	LIST_HEAD(name, type)						\
struct name {								\
	struct type *lh_first;	/* first element */			\
}

#define	LIST_FIRST(head)	((head)->lh_first)
#define	LIST_NEXT(elm, field)	((elm)->field.le_next)

#define	LIST_FOREACH(var, head, field)					\
	for ((var) = LIST_FIRST((head));				\
	    (var);							\
	    (var) = LIST_NEXT((var), field))

#define	TAILQ_EMPTY(head)	((head)->tqh_first == NULL)
#define	TAILQ_FIRST(head)	((head)->tqh_first)
#define	TAILQ_NEXT(elm, field) ((elm)->field.tqe_next)

#define	TAILQ_FOREACH(var, head, field)					\
	for ((var) = TAILQ_FIRST((head));				\
	    (var);							\
	    (var) = TAILQ_NEXT((var), field))

#define	EPERM		1		/* Operation not permitted */
#define	ENOENT		2		/* No such file or directory */
#define	ESRCH		3		/* No such process */
#define	EINTR		4		/* Interrupted system call */
#define	EIO		5		/* Input/output error */
#define	ENXIO		6		/* Device not configured */
#define	E2BIG		7		/* Argument list too long */
#define	ENOEXEC		8		/* Exec format error */
#define	EBADF		9		/* Bad file descriptor */
#define	ECHILD		10		/* No child processes */
#define	EDEADLK		11		/* Resource deadlock avoided */
/* 11 was EAGAIN */
#define	ENOMEM		12		/* Cannot allocate memory */
#define	EACCES		13		/* Permission denied */
#define	EFAULT		14		/* Bad address */
#ifndef _POSIX_SOURCE
#define	ENOTBLK		15		/* Block device required */
#endif
#define	EBUSY		16		/* Device busy */
#define	EEXIST		17		/* File exists */
#define	EXDEV		18		/* Cross-device link */
#define	ENODEV		19		/* Operation not supported by device */
#define	ENOTDIR		20		/* Not a directory */
#define	EISDIR		21		/* Is a directory */
#define	EINVAL		22		/* Invalid argument */
#define	ENFILE		23		/* Too many open files in system */
#define	EMFILE		24		/* Too many open files */
#define	ENOTTY		25		/* Inappropriate ioctl for device */
#ifndef _POSIX_SOURCE
#define	ETXTBSY		26		/* Text file busy */
#endif
#define	EFBIG		27		/* File too large */
#define	ENOSPC		28		/* No space left on device */
#define	ESPIPE		29		/* Illegal seek */
#define	EROFS		30		/* Read-only filesystem */
#define	EMLINK		31		/* Too many links */
#define	EPIPE		32		/* Broken pipe */

/* math software */
#define	EDOM		33		/* Numerical argument out of domain */
#define	ERANGE		34		/* Result too large */

/* non-blocking and interrupt i/o */
#define	EAGAIN		35		/* Resource temporarily unavailable */
#ifndef _POSIX_SOURCE
#define	EWOULDBLOCK	EAGAIN		/* Operation would block */
#define	EINPROGRESS	36		/* Operation now in progress */
#define	EALREADY	37		/* Operation already in progress */

/* ipc/network software -- argument errors */
#define	ENOTSOCK	38		/* Socket operation on non-socket */
#define	EDESTADDRREQ	39		/* Destination address required */
#define	EMSGSIZE	40		/* Message too long */
#define	EPROTOTYPE	41		/* Protocol wrong type for socket */
#define	ENOPROTOOPT	42		/* Protocol not available */
#define	EPROTONOSUPPORT	43		/* Protocol not supported */
#define	ESOCKTNOSUPPORT	44		/* Socket type not supported */
#define	EOPNOTSUPP	45		/* Operation not supported */
#define	ENOTSUP		EOPNOTSUPP	/* Operation not supported */
#define	EPFNOSUPPORT	46		/* Protocol family not supported */
#define	EAFNOSUPPORT	47		/* Address family not supported by protocol family */
#define	EADDRINUSE	48		/* Address already in use */
#define	EADDRNOTAVAIL	49		/* Can't assign requested address */

/* ipc/network software -- operational errors */
#define	ENETDOWN	50		/* Network is down */
#define	ENETUNREACH	51		/* Network is unreachable */
#define	ENETRESET	52		/* Network dropped connection on reset */
#define	ECONNABORTED	53		/* Software caused connection abort */
#define	ECONNRESET	54		/* Connection reset by peer */
#define	ENOBUFS		55		/* No buffer space available */
#define	EISCONN		56		/* Socket is already connected */
#define	ENOTCONN	57		/* Socket is not connected */
#define	ESHUTDOWN	58		/* Can't send after socket shutdown */
#define	ETOOMANYREFS	59		/* Too many references: can't splice */
#define	ETIMEDOUT	60		/* Operation timed out */
#define	ECONNREFUSED	61		/* Connection refused */

#define	ELOOP		62		/* Too many levels of symbolic links */
#endif /* _POSIX_SOURCE */
#define	ENAMETOOLONG	63		/* File name too long */

/* should be rearranged */
#ifndef _POSIX_SOURCE
#define	EHOSTDOWN	64		/* Host is down */
#define	EHOSTUNREACH	65		/* No route to host */
#endif /* _POSIX_SOURCE */
#define	ENOTEMPTY	66		/* Directory not empty */

/* quotas & mush */
#ifndef _POSIX_SOURCE
#define	EPROCLIM	67		/* Too many processes */
#define	EUSERS		68		/* Too many users */
#define	EDQUOT		69		/* Disc quota exceeded */

/* Network File System */
#define	ESTALE		70		/* Stale NFS file handle */
#define	EREMOTE		71		/* Too many levels of remote in path */
#define	EBADRPC		72		/* RPC struct is bad */
#define	ERPCMISMATCH	73		/* RPC version wrong */
#define	EPROGUNAVAIL	74		/* RPC prog. not avail */
#define	EPROGMISMATCH	75		/* Program version wrong */
#define	EPROCUNAVAIL	76		/* Bad procedure for program */
#endif /* _POSIX_SOURCE */

#define	ENOLCK		77		/* No locks available */
#define	ENOSYS		78		/* Function not implemented */

#ifndef _POSIX_SOURCE
#define	EFTYPE		79		/* Inappropriate file type or format */
#define	EAUTH		80		/* Authentication error */
#define	ENEEDAUTH	81		/* Need authenticator */
#define	EIDRM		82		/* Identifier removed */
#define	ENOMSG		83		/* No message of desired type */
#define	EOVERFLOW	84		/* Value too large to be stored in data type */
#define	ECANCELED	85		/* Operation canceled */
#define	EILSEQ		86		/* Illegal byte sequence */
#define	ENOATTR		87		/* Attribute not found */

#define	EDOOFUS		88		/* Programming error */
#endif /* _POSIX_SOURCE */

#define	EBADMSG		89		/* Bad message */
#define	EMULTIHOP	90		/* Multihop attempted */
#define	ENOLINK		91		/* Link has been severed */
#define	EPROTO		92		/* Protocol error */


/*
 * Basic types upon which most other types are built.
 */
typedef	signed char		__int8_t;
typedef	unsigned char		__uint8_t;
typedef	short			__int16_t;
typedef	unsigned short		__uint16_t;
typedef	int			__int32_t;
typedef	unsigned int		__uint32_t;

/*
 * Standard type definitions.
 */
typedef	__int64_t	__critical_t;
typedef	double		__double_t;
typedef	float		__float_t;
typedef	__int64_t	__intfptr_t;
typedef	__int64_t	__intmax_t;
typedef	__int64_t	__intptr_t;
typedef	__int32_t	__int_fast8_t;
typedef	__int32_t	__int_fast16_t;
typedef	__int32_t	__int_fast32_t;
typedef	__int64_t	__int_fast64_t;
typedef	__int8_t	__int_least8_t;
typedef	__int16_t	__int_least16_t;
typedef	__int32_t	__int_least32_t;
typedef	__int64_t	__int_least64_t;
typedef	__int64_t	__ptrdiff_t;		/* ptr1 - ptr2 */
typedef	__int64_t	__register_t;
typedef	__int64_t	__segsz_t;		/* segment size (in pages) */
typedef	unsigned long long	__size_t;		/* sizeof() */
typedef	__int64_t	__time_t;		/* time()... */
typedef	__uint64_t	__uintfptr_t;
typedef	__uint64_t	__uintmax_t;
typedef	__uint64_t	__uintptr_t;
typedef	__uint32_t	__uint_fast8_t;
typedef	__uint32_t	__uint_fast16_t;
typedef	__uint32_t	__uint_fast32_t;
typedef	__uint64_t	__uint_fast64_t;
typedef	__uint8_t	__uint_least8_t;
typedef	__uint16_t	__uint_least16_t;
typedef	__uint32_t	__uint_least32_t;
typedef	__uint64_t	__uint_least64_t;
typedef	__uint64_t	__u_register_t;
typedef	__uint64_t	__vm_offset_t;
typedef	__int64_t	__vm_ooffset_t;
typedef	__uint64_t	__vm_paddr_t;
typedef	__uint64_t	__vm_pindex_t;
typedef	__uint64_t	__vm_size_t;

typedef	__int64_t	__blkcnt_t;	/* file block count */
typedef	__int32_t	__clockid_t;	/* clock_gettime()... */
typedef	__uint64_t	__cap_rights_t;	/* capability rights */
typedef	__uint32_t	__fflags_t;	/* file flags */
typedef	__uint64_t	__fsblkcnt_t;
typedef	__uint64_t	__fsfilcnt_t;
typedef	__int32_t	__lwpid_t;	/* Thread ID (a.k.a. LWP) */
typedef	int		__accmode_t;	/* access permissions */
typedef	int		__nl_item;
typedef	__int64_t	__off_t;	/* file offset */
typedef	__int32_t	__pid_t;	/* process [group] */
/* signed, because of legacy code */
/* that uses -1 for RLIM_INFINITY */
typedef	__uint8_t	__sa_family_t;
typedef	__uint32_t	__socklen_t;
typedef	long		__suseconds_t;	/* microseconds (signed) */
typedef	__uint32_t	__uid_t;
typedef	unsigned int	__useconds_t;	/* microseconds (unsigned) */
typedef	int		__cpuwhich_t;	/* which parameter for cpuset. */
typedef	int		__cpulevel_t;	/* level parameter for cpuset. */
typedef int		__cpusetid_t;	/* cpuset identifier. */

typedef	int		__ct_rune_t;	/* arg type for ctype funcs */
typedef	__ct_rune_t	__rune_t;	/* rune_t (see above) */
typedef	__ct_rune_t	__wchar_t;	/* wchar_t (see above) */
typedef	__ct_rune_t	__wint_t;	/* wint_t (see above) */

typedef	__vm_offset_t	vm_offset_t;
typedef	__vm_ooffset_t	vm_ooffset_t;
typedef	__vm_paddr_t	vm_paddr_t;
typedef	__vm_pindex_t	vm_pindex_t;
typedef	__vm_size_t	vm_size_t;

typedef	__uint32_t	__fixpt_t;	/* fixed point number */

typedef	__ssize_t	ssize_t;
typedef	long long	off_t;		/* file offset */
typedef	__size_t	size_t;
typedef	char* caddr_t;	/* core address */
typedef	uint16_t	au_event_t;
typedef uint32_t gid_t;
typedef uint32_t uid_t;
typedef	__uint16_t	mode_t;

#define	M_NOWAIT	0x0001		/* do not block */
#define	M_WAITOK	0x0002		/* ok to block */
#define	M_ZERO		0x0100		/* bzero the allocation */
#define	M_NOVM		0x0200		/* don't ask VM for pages */
#define	M_EXEC	0x4000	/* allocate executable space */
#define	M_USE_RESERVE	0x0400		/* can alloc out of reserve memory */
#define	M_NODUMP	0x0800	

#define	M_MAGIC		877983977	/* time when first defined :-) */

#define	O_CREAT		0x0200		/* create if nonexistent */
#define	O_TRUNC		0x0400		/* truncate to zero length */
#define	O_EXCL		0x0800		/* error if already exists */
#define	O_RDONLY	0x0000		/* open for reading only */
#define	O_WRONLY	0x0001		/* open for writing only */
#define	O_RDWR		0x0002		/* open for reading and writing */
#define	O_ACCMODE	0x0003		/* mask for above modes */

struct au_tid_addr {
	uint32_t at_port;
	uint32_t at_type;
	uint32_t at_addr[4];
};
typedef	struct au_tid_addr	au_tid_addr_t;

struct au_mask {
	unsigned int am_success;
	unsigned int am_failure;
};
typedef	struct au_mask	au_mask_t;

struct auditinfo_addr {
	uint32_t ai_auid;
	au_mask_t ai_mask;
	au_tid_addr_t ai_termid;
	int	ai_asid;
	uint64_t ai_flags;
};

struct ucred {
	uint32_t cr_ref;
	uint32_t cr_uid;
	uint32_t cr_ruid;
	uint32_t cr_svuid;
	int	cr_ngroups;
	uint32_t cr_rgid;
	uint32_t cr_svgid;
	void* cr_uidinfo;
	void* cr_ruidinfo;
	void* cr_prison;
	void* cr_loginclass;
	uint32_t cr_flags;
	void* cr_pspare2[2];
	uint64_t cr_sceAuthID;
	uint64_t cr_sceCaps[0x04];
	uint64_t cr_sceAttr[0x04];
	char     cr_unk0A0[0x48];
	auditinfo_addr cr_audit;
	uint32_t* cr_groups;
	int	cr_agroups;
};

struct prison
{

};

TYPE_BEGIN(struct mount, 0x1000); // XXX: random, don't use directly without fixing it
TYPE_FIELD(void* mnt_data, 0x288);
TYPE_END();

TYPE_BEGIN(struct vnode, 0x1000); // XXX: random, don't use directly without fixing it
TYPE_FIELD(mount* v_mount, 0x20);
TYPE_END();

struct filedesc 
{
	void** fd_ofiles;	/* file structures for open files */
	char* fd_ofileflags;		/* per-process open file flags */
	vnode* fd_cdir;		/* current directory */
	vnode* fd_rdir;		/* root directory */
	vnode* fd_jdir;		/* jail root directory */
	int	fd_nfiles;		/* number of open files allocated */
	unsigned long* fd_map;		/* bitmap of free fds */
	int	fd_lastfile;		/* high-water mark of fd_ofiles */
	int	fd_freefile;		/* approx. next free file */
	unsigned short	fd_cmask;		/* mask for file creation */
	unsigned short	fd_refcnt;		/* thread reference count */
	unsigned short	fd_holdcnt;		/* hold count on structure + mutex */
};

struct lock_object {
	const char* lo_name;
	uint32_t lo_flags;
	uint32_t lo_data;
	void* lo_witness;
};

struct mtx
{
	struct lock_object lock_object;	/* Common lock properties. */
	volatile uintptr_t mtx_lock;	/* Owner and flags. */
};

struct sx {
	struct lock_object lock_object;
	volatile uintptr_t sx_lock;
};

TYPE_BEGIN(struct proc, 0x1000); // XXX: random, don't use directly without fixing it
TYPE_FIELD(LIST_ENTRY(proc) p_list, 0);
TYPE_FIELD(TAILQ_HEAD(, thread) p_threads, 0x10);
TYPE_FIELD(struct ucred* p_ucred, 0x40);
TYPE_FIELD(struct filedesc* p_fd, 0x48);
TYPE_FIELD(int p_pid, 0xB0);
TYPE_FIELD(mtx p_lock, 0xF8);
TYPE_FIELD(struct vmspace* p_vmspace, 0x168);
TYPE_FIELD(struct dynlib* p_dynlib, 0x340);
TYPE_FIELD(char titleId[10], 0x390);
TYPE_FIELD(char contentid[64], 0x3D4);
TYPE_FIELD(char p_comm[32], 0x454);
TYPE_FIELD(char p_elfpath[1024], 0x474);
TYPE_FIELD(int p_randomized_path_len, 0x8A8);
TYPE_FIELD(char p_randomized_path[0x100], 0x8AC);
TYPE_END();

TYPE_BEGIN(struct thread, 0x3D8); // XXX: random, don't use directly without fixing it
TYPE_FIELD(struct mtx* volatile td_lock, 0);
TYPE_FIELD(struct proc* td_proc, 8);
TYPE_FIELD(TAILQ_ENTRY(thread) td_plist, 0x10);
TYPE_FIELD(int tid, 0x88);
TYPE_FIELD(int td_pinned, 0x12C);
TYPE_FIELD(struct ucred* td_ucred, 0x130);
TYPE_FIELD(char td_name[32], 0x284);
TYPE_FIELD(uint64_t td_retval[2], 0x398);
TYPE_FIELD(uint16_t td_priority, 0x380);
TYPE_END();

#define	EVENTHANDLER_PRI_FIRST	0
#define	EVENTHANDLER_PRI_ANY	10000
#define	EVENTHANDLER_PRI_LAST	20000

struct eventhandler_list {
	char* el_name;
	int	 el_flags;
#define EHL_INITTED	(1<<0)
	unsigned int el_runcount;
	mtx	el_lock;
	TAILQ_ENTRY(eventhandler_list) el_link;
	TAILQ_HEAD(, eventhandler_entry) el_entries;
};

struct eventhandler_entry {
	TAILQ_ENTRY(eventhandler_entry)	ee_link;
	int				ee_priority;
#define	EHE_DEAD_PRIORITY	(-1)
	void* ee_arg;
};

typedef struct eventhandler_entry* eventhandler_tag;

struct iovec {
	void* iov_base;  // Pointer to the data
	size_t iov_len;   // Length of the data
};

enum	uio_rw { UIO_READ, UIO_WRITE };

/* Segment flag values. */
enum uio_seg {
	UIO_USERSPACE,		/* from user data space */
	UIO_SYSSPACE,		/* from system space */
	UIO_NOCOPY			/* don't copy, already in object */
};

struct uio {
	iovec* uio_iov;		/* scatter/gather list */
	int	uio_iovcnt;				/* length of scatter/gather list */
	off_t	uio_offset;			/* offset in target object */
	ssize_t	uio_resid;			/* remaining bytes to process */
	enum uio_seg uio_segflg;	/* address space */
	enum uio_rw uio_rw;		/* operation */
	thread* uio_td;				/* owner */
};

struct fpu_kern_ctx {
	
};

typedef	void (*systrace_args_func_t)(int, void*, uint64_t*, int*);
typedef	int	sy_call_t(struct thread*, void*);

struct sysent {			/* system call table */
	int	sy_narg;	/* number of arguments */
	sy_call_t* sy_call;	/* implementing function */
	au_event_t sy_auevent;	/* audit event associated with syscall */
	systrace_args_func_t sy_systrace_args_func;
	/* optional argument conversion function. */
	uint32_t sy_entry;	/* DTrace entry ID for systrace. */
	uint32_t sy_return;	/* DTrace return ID for systrace. */
	uint32_t sy_flags;	/* General flags for system calls. */
	uint32_t sy_thrcnt;
};

struct sysentvec {
	int		sv_size;	/* number of entries */
	struct sysent* sv_table;	/* pointer to sysent */
};

typedef unsigned char vm_prot_t;	/* protection codes */

#define	VM_PROT_NONE		((vm_prot_t) 0x00)
#define	VM_PROT_READ		((vm_prot_t) 0x01)
#define	VM_PROT_WRITE		((vm_prot_t) 0x02)
#define	VM_PROT_EXECUTE		((vm_prot_t) 0x04)
#define	VM_PROT_COPY		((vm_prot_t) 0x08)	/* copy-on-read */

#define	VM_PROT_ALL		(VM_PROT_READ|VM_PROT_WRITE|VM_PROT_EXECUTE)
#define VM_PROT_RW		(VM_PROT_READ|VM_PROT_WRITE)
#define	VM_PROT_DEFAULT		VM_PROT_ALL

typedef char vm_inherit_t;	/* inheritance codes */

#define	VM_INHERIT_SHARE	((vm_inherit_t) 0)
#define	VM_INHERIT_COPY		((vm_inherit_t) 1)
#define	VM_INHERIT_NONE		((vm_inherit_t) 2)
#define	VM_INHERIT_DEFAULT	VM_INHERIT_COPY

typedef unsigned char vm_flags_t;
typedef unsigned int vm_eflags_t;

union vm_map_object {
	struct vm_object* vm_object;	/* object object */
	struct vm_map* sub_map;		/* belongs to another map */
};

struct vm_map_entry {
	struct vm_map_entry* prev;	/* previous entry */						// 0x00
	struct vm_map_entry* next;	/* next entry */							// 0x08
	struct vm_map_entry* left;	/* left child in binary search tree */		// 0x10
	struct vm_map_entry* right;	/* right child in binary search tree */		// 0x18
	vm_offset_t start;		/* start address */								// 0x20
	vm_offset_t end;		/* end address */								// 0x28
	vm_offset_t avail_ssize;	/* amt can grow if this is a stack */		// 0x30
	vm_size_t adj_free;		/* amount of adjacent free space */				// 0x38
	vm_size_t max_free;		/* max free space in subtree */					// 0x40
	union vm_map_object object;	/* object I point to */						// 0x48
	vm_ooffset_t offset;		/* offset into object */					// 0x50
	vm_eflags_t eflags;		/* map entry flags */							// 0x58
	vm_prot_t protection;		/* protection code */						// 0x5C
	vm_prot_t max_protection;	/* maximum protection */					// 0x5D
	vm_inherit_t inheritance;	/* inheritance */							// 0x5E
	int wired_count;		/* can be paged if = 0 */						// 0x60
	vm_pindex_t lastr;		/* last read */									// 0x68
	struct ucred* cred;		/* tmp storage for creator ref */				// 0x70
	char unk78[0x1]; //budget related?
	char unk79[0x14];
	char name[0x20]; //0x8D
	char unkAD[0x13];
}; // 0xC0

struct vm_map {
	struct vm_map_entry header;	/* List of entries */									// 0x0
	struct sx lock;			/* Lock for map data */										// 0xC0
	struct mtx system_mtx;																// 0xE0
	int nentries;			/* Number of entries */
	vm_size_t size;			/* virtual size */
	unsigned int timestamp;		/* Version number */
	unsigned char needs_wakeup;
	unsigned char system_map;		/* (c) Am I a system map? */
	vm_flags_t flags;		/* flags for this vm_map */
};

typedef struct vm_map* vm_map_t;

struct vmspace {
	struct vm_map vm_map;	/* VM address map */
};

/*
 * Types for d_flags.
 */
#define	D_TAPE	0x0001
#define	D_DISK	0x0002
#define	D_TTY	0x0004
#define	D_MEM	0x0008

#define	D_TYPEMASK	0xffff

 /*
  * Flags for d_flags which the drivers can set.
  */
#define	D_TRACKCLOSE	0x00080000	/* track all closes */
#define D_MMAP_ANON	0x00100000	/* special treatment in vm_mmap.c */
#define D_PSEUDO	0x00200000	/* make_dev() can return NULL */
#define D_NEEDGIANT	0x00400000	/* driver want Giant */
#define	D_NEEDMINOR	0x00800000	/* driver uses clone_create() */

  /*
   * Version numbers.
   */
#define D_VERSION_00	0x20011966
#define D_VERSION_01	0x17032005	/* Add d_uid,gid,mode & kind */
#define D_VERSION_02	0x28042009	/* Add d_mmap_single */
#define D_VERSION_03	0x17122009	/* d_mmap takes memattr,vm_ooffset_t */
#define D_VERSION	D_VERSION_03

   /*
	* Flags used for internal housekeeping
	*/
#define D_INIT		0x80000000	/* cdevsw initialized */

#define	MAKEDEV_REF		0x01
#define	MAKEDEV_WHTOUT		0x02
#define	MAKEDEV_NOWAIT		0x04
#define	MAKEDEV_WAITOK		0x08
#define	MAKEDEV_ETERNAL		0x10
#define	MAKEDEV_CHECKNAME	0x20

#define		UID_ROOT	0
#define		UID_BIN		3
#define		UID_UUCP	66
#define		UID_NOBODY	65534

#define		GID_WHEEL	0
#define		GID_KMEM	2
#define		GID_TTY		4
#define		GID_OPERATOR	5
#define		GID_BIN		7
#define		GID_GAMES	13
#define		GID_DIALER	68
#define		GID_NOBODY	65534

#define	S_IRWXU	0000700			/* RWX mask for owner */
#define	S_IRUSR	0000400			/* R for owner */
#define	S_IWUSR	0000200			/* W for owner */
#define	S_IXUSR	0000100			/* X for owner */

#define	S_IRWXG	0000070			/* RWX mask for group */
#define	S_IRGRP	0000040			/* R for group */
#define	S_IWGRP	0000020			/* W for group */
#define	S_IXGRP	0000010			/* X for group */

#define	S_IRWXO	0000007			/* RWX mask for other */
#define	S_IROTH	0000004			/* R for other */
#define	S_IWOTH	0000002			/* W for other */
#define	S_IXOTH	0000001			/* X for other */

typedef int d_ioctl_t(struct cdev* dev, unsigned long cmd, caddr_t data, int fflag, struct thread* td);

	/*
	 * Character device switch table
	 */
#pragma pack(push, 1)
struct cdevsw {
	int			d_version;
	unsigned int d_flags;
	const char* d_name;
	void* d_open;
	void* d_fdopen;
	void* d_close;
	void* d_read;
	void* d_write;
	d_ioctl_t* d_ioctl;
	void* d_poll;
	void* d_mmap;
	void* d_strategy;
	void* d_dump;
	void* d_kqfilter;
	void* d_purge;
	void* d_mmap_single;
	
	// PlayStation 4 Addition, credits: ChendoChap
	void* d_mmap_single_ext;
	
	int32_t			d_spare0[3];
	void* d_spare1[3];
	
	char pad[0x1C];
};
#pragma pack(pop)

static_assert(sizeof(struct cdevsw) == 0xC0, "devsw size invalid.");

typedef uint32_t devfs_rid;
typedef uint16_t devfs_rsnum;

struct devfs_rule {
	uint32_t dr_magic;			/* Magic number. */
	devfs_rid dr_id;			/* Identifier. */

	/*
	 * Conditions under which this rule should be applied.  These
	 * are ANDed together since OR can be simulated by using
	 * multiple rules.  dr_icond determines which of the other
	 * variables we should process.
	 */
	int	dr_icond;
#define	DRC_DSWFLAGS	0x001
#define	DRC_PATHPTRN	0x002
	int	dr_dswflags;			/* cdevsw flags to match. */
#define	DEVFS_MAXPTRNLEN	200
	char	dr_pathptrn[DEVFS_MAXPTRNLEN];	/* Pattern to match path. */

	/*
	 * Things to change.  dr_iacts determines which of the other
	 * variables we should process.
	 */
	int	dr_iacts;
#define	DRA_BACTS	0x001
#define	DRA_UID		0x002
#define	DRA_GID		0x004
#define	DRA_MODE	0x008
#define	DRA_INCSET	0x010
	int	dr_bacts;			/* Boolean (on/off) action. */
#define	DRB_HIDE	0x001			/* Hide entry (DE_WHITEOUT). */
#define	DRB_UNHIDE	0x002			/* Unhide entry. */
	uid_t	dr_uid;
	gid_t	dr_gid;
	mode_t	dr_mode;
	devfs_rsnum dr_incset;			/* Included ruleset. */
};

#define	DEVFS_MAGIC	0xdb0a087a
#define _PATH_DEV "/dev/"

struct devfs_dirent
{

};

TYPE_BEGIN(struct devfs_mount, 0x50);
TYPE_FIELD(devfs_dirent* dm_rootdir, 0x10);
TYPE_END();

TYPE_BEGIN(struct devfs_krule, 0x108);
TYPE_FIELD(devfs_rule dk_rule, 0x18);
TYPE_END();

struct dynlib_get_obj_member {
	uint32_t handle;
	uint32_t index;
	uint64_t value;
};

typedef STAILQ_HEAD(Struct_Objlist, Struct_Objlist_Entry) Objlist;

TYPE_BEGIN(struct dynlib_obj, 0x180);
TYPE_FIELD(SLIST_ENTRY(dynlib_obj) entries, 0x0);
TYPE_FIELD(char* path, 0x8);
TYPE_FIELD(int ref_count, 0x20);
TYPE_FIELD(int dl_ref_count, 0x24);
TYPE_FIELD(int handle, 0x28);
TYPE_FIELD(caddr_t map_base, 0x30);
TYPE_FIELD(size_t map_size, 0x38);
TYPE_FIELD(size_t text_size, 0x40);
TYPE_FIELD(caddr_t database, 0x48);
TYPE_FIELD(size_t data_size, 0x50);
TYPE_FIELD(size_t vaddr_base, 0x68);
TYPE_FIELD(caddr_t realloc_base, 0x70);
TYPE_FIELD(caddr_t entry, 0x78);
TYPE_END();

// Thank you flatz for 1.62
// Thank you ChendoChap for fixing newer fw's
struct dynlib
{
	SLIST_HEAD(, dynlib_obj) objs;
	struct dynlib* self;
	struct dynlib_obj* main_obj;
	struct dynlib_obj* libkernel_obj;
	struct dynlib_obj* asan_obj;
	uint32_t nmodules; // module count
	char unk2C[0x4];
	Objlist obj_list_0;
	Objlist obj_list_1;
	Objlist obj_list_2;
	Objlist obj_list_3;
	sx bind_lock;
	char unk90[0x18];
	uint8_t unkA8[0x8];
	uint8_t unkB0[0x8];
	uint8_t unkB8[0x8];
	uint8_t unkC0[0x4]; // set to 1
	uint8_t unkC4[0x4]; // set to 1, this and above optimized to one 0x100000001 (on creation)
	void* procparam_seg_addr;
	uint64_t procparam_seg_filesz;
	void* unpatched_call_addr;
	void* __freeze_pointer;
	void* sysc_s00_pointer;
	void* sysc_e00_pointer;
	uint32_t restrict_flags; //flags of some kind, conditionally zeroes out some stuff in the dynlib  info_ex syscall and other places as well.
	uint32_t no_dynamic_segment; //also flags, used to conditionally load the asan? other bit used for sys_mmap_dmem?
	int is_sandboxed; //((proc->p_fd->fd_rdir != rootvnode) ? 1 : 0)   -> used to determine if it should use random path or system/ to load modules
	uint8_t unk104[0x4];
};

struct stat {
	
};

LIST_HEAD(proclist, proc);

#define	FOREACH_PROC_IN_SYSTEM(p)					\
	LIST_FOREACH((p), allproc, p_list)