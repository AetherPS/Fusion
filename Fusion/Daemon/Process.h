#pragma once

#define	WMESGLEN	8		/* size of returned wchan message */
#define	LOCKNAMELEN	8		/* size of returned lock name */
#define	TDNAMLEN	16		/* size of returned thread name */
#define	COMMLEN		19		/* size of returned ki_comm name */
#define	KI_EMULNAMELEN	16	/* size of returned ki_emul */
#define	KI_NGROUPS	16		/* number of groups in ki_groups */
#define	LOGNAMELEN	17		/* size of returned ki_login */
#define	LOGINCLASSLEN	17	/* size of returned ki_loginclass */

#pragma pack(push, 1)
struct kinfo_proc {
	int	ki_structsize;						// 0x00
	int	ki_layout;							// 0x04
	void* args;								// 0x08
	void* paddr;							// 0x10
	void* addr;								// 0x18
	void* tracep;							// 0x20
	void* textvp;							// 0x28
	void* fd;								// 0x30
	void* vmspace;							// 0x38
	void* wchan;							// 0x40
	pid_t ki_pid;							// 0x48
	pid_t ki_ppid;							// 0x4C
	pid_t ki_pgid;							// 0x50
	pid_t ki_tpgid;							// 0x54
	pid_t ki_sid;							// 0x58
	pid_t ki_tsid;							// 0x5C
	short ki_jobc;							// 0x60
	char _0x62[0x2];						// 0x62
	dev_t ki_tdev;							// 0x64
	sigset_t ki_siglist;					// 0x68
	sigset_t ki_sigmask;					// 0x78
	sigset_t ki_sigignore;					// 0x88
	sigset_t ki_sigcatch;					// 0x98
	uid_t ki_uid;							// 0xA8
	uid_t ki_ruid;							// 0xAC
	uid_t ki_svuid;							// 0xB0
	gid_t ki_rgid;							// 0xB4
	gid_t ki_svgid;							// 0xB8
	short ki_ngroups;						// 0xBC
	char _0xBE[2];
	gid_t	ki_groups[KI_NGROUPS];			// 0xC0
	uint64_t ki_size;						// 0x100
	int64_t ki_rssize;						// 0x108
	int64_t ki_swrss;						// 0x110
	int64_t ki_tsize;						// 0x118
	int64_t ki_dsize;						// 0x120
	int64_t ki_ssize;						// 0x128
	u_short	ki_xstat;						// 0x130
	u_short	ki_acflag;						// 0x132
	uint32_t ki_pctcpu;						// 0x134
	u_int	ki_estcpu;						// 0x138
	u_int	ki_slptime;						// 0x13C
	u_int	ki_swtime;						// 0x140
	char _0x144[0x4];
	u_int64_t ki_runtime;					// 0x148
	struct	timeval ki_start;				// 0x150
	struct	timeval ki_childtime;			// 0x160
	long	ki_flag;						// 0x170
	long	ki_kiflag;						// 0x178
	int	ki_traceflag;						// 0x180
	char	ki_stat;						// 0x184
	signed char ki_nice;					// 0x185
	char	ki_lock;						// 0x186
	char	ki_rqindex;						// 0x187
	u_char	ki_oncpu;						// 0x188
	u_char	ki_lastcpu;						// 0x189
	char ki_tdname[TDNAMLEN + 1];			// 0x190
	char ki_wmesg[WMESGLEN + 1];			// 0x19B
	char ki_login[LOGNAMELEN + 1];			// 0x1A4
	char ki_lockname[LOCKNAMELEN + 1];		// 0x1B6
	char ki_comm[COMMLEN + 1];				// 0x1BF
	char ki_emul[KI_EMULNAMELEN + 1];		// 0x1D3
	char ki_loginclass[LOGINCLASSLEN + 1];	// 0x1E4
	char _0x1F6[0x56];
	u_int ki_cr_flags;						// 0x24C
	int	ki_jid;								// 0x250
	int	ki_numthreads;						// 0x254
	int32_t	ki_tid;							// 0x258
	char _0x25C[0x1EC];
}; // Size = 0x448
#pragma pack(pop)

static_assert(sizeof(kinfo_proc) == 0x448, "kinfo_proc size is not 0x448");

#define CTL_KERN 1
#define KERN_PROC 14
#define KERN_PROC_ALL 0
#define KERN_PROC_PROC 8
#define KERN_PROC_PID 1

int GetProcessList(std::vector<kinfo_proc>& ProcessList);
int GetProcInfo(int pid, kinfo_proc* out);
int GetPidByName(const char* ProcessName);
int GetPidByAppId(int appId);