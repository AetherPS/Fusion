#pragma once

#if SOFTWARE_VERSION == 1250

/* Util */
#define addr_Xfast_syscall		                0x000001C0
#define addr_sysvec                             0x01A7CFE8
#define addr_prison0                            0x0111FA18
#define addr_rootvnode                          0x02136E90
#define addr_copyout							0x002BD5C0
#define addr_copyin								0x002BD6B0
#define addr_copyout_nofault					0x0036C6A0
#define addr_copyin_nofault						0x0036C660
#define addr_copyinstr							0x002BDB60
#define addr_kern_open                          0x003435B0
#define addr_kern_mkdir                         0x003486F0
#define addr_kernel_map                         0x022D1D50
#define addr_kmem_alloc                         0x00465A20
#define addr_kmem_free                          0x00465BF0
#define addr_vn_fullpath                        0x00308CB0
#define addr_fuse_loader						0x004953A0
#define addr_DirectMemoryHook					0x00283D20
#define addr_devact_onioctl_hook				0x00638930
#define addr_dipsw_onioctl_hook					0x006559E0
#define addr_sceKernelCheckDipsw_Hook			0x006550D0
#define addr_dmamini_initialize_ioctl			0x005C96F0
#define addr_trapHook                           0x0
#define addr_trap_fatalHook                     0x0
#define addr_QAFlags							0x021CC5D0
#define addr_getnewvnode						0x0036E2C0

/* STD Lib */
#define addr_M_TEMP                             0x01520D00
#define addr_M_MOUNT                            0x01A40250
#define addr_malloc								0x00009520
#define addr_free					    		0x000096E0
#define addr_memcpy								0x002BD4C0
#define addr_memset								0x001FA180
#define addr_memcmp								0x003942E0
#define addr_strlen                             0x0036AB70
#define addr_strcpy								0x004176C0
#define addr_strncpy                            0x003A8290
#define addr_strcmp                             0x000B2940
#define addr_strncmp                            0x003C6350
#define addr_strstr                             0x0021CC90
#define addr_sprintf                            0x002E0660
#define addr_snprintf                           0x002E0720
#define addr_vsprintf                           0x002E06F0
#define addr_vprintf                            0x002E0490
#define addr_sscanf                             0x0043E150
#define addr_strdup                             0x00407830
#define addr_realloc                            0x000097E0
#define addr_printf                             0x002E0420
#define addr_hexdump							0x002E1D50
#define addr_dynlib_is_host_path				0x001B8690
#define addr_dynlib_basename					0x001B8720
#define addr_dynlib_basename_host				0x001B86F0

/* Event Handling */
#define addr_eventhandler_register              0x00224150
#define addr_eventhandler_deregister            0x002244E0
#define addr_eventhandler_find_list             0x002246D0

/* Proc */
#define addr_allproc						    0x01B28538
#define addr_allproc_lock						0x01B284D8
#define addr_pfind								0x0000EA40
#define addr_proc_rwmem							0x00365FE0
#define addr_create_thread                      0x0004C6C0
#define addr_do_dlsym							0x003BAF40
#define addr_find_obj_by_handle					0x003BC0C0

/* Virtual Memory */
#define addr_vm_map_lock						0x002F6FA0
#define addr_vm_map_unlock 						0x002F7010
#define addr_vm_map_findspace					0x002FA1B0
#define addr_vm_map_delete						0x002F9BF0
#define addr_vm_map_insert						0x002F82F0
#define addr_vm_map_protect						0x002FBF50

/* Mutex Locks */
#define addr_mtx_lock_flags                     0x00378300
#define addr_mtx_unlock_flags                   0x003785B0
#define addr_sx_xlock							0x000A3840
#define addr_sx_xunlock							0x000A3A00
#define addr_sx_slock							0x000A3660
#define addr_sx_sunlock							0x000A3950

/* Driver */
#define addr_make_dev_p                         0x0038A950
#define addr_destroy_dev                        0x0038AE70
#define addr_devfs_rule_applyde_recursive       0x002DEB40

/* Flash & NVS */
#define addr_icc_nvs_read						0x000A5BD0
#define addr_icc_nvs_write						0x000A5A10

/* Sysctl */
#define addr_sysctl__children					0x022CC600
#define addr_sysctl_ctx_init					0x003F9590
#define addr_sysctl_ctx_free					0x003F95B0
#define addr_sysctl_add_oid						0x003F9BF0
#define addr_sysctl_handle_int					0x003FA070
#define addr_sysctl_handle_string				0x003FA310

/* FSelfs */
#define addr_sceSblAuthMgrGetSelfInfo						0x0063D050
#define addr_sceSblAuthMgrSmStart							0x0063DBE0
#define addr_sceSblAuthMgrVerifyHeader						0x0063C870
#define addr_sbl_drv_msg_mtx								0x02647358
#define addr_gpu_va_page_list								0x02647350
#define addr_mini_syscore_self_binary						0x0153D6C8  
#define addr_sceSblAuthMgrVerifyHeaderHook1					0x00642916
#define addr_sceSblAuthMgrVerifyHeaderHook2					0x006435F9
#define addr_SceSblAuthMgrIsLoadable2Hook					0x0064217E
#define addr_SceSblAuthMgrSmLoadSelfSegment_Mailbox			0x0064002D
#define addr_SceSblAuthMgrSmLoadSelfBlock_Mailbox			0x00640C68
#define addr_sceSblAuthMgrIsLoadable__sceSblACMgrGetPathId	0x0064202C

/* Fake Pkgs */
#define addr_sbl_keymgr_buf_gva								0x0266C808
#define addr_sbl_keymgr_buf_va								0x0266C000
#define addr_sbl_keymgr_key_slots							0x02668040
#define addr_sbl_keymgr_key_rbtree							0x02668050
#define addr_sbl_pfs_sx										0x0265C080
#define addr_fpu_ctx										0x026542C0
#define addr_fpu_kern_enter									0x001E0020
#define addr_fpu_kern_leave									0x001E00E0
#define addr_Sha256Hmac										0x001F8D80
#define addr_sceSblDriverSendMsg							0x0061BFE0
#define addr_sceSblPfsSetKeys								0x00626720
#define addr_RsaesPkcs1v15Dec2048CRT						0x0021BC40
#define addr_AesCbcCfb128Encrypt							0x00340E70
#define addr_AesCbcCfb128Decrypt							0x003410A0
#define addr_sceSblKeymgrSetKeyForPfs						0x0062B070
#define addr_sceSblKeymgrClearKey							0x0062B3B0
#define addr_sceSblKeymgrSetKeyStorage						0x00624780

#define addr_SceSblDriverSendMsgHook						0x00624825
#define addr_SceSblPfsSetKeysHook							0x006A2E39
#define addr_NpdrmDecryptIsolatedRifHook					0x0064C580
#define addr_NpdrmDecryptRifNewHook							0x0064D34E
#define addr_SceSblKeymgrInvalidateKeySxXlockHook			0x0062C22D

/* Library Replacement */
#define addr_load_prx					0x003B9CC0

/* TTY Redirector */
#define addr_cloneuio					0x0036CCC0
#define addr_console_write				0x0046F9D0
#define addr_deci_tty_write				0x0048C520
#define addr_M_IOV						0x01A4A230
#define addr_console_cdev				0x022D1F30
#define addr_DeciTTYWriteHook			0x01A7EDD8

/* Kernel Patches */
#define patch_memcpy			0x002BD4CD
#define patch_kmem_alloc1		0x00465AEC
#define patch_kmem_alloc2		0x00465AF4
#define patch_ASLR				0x00477C94
#define patch_copyin1			0x002BD707
#define patch_copyin2			0x002BD713
#define patch_copyout1			0x002BD612
#define patch_copyout2			0x002BD61E
#define patch_copyinstr1		0x002BDBB3
#define patch_copyinstr2		0x002BDBBF
#define patch_copyinstr3		0x002BDBF0
#define patch_swword_lwpid1		0x002BDA42
#define patch_swword_lwpid2		0x002BDA55
#define patch_ptrace1			0x003669DD
#define patch_ptrace2			0x00366EB1
#define patch_dynlibPath1		0x001B842F
#define patch_dynlibPath2		0x001B8437
#define patch_disablepfsSig		0x0069DA40
#define patch_debugRif1			0x0064EBD0
#define patch_debugRif2			0x0064EC00
#define patch_debugSettings1	0x004E8788
#define patch_debugSettings2	0x004E984E
#define patch_mount				0x001512A7
#define patch_setuid			0x0039151F
#define patch_sysmap			0x001FA75A
#define patch_dynlib_dlsym1		0x001B7758
#define patch_dynlib_dlsym2		0x003BD8A0
#define patch_display_dump		0x001BF205
#define patch_debuglogs			0x002E0507
#define patch_fuseLoader		0x004953CE
#define patch_fuseroot1			0x0010D066
#define patch_fuseroot2			0x0010D07E
#define patch_mprotect			0x002FC12C
#define patch_dmamini0			0x005C970B
#define patch_dmamini1			0x005C970F
#define patch_mdbg_basic	    0x0075CC10
#define offsets_mdbgAssistMode  { 0x54, 0x238, 0x1416, 0x2120, 0x2146, 0x216C, 0x2192, 0x21B8, 0x21DE, 0x2204, 0x222A, 0x2250, 0x2276, 0x229C, 0x22C2, 0x22E8, 0x230E, 0x2334, 0x236A, 0x2390, 0x23B6, 0x23DC };

#endif