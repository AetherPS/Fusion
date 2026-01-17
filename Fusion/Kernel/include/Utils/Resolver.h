#pragma once

#include <Types/All.h>

#define RESOLVE(var) var = (decltype(var))(void*)(offsets->var)

#ifdef __cplusplus
extern "C" {
#endif

/* Util */
extern struct sysentvec* sysvec;
extern struct prison* prison0;
extern struct vnode* rootvnode;
extern int (*copyout)(const void* kaddr, void* udaddr, size_t len);
extern int (*copyin)(const void* uaddr, void* kaddr, size_t len);
extern int (*copyout_nofault)(const void* kaddr, void* udaddr, size_t len);
extern int (*copyin_nofault)(const void* uaddr, void* kaddr, size_t len);
extern int (*copyinstr)(const void* uaddr, void* kaddr, size_t len, size_t*);
extern int (*kern_open)(struct thread* td, const char* path, int pathseg, int flags, int mode);
extern int (*kern_mkdir)(struct thread* td, char* path, int pathseg, int mode);
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
extern int (*sscanf)(const char* str, const char* format, ...);
extern char* (*strdup)(const char* s, void* type);
extern char* (*realloc)(void* addr, unsigned long size, void* mtp, int flags);
extern void (*printf)(const char* fmt, ...);
extern void (*hexdump)(const void* ptr, int length, const char* hdr, int flags);
extern bool (*dynlib_is_host_path)(char* s);
extern char* (*dynlib_basename)(char* s);
extern char* (*dynlib_basename_host)(char* s);

/* Event Handling */
extern eventhandler_tag(*eventhandler_register)(struct eventhandler_list* list, const char* name, void* func, const char* unk, void* arg, int priority);
extern void (*eventhandler_deregister)(struct eventhandler_list* a, struct eventhandler_entry* b);
extern struct eventhandler_list* (*eventhandler_find_list)(const char* name);

/* Proc */
extern struct proclist* allproc;
extern struct sx* allproc_lock;
extern struct proc* (*pfind)(int pid);
extern int (*proc_rwmem)(struct proc* p, struct uio* uio);
extern int (*create_thread)(struct thread* td, uint64_t ctx, void* start_func, void* arg, char* stack_base, size_t stack_size, char* tls_base, long* child_tid, long* parent_tid, uint64_t flags, uint64_t rtp);
extern void* (*do_dlsym)(struct dynlib* dl, struct dynlib_obj* obj, char* name, char* libName, unsigned int flags);
extern struct dynlib_obj* (*find_obj_by_handle)(struct dynlib* dl, int handle);

/* Fake Selfs */
extern int (*sceSblAuthMgrGetSelfInfo)(void* ctx, void* exInfo);
extern void (*sceSblAuthMgrSmStart)(void**);
extern int (*sceSblAuthMgrVerifyHeader)(void* ctx);

/* Fake Pkgs */
extern void* fpu_ctx;
extern int (*fpu_kern_enter)(struct thread* td, void* ctx, uint32_t flags);
extern int (*fpu_kern_leave)(struct thread* td, void* ctx);
extern void (*Sha256Hmac)(uint8_t hash[0x20], const uint8_t* data, size_t data_size, const uint8_t* key, int key_size);
extern int (*sceSblDriverSendMsg)(void* msg, size_t size);
extern int (*sceSblPfsSetKeys)(uint32_t* p_Ekh, uint32_t* p_Skh, uint8_t* p_Eekpfs, void* p_Eekc, unsigned int p_PubkeyVer, unsigned int p_KeyVer, void* p_Header, size_t p_HeaderSize, unsigned int p_Type, unsigned int p_Finalized, unsigned int p_IsDisc);
extern int (*RsaesPkcs1v15Dec2048CRT)(void* out, void* in, void* key);
extern int (*AesCbcCfb128Encrypt)(uint8_t* out, const uint8_t* in, size_t data_size, const uint8_t* key, int key_size, uint8_t* iv);
extern int (*AesCbcCfb128Decrypt)(uint8_t* out, const uint8_t* in, size_t data_size, const uint8_t* key, int key_size, uint8_t* iv);
extern int (*sceSblKeymgrSetKeyForPfs)(void* key, unsigned int* handle);
extern int (*sceSblKeymgrClearKey)(uint32_t kh);
extern int (*sceSblKeymgrSetKeyStorage)(uint64_t key_gpu_va, unsigned int key_size, uint32_t key_id, uint32_t key_handle);

/* Misc Homebrew */
extern struct mtx* sbl_drv_msg_mtx;
extern uint64_t gpu_va_page_list;
extern uint64_t sbl_keymgr_key_rbtree;
extern struct sx* sbl_pfs_sx;
extern uint64_t* sbl_keymgr_buf_gva;
extern uint64_t* sbl_keymgr_buf_va;
extern struct _SblKeySlotQueue* sbl_keymgr_key_slots;
extern struct _SelfHeader* mini_syscore_self_binary;

/* Virtual Memory */
extern void (*vm_map_lock)(struct vm_map* map);
extern void (*vm_map_unlock)(struct vm_map* map);
extern int (*vm_map_findspace)(struct vm_map* map, uint64_t start, uint64_t length, uint64_t* addr);
extern int (*vm_map_delete)(struct vm_map* map, uint64_t start, uint64_t end);
extern int (*vm_map_insert)(struct vm_map* map, uint64_t object, uint64_t offset, uint64_t start, uint64_t end, int prot, int max, int cow);
extern int (*vm_map_protect)(struct vm_map* map, uint64_t start, uint64_t end, int new_prot, bool set_max);

/* Mutex Locks */
extern void (*mtx_lock_flags)(struct mtx* mutex, int flags);
extern void (*mtx_unlock_flags)(struct mtx* mutex, int flags);
extern void (*_mtx_lock_flags)(struct mtx* mutex, int flags, const char* file, int line);
extern void (*_mtx_unlock_flags)(struct mtx* mutex, int flags, const char* file, int line);
extern int (*sx_xlock)(struct sx* sx, int opts);
extern int (*sx_xunlock)(struct sx* sx);
extern int(*sx_slock)(struct sx* sx, int opts, const char* file, int line);
extern int(*sx_sunlock)(struct sx* sx, int opts, const char* file, int line);

/* Driver */
extern int(*make_dev_p)(int _flags, struct cdev** _cdev, struct cdevsw* _devsw, struct ucred* _cr, uid_t _uid, gid_t _gid, int _mode, const char* _fmt, ...);
extern void(*destroy_dev)(struct cdev* _dev);
extern void(*devfs_rule_applyde_recursive)(struct devfs_krule* dk, struct devfs_dirent* de);

/* Flash & NVS */
extern int (*icc_nvs_read)(uint32_t block, uint32_t offset, uint32_t size, uint8_t* value);
extern int (*icc_nvs_write)(uint32_t block, uint32_t offset, uint32_t size, uint8_t* value);

/* Sysctl */
extern struct sysctl_oid_list* sysctl__children;
extern void (*sysctl_ctx_init)(struct sysctl_ctx_list* ctx);
extern void (*sysctl_ctx_free)(struct sysctl_ctx_list* ctx);
extern struct sysctl_oid* (*sysctl_add_oid)(struct sysctl_ctx_list* clist, struct sysctl_oid_list* parent, int nbr, const char* name, int kind, void* arg1, intptr_t arg2, int (*handler)(), const char* fmt, const char* descr);
extern int (*sysctl_handle_int)();
extern int (*sysctl_handle_string)();

void InitResolver(KernelAddrs* offsets);

#ifdef __cplusplus
}
#endif