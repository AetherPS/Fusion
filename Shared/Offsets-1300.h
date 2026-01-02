#pragma once

#if SOFTWARE_VERSION == 1300

/* Util */
#define addr_Xfast_syscall		                0x0
#define addr_sysvec                             0x0
#define addr_prison0                            0x0 
#define addr_rootvnode                          0x0
#define addr_copyout							0x0
#define addr_copyin								0x0
#define addr_copyout_nofault					0x0
#define addr_copyin_nofault						0x0
#define addr_copyinstr							0x0
#define addr_kern_open                          0x0
#define addr_kern_mkdir                         0x0
#define addr_kernel_map                         0x0
#define addr_kmem_alloc                         0x0
#define addr_kmem_free                          0x0
#define addr_vn_fullpath                        0x0
#define addr_fuse_loader						0x0
#define addr_DirectMemoryHook					0x0
#define addr_devact_onioctl_hook				0x0
#define addr_dipsw_onioctl_hook					0x0
#define addr_sceKernelCheckDipsw_Hook			0x0
#define addr_dmamini_initialize_ioctl			0x0
#define addr_trapHook                           0x0
#define addr_trap_fatalHook                     0x0
#define addr_QAFlags							0x0
#define addr_getnewvnode						0x0

/* STD Lib */
#define addr_M_TEMP                             0x0
#define addr_M_MOUNT                            0x0
#define addr_malloc								0x0
#define addr_free					    		0x0
#define addr_memcpy								0x0
#define addr_memset								0x0
#define addr_memcmp								0x0
#define addr_strlen                             0x0
#define addr_strcpy								0x0
#define addr_strncpy                            0x0
#define addr_strcmp                             0x0
#define addr_strncmp                            0x0
#define addr_strstr                             0x0
#define addr_sprintf                            0x0
#define addr_snprintf                           0x0
#define addr_vsprintf                           0x0
#define addr_vprintf                            0x0
#define addr_sscanf                             0x0
#define addr_strdup                             0x0
#define addr_realloc                            0x0
#define addr_printf                             0x0
#define addr_hexdump							0x0
#define addr_dynlib_is_host_path				0x0
#define addr_dynlib_basename					0x0
#define addr_dynlib_basename_host				0x0

/* Event Handling */
#define addr_eventhandler_register              0x0
#define addr_eventhandler_deregister            0x0
#define addr_eventhandler_find_list             0x0

/* Proc */
#define addr_allproc						    0x0
#define addr_allproc_lock						0x0	
#define addr_pfind								0x0
#define addr_proc_rwmem							0x0
#define addr_create_thread                      0x0
#define addr_do_dlsym							0x0
#define addr_find_obj_by_handle					0x0

/* Virtual Memory */
#define addr_vm_map_lock						0x0
#define addr_vm_map_unlock 						0x0
#define addr_vm_map_findspace					0x0
#define addr_vm_map_delete						0x0
#define addr_vm_map_insert						0x0 
#define addr_vm_map_protect						0x0

/* Mutex Locks */
#define addr_mtx_lock_flags                     0x0
#define addr_mtx_unlock_flags                   0x0
#define addr_sx_xlock							0x0
#define addr_sx_xunlock							0x0
#define addr_sx_slock							0x0
#define addr_sx_sunlock							0x0

/* Driver */
#define addr_make_dev_p                         0x0
#define addr_destroy_dev                        0x0
#define addr_devfs_rule_applyde_recursive       0x0

/* Flash & NVS */
#define addr_icc_nvs_read						0x0
#define addr_icc_nvs_write						0x0

/* FSelfs */
#define addr_sceSblAuthMgrGetSelfInfo						0x0
#define addr_sceSblAuthMgrSmStart							0x0
#define addr_sceSblAuthMgrVerifyHeader						0x0
#define addr_sbl_drv_msg_mtx								0x0
#define addr_gpu_va_page_list								0x0
#define addr_mini_syscore_self_binary						0x0
#define addr_sceSblAuthMgrVerifyHeaderHook1					0x0
#define addr_sceSblAuthMgrVerifyHeaderHook2					0x0
#define addr_SceSblAuthMgrIsLoadable2Hook					0x0
#define addr_SceSblAuthMgrSmLoadSelfSegment_Mailbox			0x0
#define addr_SceSblAuthMgrSmLoadSelfBlock_Mailbox			0x0
#define addr_sceSblAuthMgrIsLoadable__sceSblACMgrGetPathId	0x0

/* Fake Pkgs */
#define addr_sbl_keymgr_buf_gva								0x0
#define addr_sbl_keymgr_buf_va								0x0
#define addr_sbl_keymgr_key_slots							0x0
#define addr_sbl_keymgr_key_rbtree							0x0
#define addr_sbl_pfs_sx										0x0
#define addr_fpu_ctx										0x0
#define addr_fpu_kern_enter									0x0
#define addr_fpu_kern_leave									0x0
#define addr_Sha256Hmac										0x0
#define addr_sceSblDriverSendMsg							0x0
#define addr_sceSblPfsSetKeys								0x0
#define addr_RsaesPkcs1v15Dec2048CRT						0x0
#define addr_AesCbcCfb128Encrypt							0x0
#define addr_AesCbcCfb128Decrypt							0x0
#define addr_sceSblKeymgrSetKeyForPfs						0x0
#define addr_sceSblKeymgrClearKey							0x0
#define addr_sceSblKeymgrSetKeyStorage						0x0

#define addr_SceSblDriverSendMsgHook						0x0
#define addr_SceSblPfsSetKeysHook							0x0
#define addr_NpdrmDecryptIsolatedRifHook					0x0
#define addr_NpdrmDecryptRifNewHook							0x0
#define addr_SceSblKeymgrInvalidateKeySxXlockHook			0x0

/* Library Replacement */
#define addr_load_prx					0x0

/* TTY Redirector */
#define addr_cloneuio					0x0
#define addr_console_write				0x0
#define addr_deci_tty_write				0x0
#define addr_M_IOV						0x0
#define addr_console_cdev				0x0
#define addr_DeciTTYWriteHook			0x0

/* Kernel Patches */
#define patch_memcpy			0x0
#define patch_kmem_alloc1		0x0
#define patch_kmem_alloc2		0x0
#define patch_ASLR				0x0
#define patch_copyin1			0x0
#define patch_copyin2			0x0
#define patch_copyout1			0x0
#define patch_copyout2			0x0
#define patch_copyinstr1		0x0
#define patch_copyinstr2		0x0
#define patch_copyinstr3		0x0
#define patch_swword_lwpid1		0x0
#define patch_swword_lwpid2		0x0
#define patch_ptrace1			0x0
#define patch_ptrace2			0x0
#define patch_dynlibPath1		0x0
#define patch_dynlibPath2		0x0
#define patch_disablepfsSig		0x0
#define patch_debugRif1			0x0
#define patch_debugRif2			0x0
#define patch_debugSettings1	0x0
#define patch_debugSettings2	0x0
#define patch_mount				0x0
#define patch_setuid			0x0
#define patch_sysmap			0x0
#define patch_dynlib_dlsym1		0x0
#define patch_dynlib_dlsym2		0x0
#define patch_display_dump		0x0
#define patch_debuglogs			0x0
#define patch_fuseLoader		0x0
#define patch_fuseroot1			0x0
#define patch_fuseroot2			0x0
#define patch_mprotect			0x0
#define patch_AllowSLDebugger	0x0
#define patch_AllowULDebugger	0x0
#define patch_IsSoftwagner		0x0
#define patch_dmamini0			0x0
#define patch_dmamini1			0x0
#define patch_isDebuggerProcess 0x0
#define patch_isAssistMode	    0x0
#define offsets_mdbgAssistMode  { 0x1E4, 0x13C2, 0x20CC, 0x20F2, 0x2118, 0x213E, 0x2164, 0x218A, 0x21B0, 0x21D6, 0x21FC, 0x2222, 0x2248, 0x226E, 0x2294, 0x22BA, 0x22E0, 0x2316, 0x233C, 0x2362, 0x2388 };

#endif