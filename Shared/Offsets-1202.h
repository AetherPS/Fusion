#pragma once

#include <stdint.h>

static inline void InitKernel1202(uint64_t kernelBase, KernelAddrs* addrs)
{
    /* Util */
    addrs->Xfast_syscall = (void*)(kernelBase + 0x000001C0);
    addrs->sysvec = (void*)(kernelBase + 0x01A7CFE8);
    addrs->prison0 = (void*)(kernelBase + 0x0111FA18);
    addrs->rootvnode = (void*)(kernelBase + 0x02136E90);
    addrs->copyout = (void*)(kernelBase + 0x002BD580);
    addrs->copyin = (void*)(kernelBase + 0x002BD670);
    addrs->copyout_nofault = (void*)(kernelBase + 0x0036C660);
    addrs->copyin_nofault = (void*)(kernelBase + 0x0036C620);
    addrs->copyinstr = (void*)(kernelBase + 0x002BDB20);
    addrs->kern_open = (void*)(kernelBase + 0x00343570);
    addrs->kern_mkdir = (void*)(kernelBase + 0x003486B0);
    addrs->kernel_map = (void*)(kernelBase + 0x022D1D50);
    addrs->kmem_alloc = (void*)(kernelBase + 0x004659E0);
    addrs->kmem_free = (void*)(kernelBase + 0x00465BB0);
    addrs->vn_fullpath = (void*)(kernelBase + 0x00308C70);
    addrs->fuse_loader = (void*)(kernelBase + 0x00495360);
    addrs->DirectMemoryHook = (void*)(kernelBase + 0x00283CE0);
    addrs->devact_onioctl_hook = (void*)(kernelBase + 0x006388F0);
    addrs->dipsw_onioctl_hook = (void*)(kernelBase + 0x00655940);
    addrs->sceKernelCheckDipsw_Hook = (void*)(kernelBase + 0x00655030);
    addrs->dmamini_initialize_ioctl = (void*)(kernelBase + 0x005C96B0);
    addrs->trapHook = (void*)(kernelBase + 0x0);
    addrs->trap_fatalHook = (void*)(kernelBase + 0x0);
    addrs->QAFlags = kernelBase + 0x021CC5D0;
    addrs->getnewvnode = (void*)(kernelBase + 0x0036E280);

    /* STD Lib */
    addrs->M_TEMP = (void*)(kernelBase + 0x01520D00);
    addrs->M_MOUNT = (void*)(kernelBase + 0x01A40250);
    addrs->malloc = (void*)(kernelBase + 0x00009520);
    addrs->free = (void*)(kernelBase + 0x000096E0);
    addrs->memcpy = (void*)(kernelBase + 0x002BD480);
    addrs->memset = (void*)(kernelBase + 0x001FA140);
    addrs->memcmp = (void*)(kernelBase + 0x003942A0);
    addrs->strlen = (void*)(kernelBase + 0x0036AB30);
    addrs->strcpy = (void*)(kernelBase + 0x00417680);
    addrs->strncpy = (void*)(kernelBase + 0x003A8250);
    addrs->strcmp = (void*)(kernelBase + 0x000B2940);
    addrs->strncmp = (void*)(kernelBase + 0x003C6310);
    addrs->strstr = (void*)(kernelBase + 0x0021CC50);
    addrs->sprintf = (void*)(kernelBase + 0x002E0620);
    addrs->snprintf = (void*)(kernelBase + 0x002E06E0);
    addrs->vsprintf = (void*)(kernelBase + 0x002E06B0);
    addrs->vprintf = (void*)(kernelBase + 0x002E0450);
    addrs->sscanf = (void*)(kernelBase + 0x0043E110);
    addrs->strdup = (void*)(kernelBase + 0x004077F0);
    addrs->realloc = (void*)(kernelBase + 0x000097E0);
    addrs->printf = (void*)(kernelBase + 0x002E03E0);
    addrs->hexdump = (void*)(kernelBase + 0x002E1D10);
    addrs->dynlib_is_host_path = (void*)(kernelBase + 0x001B8650);
    addrs->dynlib_basename = (void*)(kernelBase + 0x001B86E0);
    addrs->dynlib_basename_host = (void*)(kernelBase + 0x001B86B0);

    /* Event Handling */
    addrs->eventhandler_register = (void*)(kernelBase + 0x00224110);
    addrs->eventhandler_deregister = (void*)(kernelBase + 0x002244A0);
    addrs->eventhandler_find_list = (void*)(kernelBase + 0x00224690);

    /* Proc */
    addrs->allproc = (void*)(kernelBase + 0x01B28538);
    addrs->allproc_lock = (void*)(kernelBase + 0x01B284D8);
    addrs->pfind = (void*)(kernelBase + 0x0000EA40);
    addrs->proc_rwmem = (void*)(kernelBase + 0x00365FA0);
    addrs->create_thread = (void*)(kernelBase + 0x0004C6C0);
    addrs->do_dlsym = (void*)(kernelBase + 0x003BAF00);
    addrs->find_obj_by_handle = (void*)(kernelBase + 0x003BC080);

    /* Virtual Memory */
    addrs->vm_map_lock = (void*)(kernelBase + 0x002F6F60);
    addrs->vm_map_unlock = (void*)(kernelBase + 0x002F6FD0);
    addrs->vm_map_findspace = (void*)(kernelBase + 0x002FA170);
    addrs->vm_map_delete = (void*)(kernelBase + 0x002F9BB0);
    addrs->vm_map_insert = (void*)(kernelBase + 0x002F82B0);
    addrs->vm_map_protect = (void*)(kernelBase + 0x002FBF10);

    /* Mutex Locks */
    addrs->mtx_lock_flags = (void*)(kernelBase + 0x003782C0);
    addrs->mtx_unlock_flags = (void*)(kernelBase + 0x00378570);
    addrs->sx_xlock = (void*)(kernelBase + 0x000A3840);
    addrs->sx_xunlock = (void*)(kernelBase + 0x000A3A00);
    addrs->sx_slock = (void*)(kernelBase + 0x000A3660);
    addrs->sx_sunlock = (void*)(kernelBase + 0x000A3950);

    /* Driver */
    addrs->make_dev_p = (void*)(kernelBase + 0x0038A910);
    addrs->destroy_dev = (void*)(kernelBase + 0x0038AE30);
    addrs->devfs_rule_applyde_recursive = (void*)(kernelBase + 0x002DEB00);

    /* Flash & NVS */
    addrs->icc_nvs_read = (void*)(kernelBase + 0x000A5BD0);
    addrs->icc_nvs_write = (void*)(kernelBase + 0x000A5A10);

    /* Sysctl */
    addrs->sysctl__children = (void*)(kernelBase + 0x022CC600);
    addrs->sysctl_ctx_init = (void*)(kernelBase + 0x003F9550);
    addrs->sysctl_ctx_free = (void*)(kernelBase + 0x003F9570);
    addrs->sysctl_add_oid = (void*)(kernelBase + 0x003F9BB0);
    addrs->sysctl_handle_int = (void*)(kernelBase + 0x003FA030);
    addrs->sysctl_handle_string = (void*)(kernelBase + 0x003FA2D0);

    /* FSelfs */
    addrs->sceSblAuthMgrGetSelfInfo = (void*)(kernelBase + 0x0063CFB0);
    addrs->sceSblAuthMgrSmStart = (void*)(kernelBase + 0x0063DB40);
    addrs->sceSblAuthMgrVerifyHeader = (void*)(kernelBase + 0x0063C7D0);
    addrs->sbl_drv_msg_mtx = (void*)(kernelBase + 0x02647358);
    addrs->gpu_va_page_list = (void*)(kernelBase + 0x02647350);
    addrs->mini_syscore_self_binary = (void*)(kernelBase + 0x0153D6C8);
    addrs->sceSblAuthMgrVerifyHeaderHook1 = (void*)(kernelBase + 0x00642876);
    addrs->sceSblAuthMgrVerifyHeaderHook2 = (void*)(kernelBase + 0x00643559);
    addrs->SceSblAuthMgrIsLoadable2Hook = (void*)(kernelBase + 0x006420DE);
    addrs->SceSblAuthMgrSmLoadSelfSegment_Mailbox = (void*)(kernelBase + 0x0063FF8D);
    addrs->SceSblAuthMgrSmLoadSelfBlock_Mailbox = (void*)(kernelBase + 0x00640BC8);
    addrs->sceSblAuthMgrIsLoadable__sceSblACMgrGetPathId = (void*)(kernelBase + 0x00641F8C);

    /* Fake Pkgs */
    addrs->sbl_keymgr_buf_gva = (void*)(kernelBase + 0x0266C808);
    addrs->sbl_keymgr_buf_va = (void*)(kernelBase + 0x0266C000);
    addrs->sbl_keymgr_key_slots = (void*)(kernelBase + 0x02668040);
    addrs->sbl_keymgr_key_rbtree = (void*)(kernelBase + 0x02668050);
    addrs->sbl_pfs_sx = (void*)(kernelBase + 0x0265C080);
    addrs->fpu_ctx = (void*)(kernelBase + 0x026542C0);
    addrs->fpu_kern_enter = (void*)(kernelBase + 0x001DFFE0);
    addrs->fpu_kern_leave = (void*)(kernelBase + 0x001E00A0);
    addrs->Sha256Hmac = (void*)(kernelBase + 0x001F8D40);
    addrs->sceSblDriverSendMsg = (void*)(kernelBase + 0x0061BFA0);
    addrs->sceSblPfsSetKeys = (void*)(kernelBase + 0x006266E0);
    addrs->RsaesPkcs1v15Dec2048CRT = (void*)(kernelBase + 0x0021BC00);
    addrs->AesCbcCfb128Encrypt = (void*)(kernelBase + 0x00340E30);
    addrs->AesCbcCfb128Decrypt = (void*)(kernelBase + 0x00341060);
    addrs->sceSblKeymgrSetKeyForPfs = (void*)(kernelBase + 0x0062B030);
    addrs->sceSblKeymgrClearKey = (void*)(kernelBase + 0x0062B370);
    addrs->sceSblKeymgrSetKeyStorage = (void*)(kernelBase + 0x00624740);
    addrs->SceSblDriverSendMsgHook = (void*)(kernelBase + 0x006247E5);
    addrs->SceSblPfsSetKeysHook = (void*)(kernelBase + 0x006A2D99);
    addrs->NpdrmDecryptIsolatedRifHook = (void*)(kernelBase + 0x0064C4E0);
    addrs->NpdrmDecryptRifNewHook = (void*)(kernelBase + 0x0064D2AE);
    addrs->SceSblKeymgrInvalidateKeySxXlockHook = (void*)(kernelBase + 0x0062C1ED);

    /* Library Replacement */
    addrs->load_prx = (void*)(kernelBase + 0x003B9C80);

    /* TTY Redirector */
    addrs->cloneuio = (void*)(kernelBase + 0x0036CC80);
    addrs->console_write = (void*)(kernelBase + 0x0046F990);
    addrs->deci_tty_write = (void*)(kernelBase + 0x0048C4E0);
    addrs->M_IOV = (void*)(kernelBase + 0x01A4A230);
    addrs->console_cdev = (void*)(kernelBase + 0x022D1F30);
    addrs->DeciTTYWriteHook = (void*)(kernelBase + 0x01A7EDD8);

    /* Kernel Patches */
    addrs->patch_memcpy = (void*)(kernelBase + 0x002BD48D);
    addrs->patch_kmem_alloc1 = (void*)(kernelBase + 0x00465AAC);
    addrs->patch_kmem_alloc2 = (void*)(kernelBase + 0x00465AB4);
    addrs->patch_ASLR = (void*)(kernelBase + 0x00477C54);
    addrs->patch_copyin1 = (void*)(kernelBase + 0x002BD6C7);
    addrs->patch_copyin2 = (void*)(kernelBase + 0x002BD6D3);
    addrs->patch_copyout1 = (void*)(kernelBase + 0x002BD5D2);
    addrs->patch_copyout2 = (void*)(kernelBase + 0x002BD5DE);
    addrs->patch_copyinstr1 = (void*)(kernelBase + 0x002BDB73);
    addrs->patch_copyinstr2 = (void*)(kernelBase + 0x002BDB7F);
    addrs->patch_copyinstr3 = (void*)(kernelBase + 0x002BDBB0);
    addrs->patch_swword_lwpid1 = (void*)(kernelBase + 0x002BD992);
    addrs->patch_swword_lwpid2 = (void*)(kernelBase + 0x002BD9A1);
    addrs->patch_ptrace1 = (void*)(kernelBase + 0x0036699D);
    addrs->patch_ptrace2 = (void*)(kernelBase + 0x00366E71);
    addrs->patch_dynlibPath1 = (void*)(kernelBase + 0x001B83EF);
    addrs->patch_dynlibPath2 = (void*)(kernelBase + 0x001B83FA);
    addrs->patch_disablepfsSig = (void*)(kernelBase + 0x0069D9A0);
    addrs->patch_debugRif1 = (void*)(kernelBase + 0x0064EB30);
    addrs->patch_debugRif2 = (void*)(kernelBase + 0x0064EB60);
    addrs->patch_debugSettings1 = (void*)(kernelBase + 0x004E8748);
    addrs->patch_debugSettings2 = (void*)(kernelBase + 0x004E980E);
    addrs->patch_mount = (void*)(kernelBase + 0x00151267);
    addrs->patch_setuid = (void*)(kernelBase + 0x003914DF);
    addrs->patch_sysmap = (void*)(kernelBase + 0x001FA71A);
    addrs->patch_dynlib_dlsym1 = (void*)(kernelBase + 0x001B7718);
    addrs->patch_dynlib_dlsym2 = (void*)(kernelBase + 0x003BD860);
    addrs->patch_display_dump = (void*)(kernelBase + 0x001BF1C5);
    addrs->patch_debuglogs = (void*)(kernelBase + 0x002E04C7);
    addrs->patch_fuseLoader = (void*)(kernelBase + 0x0049538E);
    addrs->patch_fuseroot1 = (void*)(kernelBase + 0x0010D066);
    addrs->patch_fuseroot2 = (void*)(kernelBase + 0x0010D07E);
    addrs->patch_mprotect = (void*)(kernelBase + 0x002FC0EC);
    addrs->patch_dmamini0 = (void*)(kernelBase + 0x005C96CB);
    addrs->patch_dmamini1 = (void*)(kernelBase + 0x005C96CF);
    addrs->patch_mdbg_basic = kernelBase + 0x0075CB50;

    /* mdbg Assist Mode offsets */
    uint64_t mdbg_offsets[] = { 0x54, 0x238, 0x1416, 0x2120, 0x2146, 0x216C, 0x2192, 0x21B8, 0x21DE, 0x2204, 0x222A, 0x2250, 0x2276, 0x229C, 0x22C2, 0x22E8, 0x230E, 0x2334, 0x236A, 0x2390, 0x23B6, 0x23DC };
    for (int i = 0; i < 22; i++) {
        addrs->mdbgAssistMode[i] = addrs->patch_mdbg_basic + mdbg_offsets[i];
    }
}
