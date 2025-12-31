#pragma once

typedef int d_ioctl_t(struct cdev* dev, unsigned long cmd, caddr_t data, int fflag, struct thread* td);
typedef uint32_t devfs_rid;
typedef uint16_t devfs_rsnum;
typedef uint32_t uid_t;
typedef uint32_t gid_t;
typedef uint16_t mode_t;

// --- Device Types (for d_flags) ---
#define	D_TAPE	0x0001
#define	D_DISK	0x0002
#define	D_TTY	0x0004
#define	D_MEM	0x0008
#define	D_TYPEMASK	0xffff

// --- Driver Flags (for d_flags) ---
#define	D_TRACKCLOSE	0x00080000	/* track all closes */
#define D_MMAP_ANON		0x00100000	/* special treatment in vm_mmap.c */
#define D_PSEUDO		0x00200000	/* make_dev() can return NULL */
#define D_NEEDGIANT		0x00400000	/* driver want Giant */
#define	D_NEEDMINOR		0x00800000	/* driver uses clone_create() */

// --- Version Numbers ---

#define D_VERSION_00 	0x20011966
#define D_VERSION_01 	0x17032005 	 /* Add d_uid,gid,mode & kind */
#define D_VERSION_02 	0x28042009 	 /* Add d_mmap_single */
#define D_VERSION_03 	0x17122009 	 /* d_mmap takes memattr,vm_ooffset_t */
#define D_VERSION 		D_VERSION_03

// --- Internal Housekeeping Flags ---
#define D_INIT 	 		0x80000000 	 /* cdevsw initialized */

// --- Make Dev Flags ---
#define	MAKEDEV_REF			0x01
#define	MAKEDEV_WHTOUT		0x02
#define	MAKEDEV_NOWAIT		0x04
#define	MAKEDEV_WAITOK		0x08
#define	MAKEDEV_ETERNAL		0x10
#define	MAKEDEV_CHECKNAME	0x20

// --- Standard User and Group IDs ---
#define	UID_ROOT		0
#define	UID_BIN			3
#define	UID_UUCP		66
#define	UID_NOBODY		65534

#define	GID_WHEEL		0
#define	GID_KMEM		2
#define	GID_TTY			4
#define	GID_OPERATOR	5
#define	GID_BIN			7
#define	GID_GAMES		13
#define	GID_DIALER		68
#define	GID_NOBODY		65534

// --- File Mode Constants (Permissions) ---
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

/**
 * d_ioctl_t: Prototype for the ioctl device driver entry point.
 */
typedef int d_ioctl_t(struct cdev* dev, unsigned long cmd, caddr_t data, int fflag, struct thread* td);

/**
 * struct cdevsw: The character device switch table entry.
 * Contains function pointers for all supported device operations (open, close, read, write, ioctl, etc.).
 */
#pragma pack(push, 1)
struct cdevsw {
    int d_version;
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

    int32_t d_spare0[3];
    void* d_spare1[3];

    char pad[0x1C];
};
#pragma pack(pop)

static_assert(sizeof(struct cdevsw) == 0xC0, "devsw size invalid.");

// --- Device Filesystem (devfs) ---
#define DEVFS_MAGIC 0xdb0a087a
#define _PATH_DEV   "/dev/"

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

struct devfs_dirent
{
    // Omitted fields...
};

/**
* struct devfs_mount: Information about the devfs mounted instance.
*/
TYPE_BEGIN(struct devfs_mount, 0x50);
TYPE_FIELD(devfs_dirent* dm_rootdir, 0x10);
TYPE_END();

/**
* struct devfs_krule: A rule used by devfs to manage device entry properties.
*/
TYPE_BEGIN(struct devfs_krule, 0x108);
TYPE_FIELD(struct devfs_rule dk_rule, 0x18);
TYPE_END();
