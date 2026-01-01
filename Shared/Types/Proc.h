#pragma once

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

/**
* struct ucred: Holds authentication and security information for a process/thread.
*/
struct ucred {
    uint32_t cr_ref;
    uint32_t cr_uid;
    uint32_t cr_ruid;
    uint32_t cr_svuid;
    int 	 cr_ngroups;
    uint32_t cr_rgid;
    uint32_t cr_svgid;
    void* cr_uidinfo;
    void* cr_ruidinfo;
    void* cr_prison;       // Pointer to the jail/prison structure
    void* cr_loginclass;
    uint32_t cr_flags;
    void* cr_pspare2[2];
    uint64_t cr_sceAuthID;
    uint64_t cr_sceCaps[0x04];
    uint64_t cr_sceAttr[0x04];
    char 	 cr_unk0A0[0x48];
    struct auditinfo_addr cr_audit; // Assuming auditinfo_addr is defined
    uint32_t* cr_groups;
    int 	 cr_agroups;
};

TYPE_BEGIN(struct mount, 0x1000); // XXX: random, don't use directly without fixing it
TYPE_FIELD(void* mnt_data, 0x288);
TYPE_END();

TYPE_BEGIN(struct vnode, 0x1000); // XXX: random, don't use directly without fixing it
TYPE_FIELD(struct mount* v_mount, 0x20);
TYPE_END();

/**
 * struct filedesc: Contains the file-related state for a process.
 */
struct filedesc
{
    void** fd_ofiles; 	        /* file structures for open files */
    char* fd_ofileflags; 	    /* per-process open file flags */
    struct vnode* fd_cdir; 	 	/* current directory (vnode*) */
    struct vnode* fd_rdir; 	    /* root directory (vnode*) */
    struct vnode* fd_jdir; 	    /* jail root directory (vnode*) */
    int fd_nfiles; 	            /* number of open files allocated */
    unsigned long* fd_map; 	    /* bitmap of free fds */
    int fd_lastfile; 	        /* high-water mark of fd_ofiles */
    int fd_freefile; 	        /* approx. next free file */
    unsigned short fd_cmask; 	/* mask for file creation */
    unsigned short fd_refcnt; 	/* thread reference count */
    unsigned short fd_holdcnt; 	/* hold count on structure + mutex */
};

struct prison
{

};

TYPE_BEGIN(struct proc, 0x1000); // XXX: random, don't use directly without fixing it
TYPE_FIELD(LIST_ENTRY(proc) p_list, 0);     // Link to the global process list
TYPE_FIELD(TAILQ_HEAD(, thread) p_threads, 0x10); // List of threads in this process
TYPE_FIELD(struct ucred* p_ucred, 0x40);  // Pointer to the process credentials
TYPE_FIELD(struct filedesc* p_fd, 0x48); // Pointer to the file descriptor table
TYPE_FIELD(int p_pid, 0xB0);            // Process ID
TYPE_FIELD(struct mtx p_lock, 0xF8);           // Process lock
TYPE_FIELD(struct vmspace* p_vmspace, 0x168); // Pointer to the address space
TYPE_FIELD(struct dynlib* p_dynlib, 0x340); // Pointer to dynamic library info
TYPE_FIELD(char titleId[10], 0x390);    // Application Title ID
TYPE_FIELD(char contentid[64], 0x3D4); // Application Content ID
TYPE_FIELD(char p_comm[32], 0x454);     // Command name
TYPE_FIELD(char p_elfpath[1024], 0x474); // Full path to the executable ELF file
TYPE_FIELD(int p_randomized_path_len, 0x8A8);
TYPE_FIELD(char p_randomized_path[0x100], 0x8AC);
TYPE_END();

TYPE_BEGIN(struct thread, 0x3D8); // XXX: random, don't use directly without fixing it
TYPE_FIELD(struct mtx* volatile td_lock, 0); // Thread lock pointer
TYPE_FIELD(struct proc* td_proc, 8);      // Pointer to the parent process
TYPE_FIELD(TAILQ_ENTRY(thread) td_plist, 0x10); // Link in the process's thread list
TYPE_FIELD(int tid, 0x88);              // Thread ID
TYPE_FIELD(int td_pinned, 0x12C);       // Pinning state (e.g., to a CPU)
TYPE_FIELD(struct ucred* td_ucred, 0x130); // Pointer to the thread credentials
TYPE_FIELD(char td_name[32], 0x284);     // Thread name (for debugging)
TYPE_FIELD(uint64_t td_retval[2], 0x398); // Return values from a syscall/function
TYPE_FIELD(uint16_t td_priority, 0x380); // Scheduling priority
TYPE_END();

LIST_HEAD(proclist, proc);

/**
 * FOREACH_PROC_IN_SYSTEM: Macro to iterate over all active processes.
 * Assumes 'allproc' is the head of the global process list (type 'proclist').
 * @p: The loop variable (struct proc*).
 */
#define	FOREACH_PROC_IN_SYSTEM(p)					\
	LIST_FOREACH((p), allproc, p_list)