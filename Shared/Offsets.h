#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Resolved addresses structure */
typedef struct _KernelAddrs
{
    /* Util */
    void* Xfast_syscall;
    void* sysvec;
    void* prison0;
    void* rootvnode;
    void* copyout;
    void* copyin;
    void* copyout_nofault;
    void* copyin_nofault;
    void* copyinstr;
    void* kern_open;
    void* kern_mkdir;
    void* kernel_map;
    void* kmem_alloc;
    void* kmem_free;
    void* vn_fullpath;
    void* fuse_loader;
    void* DirectMemoryHook;
    void* devact_onioctl_hook;
    void* dipsw_onioctl_hook;
    void* sceKernelCheckDipsw_Hook;
    void* dmamini_initialize_ioctl;
    void* trapHook;
    void* trap_fatalHook;
    uint64_t QAFlags;
    void* getnewvnode;

    /* STD Lib */
    void* M_TEMP;
    void* M_MOUNT;
    void* malloc;
    void* free;
    void* memcpy;
    void* memset;
    void* memcmp;
    void* strlen;
    void* strcpy;
    void* strncpy;
    void* strcmp;
    void* strncmp;
    void* strstr;
    void* sprintf;
    void* snprintf;
    void* vsprintf;
    void* vprintf;
    void* sscanf;
    void* strdup;
    void* realloc;
    void* printf;
    void* hexdump;
    void* dynlib_is_host_path;
    void* dynlib_basename;
    void* dynlib_basename_host;

    /* Event Handling */
    void* eventhandler_register;
    void* eventhandler_deregister;
    void* eventhandler_find_list;

    /* Proc */
    void* allproc;
    void* allproc_lock;
    void* pfind;
    void* proc_rwmem;
    void* create_thread;
    void* do_dlsym;
    void* find_obj_by_handle;

    /* Virtual Memory */
    void* vm_map_lock;
    void* vm_map_unlock;
    void* vm_map_findspace;
    void* vm_map_delete;
    void* vm_map_insert;
    void* vm_map_protect;

    /* Mutex Locks */
    void* mtx_lock_flags;
    void* mtx_unlock_flags;
    void* sx_xlock;
    void* sx_xunlock;
    void* sx_slock;
    void* sx_sunlock;

    /* Driver */
    void* make_dev_p;
    void* destroy_dev;
    void* devfs_rule_applyde_recursive;

    /* Flash & NVS */
    void* icc_nvs_read;
    void* icc_nvs_write;

    /* Sysctl */
    void* sysctl__children;
    void* sysctl_ctx_init;
    void* sysctl_ctx_free;
    void* sysctl_add_oid;
    void* sysctl_handle_int;
    void* sysctl_handle_string;

    /* FSelfs */
    void* sceSblAuthMgrGetSelfInfo;
    void* sceSblAuthMgrSmStart;
    void* sceSblAuthMgrVerifyHeader;
    void* sbl_drv_msg_mtx;
    void* gpu_va_page_list;
    void* mini_syscore_self_binary;
    void* sceSblAuthMgrVerifyHeaderHook1;
    void* sceSblAuthMgrVerifyHeaderHook2;
    void* SceSblAuthMgrIsLoadable2Hook;
    void* SceSblAuthMgrSmLoadSelfSegment_Mailbox;
    void* SceSblAuthMgrSmLoadSelfBlock_Mailbox;
    void* sceSblAuthMgrIsLoadable__sceSblACMgrGetPathId;

    /* Fake Pkgs */
    void* sbl_keymgr_buf_gva;
    void* sbl_keymgr_buf_va;
    void* sbl_keymgr_key_slots;
    void* sbl_keymgr_key_rbtree;
    void* sbl_pfs_sx;
    void* fpu_ctx;
    void* fpu_kern_enter;
    void* fpu_kern_leave;
    void* Sha256Hmac;
    void* sceSblDriverSendMsg;
    void* sceSblPfsSetKeys;
    void* RsaesPkcs1v15Dec2048CRT;
    void* AesCbcCfb128Encrypt;
    void* AesCbcCfb128Decrypt;
    void* sceSblKeymgrSetKeyForPfs;
    void* sceSblKeymgrClearKey;
    void* sceSblKeymgrSetKeyStorage;
    void* SceSblDriverSendMsgHook;
    void* SceSblPfsSetKeysHook;
    void* NpdrmDecryptIsolatedRifHook;
    void* NpdrmDecryptRifNewHook;
    void* SceSblKeymgrInvalidateKeySxXlockHook;

    /* Library Replacement */
    void* load_prx;

    /* TTY Redirector */
    void* cloneuio;
    void* console_write;
    void* deci_tty_write;
    void* M_IOV;
    void* console_cdev;
    void* DeciTTYWriteHook;

    /* Kernel Patches */
    void* patch_memcpy;
    void* patch_kmem_alloc1;
    void* patch_kmem_alloc2;
    void* patch_ASLR;
    void* patch_copyin1;
    void* patch_copyin2;
    void* patch_copyout1;
    void* patch_copyout2;
    void* patch_copyinstr1;
    void* patch_copyinstr2;
    void* patch_copyinstr3;
    void* patch_swword_lwpid1;
    void* patch_swword_lwpid2;
    void* patch_ptrace1;
    void* patch_ptrace2;
    void* patch_dynlibPath1;
    void* patch_dynlibPath2;
    void* patch_disablepfsSig;
    void* patch_debugRif1;
    void* patch_debugRif2;
    void* patch_debugSettings1;
    void* patch_debugSettings2;
    void* patch_mount;
    void* patch_setuid;
    void* patch_sysmap;
    void* patch_dynlib_dlsym1;
    void* patch_dynlib_dlsym2;
    void* patch_display_dump;
    void* patch_debuglogs;
    void* patch_fuseLoader;
    void* patch_fuseroot1;
    void* patch_fuseroot2;
    void* patch_mprotect;
    void* patch_dmamini0;
    void* patch_dmamini1;
    uint64_t patch_mdbg_basic;
    uint64_t mdbgAssistMode[22];
    void* patch_mpage_panic;
    void* patch_vputx_panic;
    void* patch_vm_fault_panic;
} KernelAddrs;

/* Include implementation files */
#include "Offsets-900.h"
#include "Offsets-1202.h"
#include "Offsets-1250.h"
#include "Offsets-1300.h"

static int DetectFirmwareVersion(uint64_t kernelbase)
{
    const char* buildString = (char*)(*(uint64_t*)(kernelbase + 0x130) + 0x80);
    if (buildString == 0 || buildString[0] != 'r')
    {
        return -1;
    }

    int buildNum = 0;
    for (int i = 1; buildString[i] >= '0' && buildString[i] <= '9'; i++)
    {
        buildNum = buildNum * 10 + (buildString[i] - '0');
    }

    switch (buildNum)
    {
    case 178153:
        return 900;
    case 222806:
        return 1202;
    case 223219:
        return 1250;
    case 225997:
        return 1300;
    default:
        return -1;
    }
}

static inline const char* InitializeFirmwareOffsetsWithVersion(KernelAddrs* offsets, uint64_t kernelBase, int firmware)
{
    if (!offsets) return 0;

    switch (firmware)
    {
    case 900:
        InitKernel900(kernelBase, offsets);
        return "9.00";
    case 1202:
        InitKernel1202(kernelBase, offsets);
        return "12.02";
    case 1250:
        InitKernel1250(kernelBase, offsets);
        return "12.50";
    case 1300:
        InitKernel1300(kernelBase, offsets);
        return "13.00";
    default:
        return 0;
    }
}

static inline const char* InitializeFirmwareOffsets(KernelAddrs* offsets, uint64_t kernelBase)
{
    int firmware = DetectFirmwareVersion(kernelBase);

    return InitializeFirmwareOffsetsWithVersion(offsets, kernelBase, firmware);;
}

#ifdef __cplusplus
}
#endif