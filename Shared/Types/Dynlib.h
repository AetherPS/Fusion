#pragma once

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
	struct sx bind_lock;
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