#pragma once

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
