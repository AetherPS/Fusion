#include "common.h"

extern uint8_t _binary_resources_Kernel_elf_start[];
extern uint8_t _binary_resources_Kernel_elf_end[];

struct installKernelArgs {
	uint64_t sycall;
	void* payload;
	size_t psize;
};

int install_kernel(struct thread* td, struct installKernelArgs* args)
{
	uint64_t kernbase = kernel_getbase();
	kernel_resolve(kernbase);

	kernel_printf("Hello from kernel escalation!\n");

	//Jailbreak
	struct ucred* cred = td->td_proc->p_ucred;
	struct filedesc* fd = td->td_proc->p_fd;

	cred->cr_prison = *(void**)(kernbase + addr_prison0); //prison0
	fd->fd_rdir = *(void**)(kernbase + addr_rootvnode); //rootvnode
	fd->fd_jdir = *(void**)(kernbase + addr_rootvnode); //rootvnode

	cred->cr_uid = 0;
	cred->cr_ruid = 0;
	cred->cr_rgid = 0;
	cred->cr_groups[0] = 0;

	void* td_ucred = *(void**)(((char*)td) + 304);
	*(uint64_t*)(((char*)td_ucred) + 96) = 0xffffffffffffffff;
	*(uint64_t*)(((char*)td_ucred) + 88) = 0x3801000000000013;
	*(uint64_t*)(((char*)td_ucred) + 104) = 0xffffffffffffffff;

	kernel_printf("Thread jailbroken!\n");

	cpu_disable_wp();

#if defined(VERSION_900)
	*(uint8_t*)(kernbase + 0x002714BD) = 0xEB;
	*(uint8_t*)(kernbase + 0x0037BF3C) = VM_PROT_ALL;
	*(uint8_t*)(kernbase + 0x0037BF44) = VM_PROT_ALL;
#endif

	cpu_enable_wp();

	kernel_printf("Patches Applied!\n");

	size_t msize = 0;
	if (elf_mapped_size(args->payload, &msize))
	{
		return 1;
	}

	int s = (msize + 0x3FFFull) & ~0x3FFFull;
	void* payloadbase = (void*)kmem_alloc(kernel_map, s);
	if (!payloadbase)
	{
		return 1;
	}

	int r = 0;
	int (*payload_entry)(void* p);

	if ((r = load_elf(args->payload, args->psize, payloadbase, msize, (void**)&payload_entry)))
	{
		return r;
	}

	kernel_printf("Starting Kernel. (Shutdown: %llX, ELFBase: %llX, Size: %i)\n", payload_entry, payloadbase, s);

	if (payload_entry(0))
	{
		return 1;
	}

	return 0;
}

void ascii()
{
	klog("#######################################################################################################################\n");
	klog("\n");
	klog("FFFFFFFFFFFFFFFFFFFFFF                                     iiii                                      333333333333333   \n");
	klog("F::::::::::::::::::::F                                    i::::i                                    3:::::::::::::::33 \n");
	klog("F::::::::::::::::::::F                                     iiii                                     3::::::33333::::::3\n");
	klog("FF::::::FFFFFFFFF::::F                                                                              3333333     3:::::3\n");
	klog("  F:::::F       FFFFFFuuuuuu    uuuuuu      ssssssssss   iiiiiii    ooooooooooo   nnnn  nnnnnnnn                3:::::3\n");
	klog("  F:::::F             u::::u    u::::u    ss::::::::::s  i:::::i  oo:::::::::::oo n:::nn::::::::nn              3:::::3\n");
	klog("  F::::::FFFFFFFFFF   u::::u    u::::u  ss:::::::::::::s  i::::i o:::::::::::::::on::::::::::::::nn     33333333:::::3 \n");
	klog("  F:::::::::::::::F   u::::u    u::::u  s::::::ssss:::::s i::::i o:::::ooooo:::::onn:::::::::::::::n    3:::::::::::3  \n");
	klog("  F:::::::::::::::F   u::::u    u::::u   s:::::s  ssssss  i::::i o::::o     o::::o  n:::::nnnn:::::n    33333333:::::3 \n");
	klog("  F::::::FFFFFFFFFF   u::::u    u::::u     s::::::s       i::::i o::::o     o::::o  n::::n    n::::n            3:::::3\n");
	klog("  F:::::F             u::::u    u::::u        s::::::s    i::::i o::::o     o::::o  n::::n    n::::n            3:::::3\n");
	klog("  F:::::F             u:::::uuuu:::::u  ssssss   s:::::s  i::::i o::::o     o::::o  n::::n    n::::n            3:::::3\n");
	klog("FF:::::::FF           u:::::::::::::::uus:::::ssss::::::si::::::io:::::ooooo:::::o  n::::n    n::::n3333333     3:::::3\n");
	klog("F::::::::FF            u:::::::::::::::us::::::::::::::s i::::::io:::::::::::::::o  n::::n    n::::n3::::::33333::::::3\n");
	klog("F::::::::FF             uu::::::::uu:::u s:::::::::::ss  i::::::i oo:::::::::::oo   n::::n    n::::n3:::::::::::::::33 \n");
	klog("FFFFFFFFFFF               uuuuuuuu  uuuu  sssssssssss    iiiiiiii   ooooooooooo     nnnnnn    nnnnnn 333333333333333   \n");
	klog("\n");
	klog("#######################################################################################################################\n");
}

int _main(void)
{
	ResolveDynlib();
	ascii();

	klog("Installing Kernel ELF\n");
	syscall(11, install_kernel, _binary_resources_Kernel_elf_start, _binary_resources_Kernel_elf_end - _binary_resources_Kernel_elf_start);

	klog("All Done\n");

	return 0;
}