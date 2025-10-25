#pragma once

#if SOFTWARE_VERSION == 1100

/* Util */
#define addr_Xfast_syscall		                0x000001C0
#define addr_sysvec                             0x1A6C4C8
#define addr_prison0                            0x111F830
#define addr_rootvnode                          0x2116640
#define addr_copyout							0x2DDEF0
#define addr_copyin								0x2DDFE0
#define addr_kern_open                          0xE6350
#define addr_kern_mkdir                         0xEB490
#define addr_kernel_map                         0x21FF130
#define addr_kmem_alloc                         0x245E10
#define addr_kmem_free                          0x245FE0
#define addr_vn_fullpath                        0x3C9B10
#define addr_DirectMemoryHook					0x404FC0
#define addr_fuse_loader						0x494970
#define addr_devact_onioctl_hook				0x638630
#define addr_dipsw_onioctl_hook					0x0
#define addr_dmamini_initialize_ioctl			0x5CF7C0
#define addr_trapHook                           0x0
#define addr_trap_fatalHook                     0x3C60A0
#define addr_QAFlags							0x0

/* STD Lib */
#define addr_M_TEMP                             0x1A52100
#define addr_M_MOUNT                            0x1A6ACC0
#define addr_malloc								0x1A4220
#define addr_free					    		0x1A43E0
#define addr_memcpy								0x2DDDF0
#define addr_memset								0x482D0
#define addr_memcmp								0x948B0
#define addr_strlen                             0x21DC40
#define addr_strcpy								0x1AA590
#define addr_strncpy                            0x2BBFD0
#define addr_strcmp                             0x443130
#define addr_strncmp                            0x313B10 
#define addr_strstr                             0x2C5740
#define addr_sprintf                            0x2FCE10
#define addr_snprintf                           0x2FCED0
#define addr_vsprintf                           0x2FCEA0
#define addr_vprintf                            0x2FCC40
#define addr_sscanf                             0x54A60
#define addr_strdup                             0x32DDD0
#define addr_realloc                            0x1A44E0
#define addr_kprintf                            0x2FCBD0

/* Event Handling */
#define addr_eventhandler_register              0x43E440
#define addr_eventhandler_deregister            0x43E7D0
#define addr_eventhandler_find_list             0x43E9C0

/* Proc */
#define addr_allproc						    0x22D0A98
#define addr_proc_kill                          0x44FC0
#define addr_proc_rwmem							0x3838A0
#define addr_create_thread                      0x295170

/* Virtual Memory */
#define addr_vm_map_lock						0x357760
#define addr_vm_map_unlock 						0x3577D0
#define addr_vm_map_findspace					0x35A970
#define addr_vm_map_delete						0x35A3B0
#define addr_vm_map_insert						0x358AB0
#define addr_vm_map_protect						0x35C710

/* Mutex Locks */
#define addr_mtx_lock_flags                     0x10E6A0
#define addr_mtx_unlock_flags                   0x10E950
#define addr_sx_xlock							0xE3200
#define addr_sx_xunlock							0xE33C0

/* Driver */
#define addr_make_dev_p                         0x2F7610
#define addr_destroy_dev                        0x2F7B30

/* FSelfs */
#define addr_sceSblAuthMgrGetSelfInfo						0x6437D0
#define addr_sceSblAuthMgrSmStart							0x63E9C0
#define addr_sceSblAuthMgrVerifyHeader						0x642FF0
#define addr_sbl_drv_msg_mtx								0x2646690
#define addr_gpu_va_page_list								0x2646688
#define addr_mini_syscore_self_binary						0x155CC48
#define addr_sceSblAuthMgrVerifyHeaderHook1					0x63DA26
#define addr_sceSblAuthMgrVerifyHeaderHook2					0x63E709
#define addr_SceSblAuthMgrIsLoadable2Hook					0x63D28E
#define addr_SceSblAuthMgrSmLoadSelfSegment_Mailbox			0x640E0D
#define addr_SceSblAuthMgrSmLoadSelfBlock_Mailbox			0x641A48
#define addr_sceSblAuthMgrIsLoadable__sceSblACMgrGetPathId	0x63D13C

/* Fake Pkgs */
#define addr_sbl_keymgr_buf_gva								0x2664808
#define addr_sbl_keymgr_buf_va								0x2664000
#define addr_sbl_keymgr_key_slots							0x26606E8
#define addr_sbl_keymgr_key_rbtree							0x26606F8
#define addr_sbl_pfs_sx										0x264C080
#define addr_fpu_ctx										0x2660180
#define addr_fpu_kern_enter									0xC0660
#define addr_fpu_kern_leave									0xC0720
#define addr_Sha256Hmac										0x2D1CA0
#define addr_sceSblDriverSendMsg							0x61AF60
#define addr_sceSblPfsSetKeys								0x61D900
#define addr_RsaesPkcs1v15Dec2048CRT						0x3C8060
#define addr_AesCbcCfb128Encrypt							0x2DEAA0
#define addr_AesCbcCfb128Decrypt							0x2DECD0
#define addr_sceSblKeymgrSetKeyForPfs						0x625DF0
#define addr_sceSblKeymgrClearKey							0x626130
#define addr_sceSblKeymgrSetKeyStorage						0x62EDC0

#define addr_SceSblDriverSendMsgHook						0x62EE65
#define addr_SceSblPfsSetKeysHook							0x6993B9
#define addr_NpdrmDecryptIsolatedRifHook					0x64D230
#define addr_NpdrmDecryptRifNewHook							0x64DFFE
#define addr_SceSblKeymgrInvalidateKeySxXlockHook			0x626FAD

/* Library Replacement */
#define addr_load_prx					0x85100

/* TTY Redirector */
#define addr_cloneuio					0x0
#define addr_console_write				0x0
#define addr_deci_tty_write				0x0
#define addr_M_IOV						0x0
#define addr_console_cdev				0x0
#define addr_DeciTTYWriteHook			0x0

/* ShellCore Patches */
#define addr_sceKernelIsGenuineCEX1		0x0016B664
#define addr_sceKernelIsGenuineCEX2		0x0086BD24
#define addr_sceKernelIsGenuineCEX3		0x008BC022
#define addr_sceKernelIsGenuineCEX4		0x00A1D6C4

#define addr_sceKernelIsAssistMode1		0x0016B692
#define addr_sceKernelIsAssistMode2		0x00249E0C
#define addr_sceKernelIsAssistMode3		0x0086BD52
#define addr_sceKernelIsAssistMode4		0x00A1D6F2

#define addr_enableFpkg					0x003D7C9F
#define addr_fakeText					0xFC8439
#define addr_mountDataIntoSandbox		0x31F070

#define addr_disablePkgPatchCheck1		0x00134A50
#define addr_disablePkgPatchCheck2		0x003C41A7
#define addr_disablePkgPatchCheck3		0x003C73F0

/* ShellUI Patches */
#define sceKernelGetDebugMenuModeForRcmgr		0x1D100
#define sceKernelGetUtokenStoreModeForRcmgr		0x1D460

/* Kernel Patches */
#define patch_ASLR				0x003B11A4
#define patch_copyin1			0x002DE037
#define patch_copyin2			0x002DE043
#define patch_copyout1			0x002DDF42
#define patch_copyout2			0x002DDF4E
#define patch_copyinstr1		0x002DE4E3
#define patch_copyinstr2		0x002DE4EF
#define patch_copyinstr3		0x002DE520
#define patch_swword_lwpid1		0x002DE302
#define patch_swword_lwpid2		0x002DE311
#define patch_ptrace1			0x00384285
#define patch_ptrace2			0x00384771
#define patch_dynlibPath1		0x001E597F
#define patch_dynlibPath2		0x001E598A
#define patch_disablepfsSig		0x00684EB0
#define patch_debugRif1			0x0064BFD0
#define patch_debugRif2			0x0064C000
#define patch_debugSettings1	0x004EE328
#define patch_debugSettings2	0x004EF3EE
#define patch_mount				0x00388B37
#define patch_setuid			0x0043151F
#define patch_sysmap			0x0015626A
#define patch_dynlib_dlsym1		0x001E4CA8
#define patch_dynlib_dlsym2		0x00088CE0
#define patch_display_dump		0x000442B5
#define patch_debuglogs			0x002FCCB7
#define patch_fuseLoader		0x0049499E
#define patch_fuseroot1			0x0040CFA6
#define patch_fuseroot2			0x0030687E
#define patch_mprotect			0x0035C8EC
#define patch_AllowSLDebugger	0x003CA4A0
#define patch_AllowULDebugger	0x003CA7A0
#define patch_IsSoftwagner		0x003CA880
#define patch_dmamini0			0x005CF7DB
#define patch_dmamini1			0x005CF7DF
#define patch_isDebuggerProcess 0x003D05E0
#define patch_isAssistMode	    0x0
#define offsets_mdbgAssistMode  { 0x1E4, 0x13C2, 0x20CC, 0x20F2, 0x2118, 0x213E, 0x2164, 0x218A, 0x21B0, 0x21D6, 0x21FC, 0x2222, 0x2248, 0x226E, 0x2294, 0x22BA, 0x22E0, 0x2316, 0x233C, 0x2362, 0x2388 };

#endif