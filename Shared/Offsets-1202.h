#pragma once

#if SOFTWARE_VERSION == 1202

/* Util */
#define addr_Xfast_syscall		                0x000001C0
#define addr_sysvec                             0x01A7CFE8
#define addr_prison0                            0x0111FA18 
#define addr_rootvnode                          0x02136E90
#define addr_copyout							0x002BD580
#define addr_copyin								0x002BD670
#define addr_copyinstr							0x002BDB20
#define addr_kern_open                          0x00343570
#define addr_kern_mkdir                         0x003486B0
#define addr_kernel_map                         0x022D1D50
#define addr_kmem_alloc                         0x004659E0
#define addr_kmem_free                          0x00465BB0
#define addr_vn_fullpath                        0x00308C70
#define addr_fuse_loader						0x00495360
#define addr_DirectMemoryHook					0x00283CE0
#define addr_devact_onioctl_hook				0x006388F0
#define addr_dipsw_onioctl_hook					0x00655940
#define addr_sceKernelCheckDipsw_Hook			0x00655030
#define addr_dmamini_initialize_ioctl			0x005C96B0
#define addr_trapHook                           0x0
#define addr_trap_fatalHook                     0x0
#define addr_QAFlags							0x021CC5D0

/* STD Lib */
#define addr_M_TEMP                             0x01520D00
#define addr_M_MOUNT                            0x01A40250
#define addr_malloc								0x00009520
#define addr_free					    		0x000096E0
#define addr_memcpy								0x002BD480
#define addr_memset								0x001FA140
#define addr_memcmp								0x003942A0
#define addr_strlen                             0x0036AB30
#define addr_strcpy								0x00417680
#define addr_strncpy                            0x003A8250
#define addr_strcmp                             0x000B2940
#define addr_strncmp                            0x003C6310
#define addr_strstr                             0x0021CC50
#define addr_sprintf                            0x002E0620
#define addr_snprintf                           0x002E06E0
#define addr_vsprintf                           0x002E06B0
#define addr_vprintf                            0x002E0450
#define addr_sscanf                             0x0043E110
#define addr_strdup                             0x004077F0
#define addr_realloc                            0x000097E0
#define addr_kprintf                            0x002E03E0

/* Event Handling */
#define addr_eventhandler_register              0x00224110
#define addr_eventhandler_deregister            0x002244A0
#define addr_eventhandler_find_list             0x00224690

/* Proc */
#define addr_allproc						    0x01B28538
#define addr_pfind								0x0000EA40
#define addr_proc_rwmem							0x00365FA0
#define addr_create_thread                      0x0004C6C0
#define addr_do_dlsym							0x003BAF00
#define addr_find_obj_by_handle					0x003BC080

/* Virtual Memory */
#define addr_vm_map_lock						0x002F6F60
#define addr_vm_map_unlock 						0x002F6FD0
#define addr_vm_map_findspace					0x002FA170
#define addr_vm_map_delete						0x002F9BB0
#define addr_vm_map_insert						0x002F82B0 
#define addr_vm_map_protect						0x002FBF10

/* Mutex Locks */
#define addr_mtx_lock_flags                     0x003782C0
#define addr_mtx_unlock_flags                   0x00378570
#define addr_sx_xlock							0x000A3840
#define addr_sx_xunlock							0x000A3A00

/* Driver */
#define addr_make_dev_p                         0x0038A910
#define addr_destroy_dev                        0x0038AE30

/* FSelfs */
#define addr_sceSblAuthMgrGetSelfInfo						0x0063CFB0
#define addr_sceSblAuthMgrSmStart							0x0063DB40
#define addr_sceSblAuthMgrVerifyHeader						0x0063C7D0
#define addr_sbl_drv_msg_mtx								0x02647358
#define addr_gpu_va_page_list								0x02647350
#define addr_mini_syscore_self_binary						0x0153D6C8
#define addr_sceSblAuthMgrVerifyHeaderHook1					0x00642876
#define addr_sceSblAuthMgrVerifyHeaderHook2					0x00643559
#define addr_SceSblAuthMgrIsLoadable2Hook					0x006420DE
#define addr_SceSblAuthMgrSmLoadSelfSegment_Mailbox			0x0063FF8D
#define addr_SceSblAuthMgrSmLoadSelfBlock_Mailbox			0x00640BC8
#define addr_sceSblAuthMgrIsLoadable__sceSblACMgrGetPathId	0x00641F8C

/* Fake Pkgs */
#define addr_sbl_keymgr_buf_gva								0x0266C808
#define addr_sbl_keymgr_buf_va								0x0266C000
#define addr_sbl_keymgr_key_slots							0x02668040
#define addr_sbl_keymgr_key_rbtree							0x02668050
#define addr_sbl_pfs_sx										0x0265C080
#define addr_fpu_ctx										0x026542C0
#define addr_fpu_kern_enter									0x001DFFE0
#define addr_fpu_kern_leave									0x001E00A0
#define addr_Sha256Hmac										0x001F8D40
#define addr_sceSblDriverSendMsg							0x0061BFA0
#define addr_sceSblPfsSetKeys								0x006266E0
#define addr_RsaesPkcs1v15Dec2048CRT						0x0021BC00
#define addr_AesCbcCfb128Encrypt							0x00340E30
#define addr_AesCbcCfb128Decrypt							0x00341060
#define addr_sceSblKeymgrSetKeyForPfs						0x0062B030
#define addr_sceSblKeymgrClearKey							0x0062B370
#define addr_sceSblKeymgrSetKeyStorage						0x00624740

#define addr_SceSblDriverSendMsgHook						0x006247E5
#define addr_SceSblPfsSetKeysHook							0x006A2D99
#define addr_NpdrmDecryptIsolatedRifHook					0x0064C4E0
#define addr_NpdrmDecryptRifNewHook							0x0064D2AE
#define addr_SceSblKeymgrInvalidateKeySxXlockHook			0x0062C1ED

/* Library Replacement */
#define addr_load_prx					0x003B9C80

/* TTY Redirector */
#define addr_cloneuio					0x0036CC80
#define addr_console_write				0x0046F990
#define addr_deci_tty_write				0x0048C4E0
#define addr_M_IOV						0x01A4A230
#define addr_console_cdev				0x022D1F30
#define addr_DeciTTYWriteHook			0x01A7EDD8

/* ShellCore Patches */
#define addr_sceKernelIsGenuineCEX1		0x0016F5A4
#define addr_sceKernelIsGenuineCEX2		0x00873754
#define addr_sceKernelIsGenuineCEX3		0x008C3A52
#define addr_sceKernelIsGenuineCEX4		0x00A27304

#define addr_sceKernelIsAssistMode1		0x0016F5D2
#define addr_sceKernelIsAssistMode2		0x0024E14C
#define addr_sceKernelIsAssistMode3		0x00873782
#define addr_sceKernelIsAssistMode4		0x00A27332

#define addr_enableFpkg					0x003DE23F
#define addr_fakeText					0x00FCFDF9
#define addr_mountDataIntoSandbox		0x003233B0

#define addr_disablePkgPatchCheck1		0x001389A0
#define addr_disablePkgPatchCheck2		0x003CA567
#define addr_disablePkgPatchCheck3		0x003CD7B0

/* ShellUI Patches */
#define sceKernelGetDebugMenuModeForRcmgr		0x0001D100
#define sceKernelGetUtokenStoreModeForRcmgr		0x0001D460

/* Kernel Patches */
#define patch_memcpy			0x002BD48D
#define patch_kmem_alloc1		0x00465AAC
#define patch_kmem_alloc2		0x00465AB4
#define patch_ASLR				0x00477C54
#define patch_copyin1			0x002BD6C7
#define patch_copyin2			0x002BD6D3
#define patch_copyout1			0x002BD5D2
#define patch_copyout2			0x002BD5DE
#define patch_copyinstr1		0x002BDB73
#define patch_copyinstr2		0x002BDB7F
#define patch_copyinstr3		0x002BDBB0
#define patch_swword_lwpid1		0x002BD992
#define patch_swword_lwpid2		0x002BD9A1
#define patch_ptrace1			0x0036699d
#define patch_ptrace2			0x00366e71
#define patch_dynlibPath1		0x001B83EF
#define patch_dynlibPath2		0x001B83FA
#define patch_disablepfsSig		0x0069D9A0
#define patch_debugRif1			0x0064EB30
#define patch_debugRif2			0x0064EB60
#define patch_debugSettings1	0x004E8748
#define patch_debugSettings2	0x004E980E
#define patch_mount				0x00151267
#define patch_setuid			0x003914DF
#define patch_sysmap			0x001FA71A
#define patch_dynlib_dlsym1		0x001B7718
#define patch_dynlib_dlsym2		0x003BD860
#define patch_display_dump		0x001BF1C5
#define patch_debuglogs			0x002E04C7
#define patch_fuseLoader		0x0049538E
#define patch_fuseroot1			0x0010D066
#define patch_fuseroot2			0x0010D07E
#define patch_mprotect			0x002FC0EC
#define patch_AllowSLDebugger	0x0041F630
#define patch_AllowULDebugger	0x0041F930
#define patch_IsSoftwagner		0x0041FA10
#define patch_dmamini0			0x005C96CB
#define patch_dmamini1			0x005C96CF
#define patch_isDebuggerProcess 0x003B24D0
#define patch_isAssistMode	    0x0075CBA4
#define offsets_mdbgAssistMode  { 0x1E4, 0x13C2, 0x20CC, 0x20F2, 0x2118, 0x213E, 0x2164, 0x218A, 0x21B0, 0x21D6, 0x21FC, 0x2222, 0x2248, 0x226E, 0x2294, 0x22BA, 0x22E0, 0x2316, 0x233C, 0x2362, 0x2388 };

#endif