#pragma once

#if SOFTWARE_VERSION == 900

/* Util */
#define addr_Xfast_syscall		                0x000001C0
#define addr_sysvec                             0x01528E30
#define addr_prison0                            0x0111F870 
#define addr_rootvnode                          0x021EFF20
#define addr_copyout							0x002715B0
#define addr_copyin								0x002716A0
#define addr_kern_open                          0x001D9EE0
#define addr_kern_mkdir                         0x001DF020
#define addr_kernel_map                         0x02268D48
#define addr_kmem_alloc                         0x0037BE70
#define addr_kmem_free                          0x0037C040
#define addr_vn_fullpath                        0x002648C0
#define addr_fuse_loader						0x00490720
#define addr_DirectMemoryHook					0x0011D250
#define addr_devact_onioctl_hook				0x006397A0
#define addr_dipsw_onioctl_hook					0x00656490
#define addr_dmamini_initialize_ioctl			0x005D1EC0
#define addr_trapHook                           0x0
#define addr_trap_fatalHook                     0x002DF710
#define addr_QAFlags							0x02216850

/* STD Lib */
#define addr_M_TEMP                             0x015621E0
#define addr_M_MOUNT                            0x015279F0
#define addr_malloc								0x00301B20
#define addr_free					    		0x00301CE0
#define addr_memcpy								0x002714B0
#define addr_memset								0x001496C0
#define addr_memcmp								0x00271E20
#define addr_strlen                             0x0030F450
#define addr_strcpy								0x00189F80
#define addr_strncpy                            0x0041E380
#define addr_strcmp                             0x0040E700
#define addr_strncmp                            0x00124750 
#define addr_strstr                             0x00487AB0
#define addr_sprintf                            0x000B7C70
#define addr_snprintf                           0x000B7D30
#define addr_vsprintf                           0x000B7D00
#define addr_vprintf                            0x000B7AA0
#define addr_sscanf                             0x0026C8D0
#define addr_strdup                             0x00278540
#define addr_realloc                            0x00301DE0
#define addr_kprintf                            0x000B7A30

/* Event Handling */
#define addr_eventhandler_register              0x000F8370
#define addr_eventhandler_deregister            0x000F8700
#define addr_eventhandler_find_list             0x000F88F0

/* Proc */
#define addr_allproc						    0x01B946E0
#define addr_proc_rwmem							0x0041EB00
#define addr_create_thread                      0x001ED670

/* Virtual Memory */
#define addr_vm_map_lock						0x0007BA30
#define addr_vm_map_unlock 						0x0007BAA0
#define addr_vm_map_findspace					0x0007EC40
#define addr_vm_map_delete						0x0007E680
#define addr_vm_map_insert						0x0007CD80 
#define addr_vm_map_protect						0x000809C0

/* Mutex Locks */
#define addr_mtx_lock_flags                     0x002EEEB0
#define addr_mtx_unlock_flags                   0x002EF170
#define addr_sx_xlock							0x0043E610
#define addr_sx_xunlock							0x0043E7D0

/* Driver */
#define addr_make_dev_p                         0x001EF590
#define addr_destroy_dev                        0x001EFAB0

/* FSelfs */
#define addr_sceSblAuthMgrGetSelfInfo						0x006441E0
#define addr_sceSblAuthMgrSmStart							0x0063FEE0
#define addr_sceSblAuthMgrVerifyHeader						0x00643A00
#define addr_sbl_drv_msg_mtx								0x02646CB0
#define addr_gpu_va_page_list								0x02646CA8
#define addr_mini_syscore_self_binary						0x01579DF8
#define addr_sceSblAuthMgrVerifyHeaderHook1					0x00645026
#define addr_sceSblAuthMgrVerifyHeaderHook2					0x00645D09
#define addr_SceSblAuthMgrIsLoadable2Hook					0x0064488E
#define addr_SceSblAuthMgrSmLoadSelfSegment_Mailbox			0x0064232D
#define addr_SceSblAuthMgrSmLoadSelfBlock_Mailbox			0x00642F68
#define addr_sceSblAuthMgrIsLoadable__sceSblACMgrGetPathId	0x0064473C

/* Fake Pkgs */
#define addr_sbl_keymgr_buf_gva								0x0264C808
#define addr_sbl_keymgr_buf_va								0x0264C000
#define addr_sbl_keymgr_key_slots							0x02648238
#define addr_sbl_keymgr_key_rbtree							0x02648248
#define addr_sbl_pfs_sx										0x0264DB40
#define addr_fpu_ctx										0x026541C0
#define addr_fpu_kern_enter									0x002196D0
#define addr_fpu_kern_leave									0x00219790
#define addr_Sha256Hmac										0x00445060
#define addr_sceSblDriverSendMsg							0x0061CED0
#define addr_sceSblPfsSetKeys								0x006252D0
#define addr_RsaesPkcs1v15Dec2048CRT						0x004628B0
#define addr_AesCbcCfb128Encrypt							0x001FF2D0
#define addr_AesCbcCfb128Decrypt							0x001FF500
#define addr_sceSblKeymgrSetKeyForPfs						0x0061F690
#define addr_sceSblKeymgrClearKey							0x0061F9D0
#define addr_sceSblKeymgrSetKeyStorage						0x00624970

#define addr_SceSblDriverSendMsgHook						0x00624A15
#define addr_SceSblPfsSetKeysHook							0x006C3EF9
#define addr_NpdrmDecryptIsolatedRifHook					0x0064E070
#define addr_NpdrmDecryptRifNewHook							0x0064EE3E
#define addr_SceSblKeymgrInvalidateKeySxXlockHook			0x0062084D

/* Library Replacement */
#define addr_load_prx					0x0021DF60

/* TTY Redirector */
#define addr_cloneuio					0x0044E7E0
#define addr_console_write				0x002D6EB0
#define addr_deci_tty_write				0x0048CDE0
#define addr_M_IOV						0x01A792C0
#define addr_console_cdev				0x021F1128
#define addr_DeciTTYWriteHook			0x01A7ED98

/* ShellCore Patches */
#define addr_sceKernelIsGenuineCEX1		0x0016EAA4
#define addr_sceKernelIsGenuineCEX2		0x008621D4
#define addr_sceKernelIsGenuineCEX3		0x008AFBC2
#define addr_sceKernelIsGenuineCEX4		0x00A27BD4

#define addr_sceKernelIsAssistMode1		0x0016EAD2
#define addr_sceKernelIsAssistMode2		0x00249F7B
#define addr_sceKernelIsAssistMode3		0x00862202
#define addr_sceKernelIsAssistMode4		0x00A27C02

#define addr_enableFpkg					0x003D7AFF
#define addr_fakeText					0x00FD3211
#define addr_mountDataIntoSandbox		0x0032079B

#define addr_disablePkgPatchCheck1		0x00138E90
#define addr_disablePkgPatchCheck2		0x003C5EA7
#define addr_disablePkgPatchCheck3		0x003C8540

/* ShellUI Patches */
#define sceKernelGetDebugMenuModeForRcmgr		0x0001D1C0
#define sceKernelGetUtokenStoreModeForRcmgr		0x0001D520

/* Kernel Patches */
#define patch_ASLR				0x0005F824
#define patch_copyin1			0x002716F7
#define patch_copyin2			0x00271703
#define patch_copyout1			0x00271602
#define patch_copyout2			0x0027160E
#define patch_copyinstr1		0x00271BA3
#define patch_copyinstr2		0x00271BAF
#define patch_copyinstr3		0x00271BE0
#define patch_swword_lwpid1		0x002719C2
#define patch_swword_lwpid2		0x002719D1
#define patch_ptrace1			0x0041F4E5
#define patch_ptrace2			0x0041F9D1
#define patch_dynlibPath1		0x0023C19F
#define patch_dynlibPath2		0x0023C1AA
#define patch_disablepfsSig		0x006885C0
#define patch_debugRif1			0x00650430
#define patch_debugRif2			0x00650460
#define patch_debugSettings1	0x004E8E48
#define patch_debugSettings2	0x004EA12F
#define patch_mount				0x0004ADE7
#define patch_setuid			0x000019FF
#define patch_sysmap			0x0016632A
#define patch_dynlib_dlsym1		0x0023B67F
#define patch_dynlib_dlsym2		0x00221B40
#define patch_display_dump		0x00028A75
#define patch_debuglogs			0x000B7B17
#define patch_fuseLoader		0x0049074E
#define patch_fuseroot1			0x00306866
#define patch_fuseroot2			0x0030687E
#define patch_mprotect			0x00080B8B
#define patch_AllowSLDebugger	0x00317D30
#define patch_AllowULDebugger	0x00318030
#define patch_IsSoftwagner		0x00318110
#define patch_dmamini0			0x005D1EDB
#define patch_dmamini1			0x005D1EDF
#define patch_isDebuggerProcess 0x0008B420
#define patch_isAssistMode	    0x0075CD84
#define offsets_mdbgAssistMode  { 0x1E4, 0x13C2, 0x20CC, 0x20F2, 0x2118, 0x213E, 0x2164, 0x218A, 0x21B0, 0x21D6, 0x21FC, 0x2222, 0x2248, 0x226E, 0x2294, 0x22BA, 0x22E0, 0x2316, 0x233C, 0x2362, 0x2388 };

#endif