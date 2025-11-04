#pragma once
#include "FakeSelfTypes.h"
#include "FakePkgTypes.h"
#include "Types.h"

/* Util */
extern sysentvec* sysvec;
extern prison* prison0;
extern vnode* rootvnode;
extern int (*copyout)(const void* kaddr, void* udaddr, size_t len);
extern int (*copyin)(const void* uaddr, void* kaddr, size_t len);
extern int (*copyinstr)(const void* uaddr, void* kaddr, size_t len, size_t*);
extern int (*kern_open)(thread* td, const char* path, int pathseg, int flags, int mode);
extern int (*kern_mkdir)(thread* td, char* path, int pathseg, int mode);
extern vm_map_t kernel_map;
extern vm_offset_t(*kmem_alloc)(vm_map_t map, vm_size_t size);
extern void(*kmem_free)(void* map, void* addr, size_t size);
extern int (*vn_fullpath)(struct thread* td, struct vnode* vp, char** retbuf, char** freebuf);
extern int (*fuse_loader)(void* m, int op, void* arg);
extern void (*dmamini_initialize_ioctl)();

/* STD Lib */
extern void* M_TEMP;
extern void* M_MOUNT;
extern void* (*malloc)(unsigned long size, void* type, int flags);
extern void (*free)(void* addr, void* type);
extern void (*memcpy)(void* dst, const void* src, size_t len);
extern void* (*memset)(void* ptr, int value, size_t num);
extern int (*memcmp)(const void* ptr1, const void* ptr2, size_t num);
extern size_t(*strlen)(const char* str);
extern int (*strcpy)(char* str1, char* str2);
extern char* (*strncpy)(char* destination, const char* source, size_t num);
extern int (*strcmp)(const char* str1, const char* str2);
extern char* (*strstr)(const char* str1, const char* str2);
extern int (*sprintf)(char* dst, const char* fmt, ...);
extern int (*snprintf)(char* str, size_t size, const char* format, ...);
extern int (*vsprintf)(char* dst, const char* fmt, va_list ap);
extern int (*vprintf)(const char* fmt, va_list arg);
extern int(*sscanf)(const char* str, const char* format, ...);
extern char* (*strdup)(const char* s, void* type);
extern char* (*realloc)(void* addr, unsigned long size, void* mtp, int flags);
extern void(*kprintf)(const char* fmt, ...);

/* Event Handling */
#if SOFTWARE_VERSION <= 505
extern eventhandler_tag(*eventhandler_register)(eventhandler_list* list, const char* name, void* func, void* arg, int priority);
#elif SOFTWARE_VERSION >= 672
extern eventhandler_tag(*eventhandler_register)(eventhandler_list* list, const char* name, void* func, const char* unk, void* arg, int priority);
#endif
extern void (*eventhandler_deregister)(eventhandler_list* a, eventhandler_entry* b);
extern eventhandler_list* (*eventhandler_find_list)(const char* name);

/*
	process_exec
	process_exit

	system_suspend_phase1
	system_resume_phase1
	shutdown_pre_sync
*/

#if SOFTWARE_VERSION <= 505
#define EVENTHANDLER_REGISTER(name, func, arg, priority)		\
	eventhandler_register(NULL, #name, func, arg, priority)
#elif SOFTWARE_VERSION >= 672
#define EVENTHANDLER_REGISTER(name, func, arg, priority)		\
	eventhandler_register(NULL, #name, func, "", arg, priority)
#endif

#define EVENTHANDLER_DEREGISTER(name, tag) 				\
do {									\
	struct eventhandler_list *_el;					\
									\
	if ((_el = eventhandler_find_list(#name)) != NULL)		\
		eventhandler_deregister(_el, tag);			\
} while(0)

/* Proc */
extern proclist* allproc;
extern sx* allproc_lock;
extern proc* (*pfind)(int pid);
extern int (*proc_rwmem)(proc * p, uio * uio);
extern int (*create_thread)(thread * td, uint64_t ctx, void* start_func, void* arg, char* stack_base, size_t stack_size, char* tls_base, long* child_tid, long* parent_tid, uint64_t flags, uint64_t rtp);
extern void* (*do_dlsym)(dynlib * dl, dynlib_obj * obj, char* name, char* libName, unsigned int flags);
extern dynlib_obj* (*find_obj_by_handle)(dynlib * dl, int handle);

/* Fake Selfs */
extern int (*sceSblAuthMgrGetSelfInfo)(SelfContext * ctx, void* exInfo);
extern void (*sceSblAuthMgrSmStart)(void**);
extern int (*sceSblAuthMgrVerifyHeader)(SelfContext * ctx);

/* Fake Pkgs */
extern fpu_kern_ctx* fpu_ctx;
extern int (*fpu_kern_enter)(thread * td, fpu_kern_ctx * ctx, uint32_t flags);
extern int (*fpu_kern_leave)(thread * td, fpu_kern_ctx * ctx);
extern void (*Sha256Hmac)(uint8_t hash[0x20], const uint8_t * data, size_t data_size, const uint8_t * key, int key_size);
extern int (*sceSblDriverSendMsg)(SblMsg * msg, size_t size);
extern int (*sceSblPfsSetKeys)(uint32_t * p_Ekh, uint32_t * p_Skh, uint8_t * p_Eekpfs, Ekc * p_Eekc, unsigned int p_PubkeyVer, unsigned int p_KeyVer, PfsHeader * p_Header, size_t p_HeaderSize, unsigned int p_Type, unsigned int p_Finalized, unsigned int p_IsDisc);
extern int (*RsaesPkcs1v15Dec2048CRT)(RsaBuffer * out, RsaBuffer * in, RsaKey * key);
extern int (*AesCbcCfb128Encrypt)(uint8_t * out, const uint8_t * in, size_t data_size, const uint8_t * key, int key_size, uint8_t * iv);
extern int (*AesCbcCfb128Decrypt)(uint8_t * out, const uint8_t * in, size_t data_size, const uint8_t * key, int key_size, uint8_t * iv);
extern int (*sceSblKeymgrSetKeyForPfs)(SblKeyDesc * key, unsigned int* handle);
extern int (*sceSblKeymgrClearKey)(uint32_t kh);
extern int (*sceSblKeymgrSetKeyStorage)(uint64_t key_gpu_va, unsigned int key_size, uint32_t key_id, uint32_t key_handle);

/* Misc Homebrew */
extern mtx* sbl_drv_msg_mtx;
extern uint64_t gpu_va_page_list;
extern uint64_t sbl_keymgr_key_rbtree;
extern sx* sbl_pfs_sx;
extern uint64_t* sbl_keymgr_buf_gva;
extern uint64_t* sbl_keymgr_buf_va;
extern _SblKeySlotQueue* sbl_keymgr_key_slots;
extern SelfHeader* mini_syscore_self_binary;

/* Virtual Memory */
extern void (*vm_map_lock)(vm_map * map);
extern void (*vm_map_unlock)(vm_map * map);
extern int (*vm_map_findspace)(vm_map * map, uint64_t start, uint64_t length, uint64_t * addr);
extern int (*vm_map_delete)(vm_map * map, uint64_t start, uint64_t end);
extern int (*vm_map_insert)(vm_map * map, uint64_t object, uint64_t offset, uint64_t start, uint64_t end, int prot, int max, int cow);
extern int (*vm_map_protect)(vm_map * map, uint64_t start, uint64_t end, int new_prot, bool set_max);

/* Mutex Locks */
extern void (*mtx_lock_flags)(mtx * mutex, int flags);
extern void (*mtx_unlock_flags)(mtx * mutex, int flags);
extern void (*_mtx_lock_flags)(mtx * mutex, int flags, const char* file, int line);
extern void (*_mtx_unlock_flags)(mtx * mutex, int flags, const char* file, int line);
extern int (*sx_xlock)(struct sx* sx, int opts);
extern int (*sx_xunlock)(struct sx* sx);
extern int(*sx_slock)(struct sx* sx, int opts, const char* file, int line);
extern int(*sx_sunlock)(struct sx* sx, int opts, const char* file, int line);

/* Driver */
extern int(*make_dev_p)(int _flags, cdev * *_cdev, cdevsw * _devsw, ucred * _cr, uid_t _uid, gid_t _gid, int _mode, const char* _fmt, ...);
extern void(*destroy_dev)(cdev * _dev);
extern void(*devfs_rule_applyde_recursive)(struct devfs_krule* dk, struct devfs_dirent* de);

void ResolveFunctions();