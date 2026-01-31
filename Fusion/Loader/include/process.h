#pragma once

#define WMESGLEN      8
#define LOCKNAMELEN   8
#define TDNAMLEN      16
#define COMMLEN       19
#define KI_EMULNAMELEN 16
#define KI_NGROUPS    16
#define LOGNAMELEN    17
#define LOGINCLASSLEN 17

#pragma pack(push, 1)
struct kinfo_proc {
    int32_t   ki_structsize;                    // 0x00
    int32_t   ki_layout;                        // 0x04
    uint64_t  args;                             // 0x08
    uint64_t  paddr;                            // 0x10
    uint64_t  addr;                             // 0x18
    uint64_t  tracep;                           // 0x20
    uint64_t  textvp;                           // 0x28
    uint64_t  fd;                               // 0x30
    uint64_t  vmspace;                          // 0x38
    uint64_t  wchan;                            // 0x40
    int32_t   ki_pid;                           // 0x48
    int32_t   ki_ppid;                          // 0x4C
    int32_t   ki_pgid;                          // 0x50
    int32_t   ki_tpgid;                         // 0x54
    int32_t   ki_sid;                           // 0x58
    int32_t   ki_tsid;                          // 0x5C
    int16_t   ki_jobc;                          // 0x60
    char      _0x62[0x2];                       // 0x62
    uint32_t  ki_tdev;                          // 0x64
    uint8_t   ki_siglist[16];                   // 0x68 (sigset_t is 128 bits)
    uint8_t   ki_sigmask[16];                   // 0x78
    uint8_t   ki_sigignore[16];                 // 0x88
    uint8_t   ki_sigcatch[16];                  // 0x98
    uint32_t  ki_uid;                           // 0xA8
    uint32_t  ki_ruid;                          // 0xAC
    uint32_t  ki_svuid;                         // 0xB0
    uint32_t  ki_rgid;                          // 0xB4
    uint32_t  ki_svgid;                         // 0xB8
    int16_t   ki_ngroups;                       // 0xBC
    char      _0xBE[2];                         // 0xBE
    uint32_t  ki_groups[KI_NGROUPS];            // 0xC0
    uint64_t  ki_size;                          // 0x100
    int64_t   ki_rssize;                        // 0x108
    int64_t   ki_swrss;                         // 0x110
    int64_t   ki_tsize;                         // 0x118
    int64_t   ki_dsize;                         // 0x120
    int64_t   ki_ssize;                         // 0x128
    uint16_t  ki_xstat;                         // 0x130
    uint16_t  ki_acflag;                        // 0x132
    uint32_t  ki_pctcpu;                        // 0x134
    uint32_t  ki_estcpu;                        // 0x138
    uint32_t  ki_slptime;                       // 0x13C
    uint32_t  ki_swtime;                        // 0x140
    char      _0x144[0x4];                      // 0x144
    uint64_t  ki_runtime;                       // 0x148
    struct {                                    // 0x150 (struct timeval)
        int64_t tv_sec;
        int64_t tv_usec;
    } ki_start;
    struct {                                    // 0x160 (struct timeval)
        int64_t tv_sec;
        int64_t tv_usec;
    } ki_childtime;
    int64_t   ki_flag;                          // 0x170
    int64_t   ki_kiflag;                        // 0x178
    int32_t   ki_traceflag;                     // 0x180
    char      ki_stat;                          // 0x184
    int8_t    ki_nice;                          // 0x185
    char      ki_lock;                          // 0x186
    char      ki_rqindex;                       // 0x187
    uint8_t   ki_oncpu;                         // 0x188
    uint8_t   ki_lastcpu;                       // 0x189
    char      ki_tdname[TDNAMLEN + 1];          // 0x18A
    char      ki_wmesg[WMESGLEN + 1];           // 0x19B
    char      ki_login[LOGNAMELEN + 1];         // 0x1A4
    char      ki_lockname[LOCKNAMELEN + 1];     // 0x1B6
    char      ki_comm[COMMLEN + 1];             // 0x1BF
    char      ki_emul[KI_EMULNAMELEN + 1];      // 0x1D3
    char      ki_loginclass[LOGINCLASSLEN + 1]; // 0x1E4
    char      _0x1F6[0x56];                     // 0x1F6
    uint32_t  ki_cr_flags;                      // 0x24C
    int32_t   ki_jid;                           // 0x250
    int32_t   ki_numthreads;                    // 0x254
    int32_t   ki_tid;                           // 0x258
    char      _0x25C[0x1EC];                    // 0x25C
};
#pragma pack(pop)

_Static_assert(sizeof(struct kinfo_proc) == 0x448, "kinfo_proc size is not 0x448");

#define CTL_KERN 1
#define KERN_PROC 14
#define KERN_PROC_ALL 0
#define KERN_PROC_PROC 8
#define KERN_PROC_PID 1

int GetProcessList(std::vector<kinfo_proc>& ProcessList);
int GetPidByName(const char* ProcessName);