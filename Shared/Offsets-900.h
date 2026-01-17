#pragma once

#include <stdint.h>

static inline void InitKernel900(uint64_t kernelBase, KernelAddrs* addrs)
{
    /* Util */
    addrs->Xfast_syscall = (void*)(kernelBase + 0x000001C0);
    addrs->sysvec = (void*)(kernelBase + 0x01528E30);
    addrs->prison0 = (void*)(kernelBase + 0x0111F870);
    addrs->rootvnode = (void*)(kernelBase + 0x021EFF20);
    addrs->copyout = (void*)(kernelBase + 0x002715B0);
    addrs->copyin = (void*)(kernelBase + 0x002716A0);
    addrs->copyout_nofault = (void*)(kernelBase + 0x0044E1C0);
    addrs->copyin_nofault = (void*)(kernelBase + 0x0044E180);
    addrs->copyinstr = (void*)(kernelBase + 0x00271B50);
    addrs->kern_open = (void*)(kernelBase + 0x001D9EE0);
    addrs->kern_mkdir = (void*)(kernelBase + 0x001DF020);
    addrs->kernel_map = (void*)(kernelBase + 0x02268D48);
    addrs->kmem_alloc = (void*)(kernelBase + 0x0037BE70);
    addrs->kmem_free = (void*)(kernelBase + 0x0037C040);
    addrs->vn_fullpath = (void*)(kernelBase + 0x002648C0);
    addrs->fuse_loader = (void*)(kernelBase + 0x00490720);
    addrs->DirectMemoryHook = (void*)(kernelBase + 0x0011D250);
    addrs->devact_onioctl_hook = (void*)(kernelBase + 0x006397A0);
    addrs->dipsw_onioctl_hook = (void*)(kernelBase + 0x00656490);
    addrs->sceKernelCheckDipsw_Hook = (void*)(kernelBase + 0x00655B80);
    addrs->dmamini_initialize_ioctl = (void*)(kernelBase + 0x005D1EC0);
    addrs->trapHook = (void*)(kernelBase + 0x0);
    addrs->trap_fatalHook = (void*)(kernelBase + 0x002DF710);
    addrs->QAFlags = kernelBase + 0x02216850;
    addrs->getnewvnode = (void*)(kernelBase + 0x00233880);

    /* STD Lib */
    addrs->M_TEMP = (void*)(kernelBase + 0x015621E0);
    addrs->M_MOUNT = (void*)(kernelBase + 0x015279F0);
    addrs->malloc = (void*)(kernelBase + 0x00301B20);
    addrs->free = (void*)(kernelBase + 0x00301CE0);
    addrs->memcpy = (void*)(kernelBase + 0x002714B0);
    addrs->memset = (void*)(kernelBase + 0x001496C0);
    addrs->memcmp = (void*)(kernelBase + 0x00271E20);
    addrs->strlen = (void*)(kernelBase + 0x0030F450);
    addrs->strcpy = (void*)(kernelBase + 0x00189F80);
    addrs->strncpy = (void*)(kernelBase + 0x0041E380);
    addrs->strcmp = (void*)(kernelBase + 0x0040E700);
    addrs->strncmp = (void*)(kernelBase + 0x00124750);
    addrs->strstr = (void*)(kernelBase + 0x00487AB0);
    addrs->sprintf = (void*)(kernelBase + 0x000B7C70);
    addrs->snprintf = (void*)(kernelBase + 0x000B7D30);
    addrs->vsprintf = (void*)(kernelBase + 0x000B7D00);
    addrs->vprintf = (void*)(kernelBase + 0x000B7AA0);
    addrs->sscanf = (void*)(kernelBase + 0x0026C8D0);
    addrs->strdup = (void*)(kernelBase + 0x00278540);
    addrs->realloc = (void*)(kernelBase + 0x00301DE0);
    addrs->printf = (void*)(kernelBase + 0x000B7A30);
    addrs->hexdump = (void*)(kernelBase + 0x000B9360);
    addrs->dynlib_is_host_path = (void*)(kernelBase + 0x0023C400);
    addrs->dynlib_basename = (void*)(kernelBase + 0x0023C490);
    addrs->dynlib_basename_host = (void*)(kernelBase + 0x0023C460);

    /* Event Handling */
    addrs->eventhandler_register = (void*)(kernelBase + 0x000F8370);
    addrs->eventhandler_deregister = (void*)(kernelBase + 0x000F8700);
    addrs->eventhandler_find_list = (void*)(kernelBase + 0x000F88F0);

    /* Proc */
    addrs->allproc = (void*)(kernelBase + 0x01B946E0);
    addrs->allproc_lock = (void*)(kernelBase + 0x01B94680);
    addrs->pfind = (void*)(kernelBase + 0x00178960);
    addrs->proc_rwmem = (void*)(kernelBase + 0x0041EB00);
    addrs->create_thread = (void*)(kernelBase + 0x001ED670);
    addrs->do_dlsym = (void*)(kernelBase + 0x0021F1E0);
    addrs->find_obj_by_handle = (void*)(kernelBase + 0x00220360);

    /* Virtual Memory */
    addrs->vm_map_lock = (void*)(kernelBase + 0x0007BA30);
    addrs->vm_map_unlock = (void*)(kernelBase + 0x0007BAA0);
    addrs->vm_map_findspace = (void*)(kernelBase + 0x0007EC40);
    addrs->vm_map_delete = (void*)(kernelBase + 0x0007E680);
    addrs->vm_map_insert = (void*)(kernelBase + 0x0007CD80);
    addrs->vm_map_protect = (void*)(kernelBase + 0x000809C0);

    /* Mutex Locks */
    addrs->mtx_lock_flags = (void*)(kernelBase + 0x002EEEB0);
    addrs->mtx_unlock_flags = (void*)(kernelBase + 0x002EF170);
    addrs->sx_xlock = (void*)(kernelBase + 0x0043E610);
    addrs->sx_xunlock = (void*)(kernelBase + 0x0043E7D0);
    addrs->sx_slock = (void*)(kernelBase + 0x0043E430);
    addrs->sx_sunlock = (void*)(kernelBase + 0x0043E710);

    /* Driver */
    addrs->make_dev_p = (void*)(kernelBase + 0x001EF590);
    addrs->destroy_dev = (void*)(kernelBase + 0x001EFAB0);
    addrs->devfs_rule_applyde_recursive = (void*)(kernelBase + 0x002040A0);

    /* Flash & NVS */
    addrs->icc_nvs_read = (void*)(kernelBase + 0x0010B310);
    addrs->icc_nvs_write = (void*)(kernelBase + 0x0010B150);

    /* Sysctl */
    addrs->sysctl__children = (void*)(kernelBase + 0x021F0300);
    addrs->sysctl_ctx_init = (void*)(kernelBase + 0x002A1760);
    addrs->sysctl_ctx_free = (void*)(kernelBase + 0x002A1780);
    addrs->sysctl_add_oid = (void*)(kernelBase + 0x002A1DC0);
    addrs->sysctl_handle_int = (void*)(kernelBase + 0x002A2240);
    addrs->sysctl_handle_string = (void*)(kernelBase + 0x002A24E0);

    /* FSelfs */
    addrs->sceSblAuthMgrGetSelfInfo = (void*)(kernelBase + 0x006441E0);
    addrs->sceSblAuthMgrSmStart = (void*)(kernelBase + 0x0063FEE0);
    addrs->sceSblAuthMgrVerifyHeader = (void*)(kernelBase + 0x00643A00);
    addrs->sbl_drv_msg_mtx = (void*)(kernelBase + 0x02646CB0);
    addrs->gpu_va_page_list = (void*)(kernelBase + 0x02646CA8);
    addrs->mini_syscore_self_binary = (void*)(kernelBase + 0x01579DF8);
    addrs->sceSblAuthMgrVerifyHeaderHook1 = (void*)(kernelBase + 0x00645026);
    addrs->sceSblAuthMgrVerifyHeaderHook2 = (void*)(kernelBase + 0x00645D09);
    addrs->SceSblAuthMgrIsLoadable2Hook = (void*)(kernelBase + 0x0064488E);
    addrs->SceSblAuthMgrSmLoadSelfSegment_Mailbox = (void*)(kernelBase + 0x0064232D);
    addrs->SceSblAuthMgrSmLoadSelfBlock_Mailbox = (void*)(kernelBase + 0x00642F68);
    addrs->sceSblAuthMgrIsLoadable__sceSblACMgrGetPathId = (void*)(kernelBase + 0x0064473C);

    /* Fake Pkgs */
    addrs->sbl_keymgr_buf_gva = (void*)(kernelBase + 0x0264C808);
    addrs->sbl_keymgr_buf_va = (void*)(kernelBase + 0x0264C000);
    addrs->sbl_keymgr_key_slots = (void*)(kernelBase + 0x02648238);
    addrs->sbl_keymgr_key_rbtree = (void*)(kernelBase + 0x02648248);
    addrs->sbl_pfs_sx = (void*)(kernelBase + 0x0264DB40);
    addrs->fpu_ctx = (void*)(kernelBase + 0x026541C0);
    addrs->fpu_kern_enter = (void*)(kernelBase + 0x002196D0);
    addrs->fpu_kern_leave = (void*)(kernelBase + 0x00219790);
    addrs->Sha256Hmac = (void*)(kernelBase + 0x00445060);
    addrs->sceSblDriverSendMsg = (void*)(kernelBase + 0x0061CED0);
    addrs->sceSblPfsSetKeys = (void*)(kernelBase + 0x006252D0);
    addrs->RsaesPkcs1v15Dec2048CRT = (void*)(kernelBase + 0x004628B0);
    addrs->AesCbcCfb128Encrypt = (void*)(kernelBase + 0x001FF2D0);
    addrs->AesCbcCfb128Decrypt = (void*)(kernelBase + 0x001FF500);
    addrs->sceSblKeymgrSetKeyForPfs = (void*)(kernelBase + 0x0061F690);
    addrs->sceSblKeymgrClearKey = (void*)(kernelBase + 0x0061F9D0);
    addrs->sceSblKeymgrSetKeyStorage = (void*)(kernelBase + 0x00624970);
    addrs->SceSblDriverSendMsgHook = (void*)(kernelBase + 0x00624A15);
    addrs->SceSblPfsSetKeysHook = (void*)(kernelBase + 0x006C3EF9);
    addrs->NpdrmDecryptIsolatedRifHook = (void*)(kernelBase + 0x0064E070);
    addrs->NpdrmDecryptRifNewHook = (void*)(kernelBase + 0x0064EE3E);
    addrs->SceSblKeymgrInvalidateKeySxXlockHook = (void*)(kernelBase + 0x0062084D);

    /* Library Replacement */
    addrs->load_prx = (void*)(kernelBase + 0x0021DF60);

    /* TTY Redirector */
    addrs->cloneuio = (void*)(kernelBase + 0x0044E7E0);
    addrs->console_write = (void*)(kernelBase + 0x002D6EB0);
    addrs->deci_tty_write = (void*)(kernelBase + 0x0048CDE0);
    addrs->M_IOV = (void*)(kernelBase + 0x01A792C0);
    addrs->console_cdev = (void*)(kernelBase + 0x021F1128);
    addrs->DeciTTYWriteHook = (void*)(kernelBase + 0x01A7ED98);

    /* Kernel Patches */
    addrs->patch_memcpy = (void*)(kernelBase + 0x002714BD);
    addrs->patch_kmem_alloc1 = (void*)(kernelBase + 0x0037BF3C);
    addrs->patch_kmem_alloc2 = (void*)(kernelBase + 0x0037BF44);
    addrs->patch_ASLR = (void*)(kernelBase + 0x0005F824);
    addrs->patch_copyin1 = (void*)(kernelBase + 0x002716F7);
    addrs->patch_copyin2 = (void*)(kernelBase + 0x00271703);
    addrs->patch_copyout1 = (void*)(kernelBase + 0x00271602);
    addrs->patch_copyout2 = (void*)(kernelBase + 0x0027160E);
    addrs->patch_copyinstr1 = (void*)(kernelBase + 0x00271BA3);
    addrs->patch_copyinstr2 = (void*)(kernelBase + 0x00271BAF);
    addrs->patch_copyinstr3 = (void*)(kernelBase + 0x00271BE0);
    addrs->patch_swword_lwpid1 = (void*)(kernelBase + 0x002719C2);
    addrs->patch_swword_lwpid2 = (void*)(kernelBase + 0x002719D1);
    addrs->patch_ptrace1 = (void*)(kernelBase + 0x0041F4E5);
    addrs->patch_ptrace2 = (void*)(kernelBase + 0x0041F9D1);
    addrs->patch_dynlibPath1 = (void*)(kernelBase + 0x0023C19F);
    addrs->patch_dynlibPath2 = (void*)(kernelBase + 0x0023C1AA);
    addrs->patch_disablepfsSig = (void*)(kernelBase + 0x006885C0);
    addrs->patch_debugRif1 = (void*)(kernelBase + 0x00650430);
    addrs->patch_debugRif2 = (void*)(kernelBase + 0x00650460);
    addrs->patch_debugSettings1 = (void*)(kernelBase + 0x004E8E48);
    addrs->patch_debugSettings2 = (void*)(kernelBase + 0x004EA12F);
    addrs->patch_mount = (void*)(kernelBase + 0x0004ADE7);
    addrs->patch_setuid = (void*)(kernelBase + 0x000019FF);
    addrs->patch_sysmap = (void*)(kernelBase + 0x0016632A);
    addrs->patch_dynlib_dlsym1 = (void*)(kernelBase + 0x0023B67F);
    addrs->patch_dynlib_dlsym2 = (void*)(kernelBase + 0x00221B40);
    addrs->patch_display_dump = (void*)(kernelBase + 0x00028A75);
    addrs->patch_debuglogs = (void*)(kernelBase + 0x000B7B17);
    addrs->patch_fuseLoader = (void*)(kernelBase + 0x0049074E);
    addrs->patch_fuseroot1 = (void*)(kernelBase + 0x00306866);
    addrs->patch_fuseroot2 = (void*)(kernelBase + 0x0030687E);
    addrs->patch_mprotect = (void*)(kernelBase + 0x00080B8B);
    addrs->patch_dmamini0 = (void*)(kernelBase + 0x005D1EDB);
    addrs->patch_dmamini1 = (void*)(kernelBase + 0x005D1EDF);
    addrs->patch_mdbg_basic = kernelBase + 0x0075CD30;

    /* mdbg Assist Mode offsets */
    uint64_t mdbg_offsets[] = { 0x54, 0x238, 0x1416, 0x2120, 0x2146, 0x216C, 0x2192, 0x21B8, 0x21DE, 0x2204, 0x222A, 0x2250, 0x2276, 0x229C, 0x22C2, 0x22E8, 0x230E, 0x2334, 0x236A, 0x2390, 0x23B6, 0x23DC };
    for (int i = 0; i < 22; i++) {
        addrs->mdbgAssistMode[i] = addrs->patch_mdbg_basic + mdbg_offsets[i];
    }
}
