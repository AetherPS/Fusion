#include "Common.h"
#include "Resolver.h"

#define NATIVE_RESOLVE(_Ty) _Ty = (decltype(_Ty))(void*)((uint8_t *)&KernelBase[addr_ ## _Ty]);

/* Util */
sysentvec* sysvec;
prison* prison0;
vnode* rootvnode;
int (*copyout)(const void* kaddr, void* udaddr, size_t len) = nullptr;
int (*copyin)(const void* uaddr, void* kaddr, size_t len) = nullptr;
int (*copyinstr)(const void* uaddr, void* kaddr, size_t len, size_t*) = nullptr;
int (*kern_open)(thread* td, const char* path, int pathseg, int flags, int mode) = nullptr;
int (*kern_mkdir)(thread* td, char* path, int pathseg, int mode) = nullptr;
vm_map_t kernel_map;
vm_offset_t(*kmem_alloc)(vm_map_t map, vm_size_t size);
void(*kmem_free)(void* map, void* addr, size_t size) = nullptr;
int (*vn_fullpath)(struct thread* td, struct vnode* vp, char** retbuf, char** freebuf) = nullptr;
int (*fuse_loader)(void* m, int op, void* arg) = nullptr;
void (*dmamini_initialize_ioctl)() = nullptr;

/* STD Lib */
void* M_TEMP = 0;
void* M_MOUNT = 0;
void* (*malloc)(unsigned long size, void* type, int flags) = 0;
void (*free)(void* addr, void* type) = 0;
void (*memcpy)(void* dst, const void* src, size_t len) = 0;
void* (*memset)(void* ptr, int value, size_t num) = 0;
int (*memcmp)(const void* ptr1, const void* ptr2, size_t num) = 0;
size_t(*strlen)(const char* str) = 0;
int (*strcpy)(char* str1, char* str2) = 0;
char* (*strncpy)(char* destination, const char* source, size_t num) = 0;
int (*strcmp)(const char* str1, const char* str2) = 0;
char* (*strstr)(const char* str1, const char* str2) = 0;
int (*sprintf)(char* dst, const char* fmt, ...) = 0;
int (*snprintf)(char* str, size_t size, const char* format, ...) = 0;
int (*vsprintf)(char* dst, const char* fmt, va_list ap);
int (*vprintf)(const char* fmt, va_list arg) = 0;
int(*sscanf)(const char* str, const char* format, ...) = 0;
char* (*strdup)(const char* s, void* type) = 0;
char* (*realloc)(void* addr, unsigned long size, void* mtp, int flags) = 0;
void(*kprintf)(const char* fmt, ...) = nullptr;
void(*hexdump)(const void* ptr, int length, const char* hdr, int flags) = nullptr;
bool (*dynlib_is_host_path)(char* s) = nullptr;
char* (*dynlib_basename)(char* s) = nullptr;
char* (*dynlib_basename_host)(char* s) = nullptr;

/* Event Handling */
#if SOFTWARE_VERSION <= 505
eventhandler_tag(*eventhandler_register)(eventhandler_list* list, const char* name, void* func, void* arg, int priority);
#elif SOFTWARE_VERSION >= 672
eventhandler_tag(*eventhandler_register)(eventhandler_list* list, const char* name, void* func, const char* unk, void* arg, int priority);
#endif
void (*eventhandler_deregister)(eventhandler_list* a, eventhandler_entry* b);
eventhandler_list* (*eventhandler_find_list)(const char* name);

/* Proc */
proclist* allproc = 0;
sx* allproc_lock = 0;
proc* (*pfind)(int pid);
int (*proc_rwmem)(proc* p, uio* uio) = 0;
int (*create_thread)(thread* td, uint64_t ctx, void* start_func, void* arg, char* stack_base, size_t stack_size, char* tls_base, long* child_tid, long* parent_tid, uint64_t flags, uint64_t rtp) = 0;
void* (*do_dlsym)(dynlib* dl, dynlib_obj* obj, char* name, char* libName, unsigned int flags) = 0;
dynlib_obj* (*find_obj_by_handle)(dynlib* dl, int handle) = 0;

/* Fake Selfs */
int (*sceSblAuthMgrGetSelfInfo)(SelfContext* ctx, void* exInfo) = nullptr;
void (*sceSblAuthMgrSmStart)(void**) = nullptr;
int (*sceSblAuthMgrVerifyHeader)(SelfContext* ctx) = nullptr;

/* Fake Pkgs */
fpu_kern_ctx* fpu_ctx;
int (*fpu_kern_enter)(thread* td, fpu_kern_ctx* ctx, uint32_t flags);
int (*fpu_kern_leave)(thread* td, fpu_kern_ctx* ctx);
void (*Sha256Hmac)(uint8_t hash[0x20], const uint8_t* data, size_t data_size, const uint8_t* key, int key_size);
int (*sceSblDriverSendMsg)(SblMsg* msg, size_t size);
int (*sceSblPfsSetKeys)(uint32_t* p_Ekh, uint32_t* p_Skh, uint8_t* p_Eekpfs, Ekc* p_Eekc, unsigned int p_PubkeyVer, unsigned int p_KeyVer, PfsHeader* p_Header, size_t p_HeaderSize, unsigned int p_Type, unsigned int p_Finalized, unsigned int p_IsDisc);
int (*RsaesPkcs1v15Dec2048CRT)(RsaBuffer* out, RsaBuffer* in, RsaKey* key);
int (*AesCbcCfb128Encrypt)(uint8_t* out, const uint8_t* in, size_t data_size, const uint8_t* key, int key_size, uint8_t* iv);
int (*AesCbcCfb128Decrypt)(uint8_t* out, const uint8_t* in, size_t data_size, const uint8_t* key, int key_size, uint8_t* iv);
int (*sceSblKeymgrSetKeyForPfs)(SblKeyDesc* key, unsigned int* handle);
int (*sceSblKeymgrClearKey)(uint32_t kh);
int (*sceSblKeymgrSetKeyStorage)(uint64_t key_gpu_va, unsigned int key_size, uint32_t key_id, uint32_t key_handle);

/* Misc Homebrew */
mtx* sbl_drv_msg_mtx;
uint64_t gpu_va_page_list;
uint64_t sbl_keymgr_key_rbtree;
sx* sbl_pfs_sx;
uint64_t* sbl_keymgr_buf_gva;
uint64_t* sbl_keymgr_buf_va;
_SblKeySlotQueue* sbl_keymgr_key_slots;
SelfHeader* mini_syscore_self_binary;

/* Virtual Memory */
void (*vm_map_lock)(vm_map* map);
void (*vm_map_unlock)(vm_map* map);
int (*vm_map_findspace)(vm_map* map, uint64_t start, uint64_t length, uint64_t* addr);
int (*vm_map_delete)(vm_map* map, uint64_t start, uint64_t end);
int (*vm_map_insert)(vm_map* map, uint64_t object, uint64_t offset, uint64_t start, uint64_t end, int prot, int max, int cow);
int (*vm_map_protect)(vm_map* map, uint64_t start, uint64_t end, int new_prot, bool set_max);

/* Mutex Locks */
void (*mtx_lock_flags)(mtx* mutex, int flags);
void (*mtx_unlock_flags)(mtx* mutex, int flags);
void (*_mtx_lock_flags)(mtx* mutex, int flags, const char* file, int line);
void (*_mtx_unlock_flags)(mtx* mutex, int flags, const char* file, int line);
int (*sx_xlock)(struct sx* sx, int opts);
int (*sx_xunlock)(struct sx* sx);
int(*sx_slock)(struct sx* sx, int opts, const char* file, int line);
int(*sx_sunlock)(struct sx* sx, int opts, const char* file, int line);

/* Driver */
int(*make_dev_p)(int _flags, cdev** _cdev, cdevsw* _devsw, ucred* _cr, uid_t _uid, gid_t _gid, int _mode, const char* _fmt, ...) = nullptr;
void(*destroy_dev)(cdev* _dev) = nullptr;
void(*devfs_rule_applyde_recursive)(struct devfs_krule* dk, struct devfs_dirent* de) = nullptr;

void ResolveFunctions()
{
    /* Util */
    NATIVE_RESOLVE(sysvec);
    NATIVE_RESOLVE(prison0);
    NATIVE_RESOLVE(rootvnode);
    NATIVE_RESOLVE(copyin);
    NATIVE_RESOLVE(copyout);
	NATIVE_RESOLVE(copyinstr);
    NATIVE_RESOLVE(kern_open);
    NATIVE_RESOLVE(kern_mkdir);
    kernel_map = *(vm_map_t*)(KernelBase + addr_kernel_map);
    NATIVE_RESOLVE(kmem_alloc);
    NATIVE_RESOLVE(kmem_free);
    NATIVE_RESOLVE(vn_fullpath);
    NATIVE_RESOLVE(fuse_loader);
	NATIVE_RESOLVE(dmamini_initialize_ioctl);

    /* STD Lib */
    NATIVE_RESOLVE(M_TEMP);
    NATIVE_RESOLVE(M_MOUNT);
    NATIVE_RESOLVE(malloc);
    NATIVE_RESOLVE(free);
    NATIVE_RESOLVE(memcpy);
    NATIVE_RESOLVE(memset);
    NATIVE_RESOLVE(memcmp);
    NATIVE_RESOLVE(strlen);
    NATIVE_RESOLVE(strcpy);
    NATIVE_RESOLVE(strncpy);
    NATIVE_RESOLVE(strcmp);
    NATIVE_RESOLVE(strstr);
    NATIVE_RESOLVE(sprintf);
    NATIVE_RESOLVE(snprintf);
    NATIVE_RESOLVE(vsprintf);
    NATIVE_RESOLVE(vprintf);
    NATIVE_RESOLVE(sscanf);
    NATIVE_RESOLVE(strdup);
    NATIVE_RESOLVE(realloc);
    NATIVE_RESOLVE(kprintf);
	NATIVE_RESOLVE(hexdump);
	NATIVE_RESOLVE(dynlib_is_host_path);
	NATIVE_RESOLVE(dynlib_basename);
	NATIVE_RESOLVE(dynlib_basename_host);

    /* Event Handling */
    NATIVE_RESOLVE(eventhandler_register);
    NATIVE_RESOLVE(eventhandler_deregister);
    NATIVE_RESOLVE(eventhandler_find_list);

    /* Proc */
    NATIVE_RESOLVE(allproc);
	NATIVE_RESOLVE(allproc_lock);
    NATIVE_RESOLVE(pfind);
    NATIVE_RESOLVE(proc_rwmem);
    NATIVE_RESOLVE(create_thread);
    NATIVE_RESOLVE(do_dlsym);
    NATIVE_RESOLVE(find_obj_by_handle);

    /* Fake Selfs */
    NATIVE_RESOLVE(sceSblAuthMgrGetSelfInfo);
    NATIVE_RESOLVE(sceSblAuthMgrSmStart);
    NATIVE_RESOLVE(sceSblAuthMgrVerifyHeader);

    /* Fake Pkgs */
    NATIVE_RESOLVE(fpu_ctx);
    NATIVE_RESOLVE(fpu_kern_enter);
    NATIVE_RESOLVE(fpu_kern_leave);
    NATIVE_RESOLVE(Sha256Hmac);
    NATIVE_RESOLVE(sceSblDriverSendMsg);
    NATIVE_RESOLVE(sceSblPfsSetKeys);
    NATIVE_RESOLVE(RsaesPkcs1v15Dec2048CRT);
    NATIVE_RESOLVE(AesCbcCfb128Encrypt);
    NATIVE_RESOLVE(AesCbcCfb128Decrypt);
    NATIVE_RESOLVE(sceSblKeymgrSetKeyForPfs);
    NATIVE_RESOLVE(sceSblKeymgrClearKey);
    NATIVE_RESOLVE(sceSblKeymgrSetKeyStorage);

    /* Misc Homebrew */
    NATIVE_RESOLVE(sbl_drv_msg_mtx);
    NATIVE_RESOLVE(gpu_va_page_list);
    NATIVE_RESOLVE(sbl_keymgr_key_rbtree);
    NATIVE_RESOLVE(sbl_pfs_sx);
    NATIVE_RESOLVE(sbl_keymgr_buf_gva);
    NATIVE_RESOLVE(sbl_keymgr_buf_va);
    NATIVE_RESOLVE(sbl_keymgr_key_slots);
    NATIVE_RESOLVE(mini_syscore_self_binary);

    /* Virtual Memory */
    NATIVE_RESOLVE(vm_map_lock);
    NATIVE_RESOLVE(vm_map_unlock);
    NATIVE_RESOLVE(vm_map_findspace);
    NATIVE_RESOLVE(vm_map_delete);
    NATIVE_RESOLVE(vm_map_insert);
    NATIVE_RESOLVE(vm_map_protect);

    /* Mutex Locks */
    NATIVE_RESOLVE(mtx_lock_flags);
    NATIVE_RESOLVE(mtx_unlock_flags);
    _mtx_lock_flags = decltype(_mtx_lock_flags)(mtx_lock_flags);
    _mtx_unlock_flags = decltype(_mtx_unlock_flags)(mtx_unlock_flags);
    NATIVE_RESOLVE(sx_xlock);
    NATIVE_RESOLVE(sx_xunlock);
	NATIVE_RESOLVE(sx_slock);
	NATIVE_RESOLVE(sx_sunlock);

    /* Driver */
    NATIVE_RESOLVE(make_dev_p);
    NATIVE_RESOLVE(destroy_dev);
	NATIVE_RESOLVE(devfs_rule_applyde_recursive);
}