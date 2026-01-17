#pragma once

#include <stdint.h>

static inline void InitKernel1300(uint64_t kernelBase, KernelAddrs* addrs)
{
    /* Util */
    addrs->Xfast_syscall = (void*)(kernelBase + 0x000001C0);
    addrs->sysvec = (void*)(kernelBase + 0x01A7CFE8);
    addrs->prison0 = (void*)(kernelBase + 0x0111FA18);
    addrs->rootvnode = (void*)(kernelBase + 0x02136E90);
    addrs->copyout = (void*)(kernelBase + 0x002BD5E0);
    addrs->copyin = (void*)(kernelBase + 0x002BD6D0);
    addrs->copyout_nofault = (void*)(kernelBase + 0x0036C6C0);
    addrs->copyin_nofault = (void*)(kernelBase + 0x0036C680);
    addrs->copyinstr = (void*)(kernelBase + 0x002BDB80);
    addrs->kern_open = (void*)(kernelBase + 0x003435D0);
    addrs->kern_mkdir = (void*)(kernelBase + 0x00348710);
    addrs->kernel_map = (void*)(kernelBase + 0x022D1D50);
    addrs->kmem_alloc = (void*)(kernelBase + 0x00465A40);
    addrs->kmem_free = (void*)(kernelBase + 0x00465C10);
    addrs->vn_fullpath = (void*)(kernelBase + 0x00308CD0);
    addrs->fuse_loader = (void*)(kernelBase + 0x004953C0);
    addrs->DirectMemoryHook = (void*)(kernelBase + 0x00283D40);
    addrs->devact_onioctl_hook = (void*)(kernelBase + 0x00638980);
    addrs->dipsw_onioctl_hook = (void*)(kernelBase + 0x00655A30);
    addrs->sceKernelCheckDipsw_Hook = (void*)(kernelBase + 0x00655120);
    addrs->dmamini_initialize_ioctl = (void*)(kernelBase + 0x005C9710);
    addrs->trapHook = (void*)(kernelBase + 0x0);
    addrs->trap_fatalHook = (void*)(kernelBase + 0x0);
    addrs->QAFlags = kernelBase + 0x021CC5D0;
    addrs->getnewvnode = (void*)(kernelBase + 0x0036E2E0);

    /* STD Lib */
    addrs->M_TEMP = (void*)(kernelBase + 0x01520D00);
    addrs->M_MOUNT = (void*)(kernelBase + 0x01A40250);
    addrs->malloc = (void*)(kernelBase + 0x00009520);
    addrs->free = (void*)(kernelBase + 0x000096E0);
    addrs->memcpy = (void*)(kernelBase + 0x002BD4C0);
    addrs->memset = (void*)(kernelBase + 0x001FA1A0);
    addrs->memcmp = (void*)(kernelBase + 0x00394300);
    addrs->strlen = (void*)(kernelBase + 0x0036AB90);
    addrs->strcpy = (void*)(kernelBase + 0x004176E0);
    addrs->strncpy = (void*)(kernelBase + 0x003A82B0);
    addrs->strcmp = (void*)(kernelBase + 0x000B2940);
    addrs->strncmp = (void*)(kernelBase + 0x003C6370);
    addrs->strstr = (void*)(kernelBase + 0x0021CCB0);
    addrs->sprintf = (void*)(kernelBase + 0x002E0680);
    addrs->snprintf = (void*)(kernelBase + 0x002E0740);
    addrs->vsprintf = (void*)(kernelBase + 0x002E0710);
    addrs->vprintf = (void*)(kernelBase + 0x002E04B0);
    addrs->sscanf = (void*)(kernelBase + 0x0043E170);
    addrs->strdup = (void*)(kernelBase + 0x00407850);
    addrs->realloc = (void*)(kernelBase + 0x000097E0);
    addrs->printf = (void*)(kernelBase + 0x002E0440);
    addrs->hexdump = (void*)(kernelBase + 0x002E1D70);
    addrs->dynlib_is_host_path = (void*)(kernelBase + 0x001B8690);
    addrs->dynlib_basename = (void*)(kernelBase + 0x001B8720);
    addrs->dynlib_basename_host = (void*)(kernelBase + 0x001B86F0);

    /* Event Handling */
    addrs->eventhandler_register = (void*)(kernelBase + 0x00224170);
    addrs->eventhandler_deregister = (void*)(kernelBase + 0x00224500);
    addrs->eventhandler_find_list = (void*)(kernelBase + 0x002246F0);

    /* Proc */
    addrs->allproc = (void*)(kernelBase + 0x01B28538);
    addrs->allproc_lock = (void*)(kernelBase + 0x01B284D8);
    addrs->pfind = (void*)(kernelBase + 0x0000EA40);
    addrs->proc_rwmem = (void*)(kernelBase + 0x00366000);
    addrs->create_thread = (void*)(kernelBase + 0x0004C6C0);
    addrs->do_dlsym = (void*)(kernelBase + 0x003BAF60);
    addrs->find_obj_by_handle = (void*)(kernelBase + 0x003BC0E0);

    /* Virtual Memory */
    addrs->vm_map_lock = (void*)(kernelBase + 0x002F6FC0);
    addrs->vm_map_unlock = (void*)(kernelBase + 0x002F7030);
    addrs->vm_map_findspace = (void*)(kernelBase + 0x002FA1D0);
    addrs->vm_map_delete = (void*)(kernelBase + 0x002F9C10);
    addrs->vm_map_insert = (void*)(kernelBase + 0x002F8310);
    addrs->vm_map_protect = (void*)(kernelBase + 0x002FBF70);

    /* Mutex Locks */
    addrs->mtx_lock_flags = (void*)(kernelBase + 0x00378320);
    addrs->mtx_unlock_flags = (void*)(kernelBase + 0x003785D0);
    addrs->sx_xlock = (void*)(kernelBase + 0x000A3840);
    addrs->sx_xunlock = (void*)(kernelBase + 0x000A3A00);
    addrs->sx_slock = (void*)(kernelBase + 0x000A3660);
    addrs->sx_sunlock = (void*)(kernelBase + 0x000A3950);

    /* Driver */
    addrs->make_dev_p = (void*)(kernelBase + 0x0038A970);
    addrs->destroy_dev = (void*)(kernelBase + 0x0038AE90);
    addrs->devfs_rule_applyde_recursive = (void*)(kernelBase + 0x002DEB60);

    /* Flash & NVS */
    addrs->icc_nvs_read = (void*)(kernelBase + 0x000A5BD0);
    addrs->icc_nvs_write = (void*)(kernelBase + 0x000A5A10);

    /* Sysctl */
    addrs->sysctl__children = (void*)(kernelBase + 0x022CC600);
    addrs->sysctl_ctx_init = (void*)(kernelBase + 0x003F95B0);
    addrs->sysctl_ctx_free = (void*)(kernelBase + 0x003F95D0);
    addrs->sysctl_add_oid = (void*)(kernelBase + 0x003F9C10);
    addrs->sysctl_handle_int = (void*)(kernelBase + 0x003FA090);
    addrs->sysctl_handle_string = (void*)(kernelBase + 0x003FA330);

    /* FSelfs */
    addrs->sceSblAuthMgrGetSelfInfo = (void*)(kernelBase + 0x0063D0A0);
    addrs->sceSblAuthMgrSmStart = (void*)(kernelBase + 0x0063DC30);
    addrs->sceSblAuthMgrVerifyHeader = (void*)(kernelBase + 0x0063C8C0);
    addrs->sbl_drv_msg_mtx = (void*)(kernelBase + 0x02647358);
    addrs->gpu_va_page_list = (void*)(kernelBase + 0x02647350);
    addrs->mini_syscore_self_binary = (void*)(kernelBase + 0x0153D6C8);
    addrs->sceSblAuthMgrVerifyHeaderHook1 = (void*)(kernelBase + 0x00642966);
    addrs->sceSblAuthMgrVerifyHeaderHook2 = (void*)(kernelBase + 0x00643649);
    addrs->SceSblAuthMgrIsLoadable2Hook = (void*)(kernelBase + 0x006421CE);
    addrs->SceSblAuthMgrSmLoadSelfSegment_Mailbox = (void*)(kernelBase + 0x00640094);
    addrs->SceSblAuthMgrSmLoadSelfBlock_Mailbox = (void*)(kernelBase + 0x00640CB8);
    addrs->sceSblAuthMgrIsLoadable__sceSblACMgrGetPathId = (void*)(kernelBase + 0x0064207C);

    /* Fake Pkgs */
    addrs->sbl_keymgr_buf_gva = (void*)(kernelBase + 0x0266C808);
    addrs->sbl_keymgr_buf_va = (void*)(kernelBase + 0x0266C000);
    addrs->sbl_keymgr_key_slots = (void*)(kernelBase + 0x02668040);
    addrs->sbl_keymgr_key_rbtree = (void*)(kernelBase + 0x02668050);
    addrs->sbl_pfs_sx = (void*)(kernelBase + 0x0265C080);
    addrs->fpu_ctx = (void*)(kernelBase + 0x026542C0);
    addrs->fpu_kern_enter = (void*)(kernelBase + 0x001E0040);
    addrs->fpu_kern_leave = (void*)(kernelBase + 0x001E0100);
    addrs->Sha256Hmac = (void*)(kernelBase + 0x001F8DA0);
    addrs->sceSblDriverSendMsg = (void*)(kernelBase + 0x0061C030);
    addrs->sceSblPfsSetKeys = (void*)(kernelBase + 0x00626770);
    addrs->RsaesPkcs1v15Dec2048CRT = (void*)(kernelBase + 0x0021BC60);
    addrs->AesCbcCfb128Encrypt = (void*)(kernelBase + 0x00340E90);
    addrs->AesCbcCfb128Decrypt = (void*)(kernelBase + 0x003410C0);
    addrs->sceSblKeymgrSetKeyForPfs = (void*)(kernelBase + 0x0062B0C0);
    addrs->sceSblKeymgrClearKey = (void*)(kernelBase + 0x0062B400);
    addrs->sceSblKeymgrSetKeyStorage = (void*)(kernelBase + 0x006247D0);
    addrs->SceSblDriverSendMsgHook = (void*)(kernelBase + 0x00624875);
    addrs->SceSblPfsSetKeysHook = (void*)(kernelBase + 0x006A2EF9);
    addrs->NpdrmDecryptIsolatedRifHook = (void*)(kernelBase + 0x0064C5D0);
    addrs->NpdrmDecryptRifNewHook = (void*)(kernelBase + 0x0064D39E);
    addrs->SceSblKeymgrInvalidateKeySxXlockHook = (void*)(kernelBase + 0x0062C27D);

    /* Library Replacement */
    addrs->load_prx = (void*)(kernelBase + 0x003B9CE0);

    /* TTY Redirector */
    addrs->cloneuio = (void*)(kernelBase + 0x0036CCE0);
    addrs->console_write = (void*)(kernelBase + 0x0046F9F0);
    addrs->deci_tty_write = (void*)(kernelBase + 0x0048C540);
    addrs->M_IOV = (void*)(kernelBase + 0x01A4A230);
    addrs->console_cdev = (void*)(kernelBase + 0x022D1F30);
    addrs->DeciTTYWriteHook = (void*)(kernelBase + 0x01A7EDD8);

    /* Kernel Patches */
    addrs->patch_memcpy = (void*)(kernelBase + 0x002BD4ED);
    addrs->patch_kmem_alloc1 = (void*)(kernelBase + 0x00465B0C);
    addrs->patch_kmem_alloc2 = (void*)(kernelBase + 0x00465AF4);
    addrs->patch_ASLR = (void*)(kernelBase + 0x00465B14);
    addrs->patch_copyin1 = (void*)(kernelBase + 0x002BD727);
    addrs->patch_copyin2 = (void*)(kernelBase + 0x002BD733);
    addrs->patch_copyout1 = (void*)(kernelBase + 0x002BD632);
    addrs->patch_copyout2 = (void*)(kernelBase + 0x002BD63E);
    addrs->patch_copyinstr1 = (void*)(kernelBase + 0x002BDBD3);
    addrs->patch_copyinstr2 = (void*)(kernelBase + 0x002BDBDF);
    addrs->patch_copyinstr3 = (void*)(kernelBase + 0x002BDC10);
    addrs->patch_swword_lwpid1 = (void*)(kernelBase + 0x002BDA62);
    addrs->patch_swword_lwpid2 = (void*)(kernelBase + 0x002BDA75);
    addrs->patch_ptrace1 = (void*)(kernelBase + 0x003669FD);
    addrs->patch_ptrace2 = (void*)(kernelBase + 0x00366ED1);
    addrs->patch_dynlibPath1 = (void*)(kernelBase + 0x001B842F);
    addrs->patch_dynlibPath2 = (void*)(kernelBase + 0x001B8437);
    addrs->patch_disablepfsSig = (void*)(kernelBase + 0x0069DB00);
    addrs->patch_debugRif1 = (void*)(kernelBase + 0x0064EC20);
    addrs->patch_debugRif2 = (void*)(kernelBase + 0x0064EC50);
    addrs->patch_debugSettings1 = (void*)(kernelBase + 0x004E87A8);
    addrs->patch_debugSettings2 = (void*)(kernelBase + 0x004E986E);
    addrs->patch_mount = (void*)(kernelBase + 0x001512A7);
    addrs->patch_setuid = (void*)(kernelBase + 0x0039153F);
    addrs->patch_sysmap = (void*)(kernelBase + 0x001FA77A);
    addrs->patch_dynlib_dlsym1 = (void*)(kernelBase + 0x001B7758);
    addrs->patch_dynlib_dlsym2 = (void*)(kernelBase + 0x003BD8C0);
    addrs->patch_display_dump = (void*)(kernelBase + 0x001BF205);
    addrs->patch_debuglogs = (void*)(kernelBase + 0x002E0527);
    addrs->patch_fuseLoader = (void*)(kernelBase + 0x004953EE);
    addrs->patch_fuseroot1 = (void*)(kernelBase + 0x0010D066);
    addrs->patch_fuseroot2 = (void*)(kernelBase + 0x0010D07E);
    addrs->patch_mprotect = (void*)(kernelBase + 0x002FC14C);
    addrs->patch_dmamini0 = (void*)(kernelBase + 0x005C972B);
    addrs->patch_dmamini1 = (void*)(kernelBase + 0x005C972F);
    addrs->patch_mdbg_basic = kernelBase + 0x0075CD90;

    /* mdbg Assist Mode offsets */
    uint64_t mdbg_offsets[] = { 0x54, 0x238, 0x1416, 0x2120, 0x2146, 0x216C, 0x2192, 0x21B8, 0x21DE, 0x2204, 0x222A, 0x2250, 0x2276, 0x229C, 0x22C2, 0x22E8, 0x230E, 0x2334, 0x236A, 0x2390, 0x23B6, 0x23DC };
    for (int i = 0; i < 22; i++) {
        addrs->mdbgAssistMode[i] = addrs->patch_mdbg_basic + mdbg_offsets[i];
    }
}
