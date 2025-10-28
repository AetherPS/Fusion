#include "common.h"
#include "kernel.h"

void* M_TEMP;
void* (*kernel_malloc)(unsigned long size, void* type, int flags);
void (*kernel_free)(void* addr, void* type);
void (*kernel_memcpy)(void* dst, const void* src, size_t len);
void* (*kernel_memset)(void* ptr, int value, size_t num);
int (*kernel_memcmp)(const void* ptr1, const void* ptr2, size_t num);
void(*kernel_printf)(const char* fmt, ...);
vm_offset_t(*kmem_alloc)(vm_map_t map, vm_size_t size);
void(*kmem_free)(void* map, void* addr, size_t size);
vm_map_t kernel_map;

uint64_t kernel_getbase() {
	return __readmsr(0xC0000082) - addr_Xfast_syscall;
}

void* kernel_alloc(uint32_t size) {
	return kernel_malloc(size, M_TEMP, 2);
}

void kernel_dealloc(void* addr) {
	kernel_free(addr, M_TEMP);
}

#define RESOLVE(name, base, offset) \
	name = (void *)(base + offset) \

void kernel_resolve(uint64_t kernbase) {
	RESOLVE(M_TEMP, kernbase, addr_M_TEMP);
	RESOLVE(kernel_malloc, kernbase, addr_malloc);
	RESOLVE(kernel_free, kernbase, addr_free);
	RESOLVE(kernel_memcpy, kernbase, addr_memcpy);
	RESOLVE(kernel_memset, kernbase, addr_memset);
	RESOLVE(kernel_memcmp, kernbase, addr_memcmp);
	RESOLVE(kernel_printf, kernbase, addr_kprintf);

	RESOLVE(kmem_alloc, kernbase, addr_kmem_alloc);
	RESOLVE(kmem_free, kernbase, addr_kmem_free);
	kernel_map = *(vm_map_t*)(kernbase + addr_kernel_map);
}

int install_kernel(struct thread* td, struct installKernelArgs* args)
{
	uint64_t kernbase = kernel_getbase();
	kernel_resolve(kernbase);

	/* ############################ Jailbreak ############################ */
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
	/* ################################################################### */

	/* ############################# Patches ############################# */
	cpu_disable_wp();
	*(uint8_t*)(kernbase + patch_memcpy) = 0xEB;
	*(uint8_t*)(kernbase + patch_kmem_alloc1) = VM_PROT_ALL;
	*(uint8_t*)(kernbase + patch_kmem_alloc2) = VM_PROT_ALL;
	cpu_enable_wp();
	/* ################################################################### */

	/* ######################## Load Kernel Payload ###################### */
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

	kernel_printf("Starting Kernel. (Entry: %llX, ELFBase: %llX, Size: %i)\n", payload_entry, payloadbase, s);

	if (payload_entry(0))
	{
		return 1;
	}
	/* ################################################################### */

	return 0;
}

void LoadKernel()
{
	syscall(11, install_kernel, _binary_resources_Kernel_elf_start, _binary_resources_Kernel_elf_end - _binary_resources_Kernel_elf_start);
}