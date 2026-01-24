#include "Common.h"
#include "TrapFatalExtension.h"

Detour* TrapFatalExtension::TrapFatalDetour;

void TrapFatalExtension::Init()
{
	Detour::Detour32(&TrapFatalDetour, g_KernelAddrs.trap_fatalHook, (void*)TrapFatalHook);
}

void TrapFatalExtension::Term()
{
	delete TrapFatalDetour;
}

bool IsStackSpace(void* p_Address)
{
	return ((reinterpret_cast<uint64_t>(p_Address) & 0xFFFFFFFF00000000) == 0xFFFFFF8000000000);
}

void TrapFatalExtension::TrapFatalHook(struct trapframe* frame, vm_offset_t eva)
{
	int code;
	uint32_t type;
	const char* msg;

	static const char* trap_msg[] = {
		"",                               /*  0 unused */
		"privileged instruction fault",   /*  1 T_PRIVINFLT */
		"",                               /*  2 unused */
		"breakpoint instruction fault",   /*  3 T_BPTFLT */
		"",                               /*  4 unused */
		"",                               /*  5 unused */
		"arithmetic trap",                /*  6 T_ARITHTRAP */
		"",                               /*  7 unused */
		"",                               /*  8 unused */
		"general protection fault",       /*  9 T_PROTFLT */
		"trace trap",                     /* 10 T_TRCTRAP */
		"",                               /* 11 unused */
		"page fault",                     /* 12 T_PAGEFLT */
		"",                               /* 13 unused */
		"alignment fault",                /* 14 T_ALIGNFLT */
		"",                               /* 15 unused */
		"",                               /* 16 unused */
		"",	                              /* 17 unused */
		"integer divide fault",           /* 18 T_DIVIDE */
		"non-maskable interrupt trap",    /* 19 T_NMI */
		"overflow trap",                  /* 20 T_OFLOW */
		"FPU bounds check fault",         /* 21 T_BOUND */
		"FPU device not available",       /* 22 T_DNA */
		"double fault",                   /* 23 T_DOUBLEFLT */
		"FPU operand fetch fault",        /* 24 T_FPOPFLT */
		"invalid TSS fault",              /* 25 T_TSSFLT */
		"segment not present fault",      /* 26 T_SEGNPFLT */
		"stack fault",                    /* 27 T_STKFLT */
		"machine check trap",             /* 28 T_MCHK */
		"SIMD floating-point exception",  /* 29 T_XMMFLT */
		"reserved (unknown) fault",       /* 30 T_RESERVED */
		"",                               /* 31 unused (reserved) */
		"DTrace pid return trap",         /* 32 T_DTRACE_RET */
		"DTrace fasttrap probe trap",     /* 33 T_DTRACE_PROBE */
	};

	code = frame->tf_err;
	type = frame->tf_trapno;

	if (type <= 33)
		msg = trap_msg[type];
	else
		msg = "UNKNOWN";

	// Print header
	printf("\n========== KERNEL PANIC ==========\n");
	printf("Trap %d: %s (%s mode)\n",
		type, msg,
		ISPL(frame->tf_cs) == SEL_UPL ? "user" : "kernel");

	// Critical crash info
	printf("\nRIP: 0x%016llx (CS: 0x%04x)\n", frame->tf_rip, frame->tf_cs & 0xffff);
	printf("RSP: 0x%016llx (SS: 0x%04x)\n", frame->tf_rsp,
		ISPL(frame->tf_cs) == SEL_UPL ? (frame->tf_ss & 0xffff) : GSEL(GDATA_SEL, SEL_KPL));
	printf("RBP: 0x%016llx\n", frame->tf_rbp);

	if (type == T_PAGEFLT)
	{
		printf("\nPage Fault Address: 0x%016lx\n", eva);
		printf("Fault Type: %s %s %s (%s)\n",
			code & PGEX_U ? "user" : "supervisor",
			code & PGEX_W ? "write" : "read",
			code & PGEX_I ? "instruction" : "data",
			code & PGEX_P ? "protection violation" : "not present");
	}

	// Process info
	if (CurrentThread()->td_proc)
		printf("\nProcess: %s (PID: %lu)\n",
			CurrentThread()->td_name,
			(unsigned long)CurrentThread()->td_proc->p_pid);

	// Register dump
	printf("\n--- Registers ---\n");
	printf("RAX: 0x%016llx  RBX: 0x%016llx  RCX: 0x%016llx  RDX: 0x%016llx\n",
		frame->tf_rax, frame->tf_rbx, frame->tf_rcx, frame->tf_rdx);
	printf("RSI: 0x%016llx  RDI: 0x%016llx  RBP: 0x%016llx  RSP: 0x%016llx\n",
		frame->tf_rsi, frame->tf_rdi, frame->tf_rbp, frame->tf_rsp);
	printf("R8 : 0x%016llx  R9 : 0x%016llx  R10: 0x%016llx  R11: 0x%016llx\n",
		frame->tf_r8, frame->tf_r9, frame->tf_r10, frame->tf_r11);
	printf("R12: 0x%016llx  R13: 0x%016llx  R14: 0x%016llx  R15: 0x%016llx\n",
		frame->tf_r12, frame->tf_r13, frame->tf_r14, frame->tf_r15);
	printf("RFL: 0x%016llx  ERR: 0x%08x\n", frame->tf_rflags, code);

	// Last branch records (useful for debugging)
	if (frame->tf_last_branch_from || frame->tf_last_branch_to)
	{
		printf("\n--- Last Branch ---\n");
		printf("From: 0x%016llx\n", frame->tf_last_branch_from);
		printf("To  : 0x%016llx\n", frame->tf_last_branch_to);
	}

	// Stack backtrace
	printf("\n--- Call Stack ---\n");
	auto hold = vm_fault_disable_pagefaults();
	auto amdFrame = reinterpret_cast<struct amd64_frame*>(frame->tf_rbp);

	if (amdFrame != nullptr)
	{
		int depth = 0;
		while (IsStackSpace(amdFrame) && depth < 32)
		{
			printf("[%2d] 0x%016llx\n", depth, (uint64_t)amdFrame->f_retaddr);
			amdFrame = amdFrame->f_frame;
			depth++;
		}
	}

	vm_fault_enable_pagefaults(hold);

	printf("\nKernel Base: 0x%016llx\n", (uint64_t)g_KernelBase);
	printf("==================================\n");
	printf("System halted.\n");

	for (;;);
}