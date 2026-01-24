#pragma once

#define PGEX_P		0x01	/* Protection violation vs. not present */
#define PGEX_W		0x02	/* during a Write cycle */
#define PGEX_U		0x04	/* access from User mode (UPL) */
#define PGEX_RSV	0x08	/* reserved PTE field is non-zero */
#define PGEX_I		0x10	/* during an instruction fetch */

#define	T_PAGEFLT	12	/* page fault */

struct amd64_frame
{
	struct amd64_frame* f_frame;
	long                f_retaddr;
	long                f_arg0;
};

struct trapframe
{
	register_t tf_rdi;			// 0x00
	register_t tf_rsi;			// 0x08
	register_t tf_rdx;			// 0x10
	register_t tf_rcx;			// 0x18
	register_t tf_r8;			// 0x20
	register_t tf_r9;			// 0x28
	register_t tf_rax;			// 0x30
	register_t tf_rbx;			// 0x38
	register_t tf_rbp;			// 0x40
	register_t tf_r10;			// 0x48
	register_t tf_r11;			// 0x50
	register_t tf_r12;			// 0x58
	register_t tf_r13;			// 0x60
	register_t tf_r14;			// 0x68
	register_t tf_r15;			// 0x70
	uint32_t tf_trapno;			// 0x78
	uint16_t tf_fs;				// 0x7C
	uint16_t tf_gs;				// 0x7E
	register_t tf_addr;			// 0x80
	uint32_t tf_flags;			// 0x88
	uint16_t tf_es;				// 0x8C
	uint16_t tf_ds;				// 0x8E

	register_t tf_last_branch_from;		// 0x90
	register_t tf_last_branch_to;		// 0x98

	/* below portion defined in hardware */
	register_t tf_err;			// 0xA0
	register_t tf_rip;			// 0xA8
	register_t tf_cs;			// 0xB0
	register_t tf_rflags;			// 0xB8
	register_t tf_rsp;			// 0xC0
	register_t tf_ss;			// 0xC8
};

class TrapFatalExtension
{
public:
	static void Init();
	static void Term();

private:
	static Detour* TrapFatalDetour;

	static void TrapFatalHook(struct trapframe* frame, vm_offset_t eva);
};