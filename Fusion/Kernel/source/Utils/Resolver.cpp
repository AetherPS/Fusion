#include "Common.h"
#include "Resolver.h"
#include "Offsets.h"

/* Util */
struct sysentvec* sysvec = NULL;
struct prison* prison0 = NULL;
struct vnode* rootvnode = NULL;
int (*copyout)(const void* kaddr, void* udaddr, size_t len) = NULL;
int (*copyin)(const void* uaddr, void* kaddr, size_t len) = NULL;
int (*copyout_nofault)(const void* kaddr, void* udaddr, size_t len) = NULL;
int (*copyin_nofault)(const void* uaddr, void* kaddr, size_t len) = NULL;
int (*copyinstr)(const void* uaddr, void* kaddr, size_t len, size_t*) = NULL;
int (*kern_open)(struct thread* td, const char* path, int pathseg, int flags, int mode) = NULL;
int (*kern_mkdir)(struct thread* td, char* path, int pathseg, int mode) = NULL;
vm_map_t kernel_map = NULL;
vm_offset_t(*kmem_alloc)(vm_map_t map, vm_size_t size) = NULL;
void(*kmem_free)(void* map, void* addr, size_t size) = NULL;
int (*vn_fullpath)(struct thread* td, struct vnode* vp, char** retbuf, char** freebuf) = NULL;
int (*fuse_loader)(void* m, int op, void* arg) = NULL;
void (*dmamini_initialize_ioctl)() = NULL;

/* STD Lib */
void* M_TEMP = NULL;
void* M_MOUNT = NULL;
void* (*malloc)(unsigned long size, void* type, int flags) = NULL;
void (*free)(void* addr, void* type) = NULL;
void (*memcpy)(void* dst, const void* src, size_t len) = NULL;
void* (*memset)(void* ptr, int value, size_t num) = NULL;
int (*memcmp)(const void* ptr1, const void* ptr2, size_t num) = NULL;
size_t(*strlen)(const char* str) = NULL;
int (*strcpy)(char* str1, char* str2) = NULL;
char* (*strncpy)(char* destination, const char* source, size_t num) = NULL;
int (*strcmp)(const char* str1, const char* str2) = NULL;
char* (*strstr)(const char* str1, const char* str2) = NULL;
int (*sprintf)(char* dst, const char* fmt, ...) = NULL;
int (*snprintf)(char* str, size_t size, const char* format, ...) = NULL;
int (*vsprintf)(char* dst, const char* fmt, va_list ap) = NULL;
int (*vprintf)(const char* fmt, va_list arg) = NULL;
int (*sscanf)(const char* str, const char* format, ...) = NULL;
char* (*strdup)(const char* s, void* type) = NULL;
char* (*realloc)(void* addr, unsigned long size, void* mtp, int flags) = NULL;
void (*printf)(const char* fmt, ...) = NULL;
void (*hexdump)(const void* ptr, int length, const char* hdr, int flags) = NULL;
bool (*dynlib_is_host_path)(char* s) = NULL;
char* (*dynlib_basename)(char* s) = NULL;
char* (*dynlib_basename_host)(char* s) = NULL;

/* Event Handling */
eventhandler_tag(*eventhandler_register)(struct eventhandler_list* list, const char* name, void* func, const char* unk, void* arg, int priority) = NULL;
void (*eventhandler_deregister)(struct eventhandler_list* a, struct eventhandler_entry* b) = NULL;
struct eventhandler_list* (*eventhandler_find_list)(const char* name) = NULL;

/* Proc */
struct proclist* allproc = NULL;
struct sx* allproc_lock = NULL;
struct proc* (*pfind)(int pid) = NULL;
int (*proc_rwmem)(struct proc* p, struct uio* uio) = NULL;
int (*create_thread)(struct thread* td, uint64_t ctx, void* start_func, void* arg, char* stack_base, size_t stack_size, char* tls_base, long* child_tid, long* parent_tid, uint64_t flags, uint64_t rtp) = NULL;
void* (*do_dlsym)(struct dynlib* dl, struct dynlib_obj* obj, char* name, char* libName, unsigned int flags) = NULL;
struct dynlib_obj* (*find_obj_by_handle)(struct dynlib* dl, int handle) = NULL;

/* Fake Selfs */
int (*sceSblAuthMgrGetSelfInfo)(void* ctx, void* exInfo) = NULL;
void (*sceSblAuthMgrSmStart)(void**) = NULL;
int (*sceSblAuthMgrVerifyHeader)(void* ctx) = NULL;

/* Fake Pkgs */
void* fpu_ctx = NULL;
int (*fpu_kern_enter)(struct thread* td, void* ctx, uint32_t flags) = NULL;
int (*fpu_kern_leave)(struct thread* td, void* ctx) = NULL;
void (*Sha256Hmac)(uint8_t hash[0x20], const uint8_t* data, size_t data_size, const uint8_t* key, int key_size) = NULL;
int (*sceSblDriverSendMsg)(void* msg, size_t size) = NULL;
int (*sceSblPfsSetKeys)(uint32_t* p_Ekh, uint32_t* p_Skh, uint8_t* p_Eekpfs, void* p_Eekc, unsigned int p_PubkeyVer, unsigned int p_KeyVer, void* p_Header, size_t p_HeaderSize, unsigned int p_Type, unsigned int p_Finalized, unsigned int p_IsDisc) = NULL;
int (*RsaesPkcs1v15Dec2048CRT)(void* out, void* in, void* key) = NULL;
int (*AesCbcCfb128Encrypt)(uint8_t* out, const uint8_t* in, size_t data_size, const uint8_t* key, int key_size, uint8_t* iv) = NULL;
int (*AesCbcCfb128Decrypt)(uint8_t* out, const uint8_t* in, size_t data_size, const uint8_t* key, int key_size, uint8_t* iv) = NULL;
int (*sceSblKeymgrSetKeyForPfs)(void* key, unsigned int* handle) = NULL;
int (*sceSblKeymgrClearKey)(uint32_t kh) = NULL;
int (*sceSblKeymgrSetKeyStorage)(uint64_t key_gpu_va, unsigned int key_size, uint32_t key_id, uint32_t key_handle) = NULL;

/* Misc Homebrew */
struct mtx* sbl_drv_msg_mtx = NULL;
uint64_t gpu_va_page_list = 0;
uint64_t sbl_keymgr_key_rbtree = 0;
struct sx* sbl_pfs_sx = NULL;
uint64_t* sbl_keymgr_buf_gva = NULL;
uint64_t* sbl_keymgr_buf_va = NULL;
struct _SblKeySlotQueue* sbl_keymgr_key_slots = NULL;
struct _SelfHeader* mini_syscore_self_binary = NULL;

/* Virtual Memory */
void (*vm_map_lock)(struct vm_map* map) = NULL;
void (*vm_map_unlock)(struct vm_map* map) = NULL;
int (*vm_map_findspace)(struct vm_map* map, uint64_t start, uint64_t length, uint64_t* addr) = NULL;
int (*vm_map_delete)(struct vm_map* map, uint64_t start, uint64_t end) = NULL;
int (*vm_map_insert)(struct vm_map* map, uint64_t object, uint64_t offset, uint64_t start, uint64_t end, int prot, int max, int cow) = NULL;
int (*vm_map_protect)(struct vm_map* map, uint64_t start, uint64_t end, int new_prot, bool set_max) = NULL;

/* Mutex Locks */
void (*mtx_lock_flags)(struct mtx* mutex, int flags) = NULL;
void (*mtx_unlock_flags)(struct mtx* mutex, int flags) = NULL;
void (*_mtx_lock_flags)(struct mtx* mutex, int flags, const char* file, int line) = NULL;
void (*_mtx_unlock_flags)(struct mtx* mutex, int flags, const char* file, int line) = NULL;
int (*sx_xlock)(struct sx* sx, int opts) = NULL;
int (*sx_xunlock)(struct sx* sx) = NULL;
int(*sx_slock)(struct sx* sx, int opts, const char* file, int line) = NULL;
int(*sx_sunlock)(struct sx* sx, int opts, const char* file, int line) = NULL;

/* Driver */
int(*make_dev_p)(int _flags, struct cdev** _cdev, struct cdevsw* _devsw, struct ucred* _cr, uid_t _uid, gid_t _gid, int _mode, const char* _fmt, ...) = NULL;
void(*destroy_dev)(struct cdev* _dev) = NULL;
void(*devfs_rule_applyde_recursive)(struct devfs_krule* dk, struct devfs_dirent* de) = NULL;

/* Flash & NVS */
int (*icc_nvs_read)(uint32_t block, uint32_t offset, uint32_t size, uint8_t* value) = NULL;
int (*icc_nvs_write)(uint32_t block, uint32_t offset, uint32_t size, uint8_t* value) = NULL;

/* Sysctl */
struct sysctl_oid_list* sysctl__children = NULL;
void (*sysctl_ctx_init)(struct sysctl_ctx_list* ctx) = NULL;
void (*sysctl_ctx_free)(struct sysctl_ctx_list* ctx) = NULL;
struct sysctl_oid* (*sysctl_add_oid)(struct sysctl_ctx_list* clist, struct sysctl_oid_list* parent, int nbr, const char* name, int kind, void* arg1, intptr_t arg2, int (*handler)(), const char* fmt, const char* descr) = NULL;
int (*sysctl_handle_int)() = NULL;
int (*sysctl_handle_string)() = NULL;

void InitResolver(KernelAddrs* offsets)
{
	/* Util */
	RESOLVE(sysvec);
	RESOLVE(prison0);
	RESOLVE(rootvnode);
	RESOLVE(copyout);
	RESOLVE(copyin);
	RESOLVE(copyout_nofault);
	RESOLVE(copyin_nofault);
	RESOLVE(copyinstr);
	RESOLVE(kern_open);
	RESOLVE(kern_mkdir);
	kernel_map = *(vm_map_t*)(offsets->kernel_map);
	RESOLVE(kmem_alloc);
	RESOLVE(kmem_free);
	RESOLVE(vn_fullpath);
	RESOLVE(fuse_loader);
	RESOLVE(dmamini_initialize_ioctl);

	/* STD Lib */
	RESOLVE(M_TEMP);
	RESOLVE(M_MOUNT);
	RESOLVE(malloc);
	RESOLVE(free);
	RESOLVE(memcpy);
	RESOLVE(memset);
	RESOLVE(memcmp);
	RESOLVE(strlen);
	RESOLVE(strcpy);
	RESOLVE(strncpy);
	RESOLVE(strcmp);
	RESOLVE(strstr);
	RESOLVE(sprintf);
	RESOLVE(snprintf);
	RESOLVE(vsprintf);
	RESOLVE(vprintf);
	RESOLVE(sscanf);
	RESOLVE(strdup);
	RESOLVE(realloc);
	RESOLVE(printf);
	RESOLVE(hexdump);
	RESOLVE(dynlib_is_host_path);
	RESOLVE(dynlib_basename);
	RESOLVE(dynlib_basename_host);

	/* Event Handling */
	RESOLVE(eventhandler_register);
	RESOLVE(eventhandler_deregister);
	RESOLVE(eventhandler_find_list);

	/* Proc */
	RESOLVE(allproc);
	RESOLVE(allproc_lock);
	RESOLVE(pfind);
	RESOLVE(proc_rwmem);
	RESOLVE(create_thread);
	RESOLVE(do_dlsym);
	RESOLVE(find_obj_by_handle);

	/* Fake Selfs */
	RESOLVE(sceSblAuthMgrGetSelfInfo);
	RESOLVE(sceSblAuthMgrSmStart);
	RESOLVE(sceSblAuthMgrVerifyHeader);

	/* Fake Pkgs */
	RESOLVE(fpu_ctx);
	RESOLVE(fpu_kern_enter);
	RESOLVE(fpu_kern_leave);
	RESOLVE(Sha256Hmac);
	RESOLVE(sceSblDriverSendMsg);
	RESOLVE(sceSblPfsSetKeys);
	RESOLVE(RsaesPkcs1v15Dec2048CRT);
	RESOLVE(AesCbcCfb128Encrypt);
	RESOLVE(AesCbcCfb128Decrypt);
	RESOLVE(sceSblKeymgrSetKeyForPfs);
	RESOLVE(sceSblKeymgrClearKey);
	RESOLVE(sceSblKeymgrSetKeyStorage);

	/* Misc Homebrew */
	RESOLVE(sbl_drv_msg_mtx);
	RESOLVE(gpu_va_page_list);
	RESOLVE(sbl_keymgr_key_rbtree);
	RESOLVE(sbl_pfs_sx);
	RESOLVE(sbl_keymgr_buf_gva);
	RESOLVE(sbl_keymgr_buf_va);
	RESOLVE(sbl_keymgr_key_slots);
	RESOLVE(mini_syscore_self_binary);

	/* Virtual Memory */
	RESOLVE(vm_map_lock);
	RESOLVE(vm_map_unlock);
	RESOLVE(vm_map_findspace);
	RESOLVE(vm_map_delete);
	RESOLVE(vm_map_insert);
	RESOLVE(vm_map_protect);

	/* Mutex Locks */
	RESOLVE(mtx_lock_flags);
	RESOLVE(mtx_unlock_flags);
	_mtx_lock_flags = decltype(_mtx_lock_flags)(mtx_lock_flags);
	_mtx_unlock_flags = decltype(_mtx_unlock_flags)(mtx_unlock_flags);
	RESOLVE(sx_xlock);
	RESOLVE(sx_xunlock);
	RESOLVE(sx_slock);
	RESOLVE(sx_sunlock);

	/* Driver */
	RESOLVE(make_dev_p);
	RESOLVE(destroy_dev);
	RESOLVE(devfs_rule_applyde_recursive);

	/* Flash & NVS */
	RESOLVE(icc_nvs_read);
	RESOLVE(icc_nvs_write);

	/* Sysctl */
	RESOLVE(sysctl__children);
	RESOLVE(sysctl_ctx_init);
	RESOLVE(sysctl_ctx_free);
	RESOLVE(sysctl_add_oid);
	RESOLVE(sysctl_handle_int);
	RESOLVE(sysctl_handle_string);
}