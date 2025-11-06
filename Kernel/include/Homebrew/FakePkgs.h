#pragma once

class FakePkgs
{
public:
	static void Init();
	static void Term();
	static void OnProcessStart(void* arg, proc* p);

private:
	const static uint8_t PkgKeyP[];
	const static uint8_t PkgKeyQ[];
	const static uint8_t PkgKeyDmp1[];
	const static uint8_t PkgKeyDmq1[];
	const static uint8_t PkgKeyIqmp[];
	const static uint8_t RifDebugKey[];
	const static uint8_t FakeKeySeed[];

	static DetourCall* SceSblDriverSendMsgDetour;
	static DetourCall* SceSblPfsSetKeysDetour;
	static DetourCall* NpdrmDecryptIsolatedRifDetour;
	static DetourCall* NpdrmDecryptRifNewDetour;
	static DetourCall* SceSblKeymgrInvalidateKeySxXlockDetour;

	static void InstallShellCorePatches();
	static void InstallShellUIPatches(proc* p);

	static void GenPfsCryptoKey(uint8_t* p_EncryptionKeyPFS, uint8_t p_Seed[PFS_SEED_SIZE], uint32_t p_Index, uint8_t p_Key[PFS_FINAL_KEY_SIZE]);
	static void GenPfsEncKey(uint8_t* p_EncryptionKeyPFS, uint8_t p_Seed[PFS_SEED_SIZE], uint8_t p_Key[PFS_FINAL_KEY_SIZE]);
	static void GenPfsSignKey(uint8_t* p_EncryptionKeyPFS, uint8_t p_Seed[PFS_SEED_SIZE], uint8_t p_Key[PFS_FINAL_KEY_SIZE]);
	static int DecryptNpdrmDebugRif(uint32_t p_Type, uint8_t* p_Data);
	static SblMapListEntry* SceSblDriverFindMappedPageListByGpuVa(vm_offset_t p_GpuVa);
	static vm_offset_t SceSblDriverGpuVaToCpuVa(vm_offset_t p_GpuVa, size_t* p_NumPageGroups);
	static SblKeyRbtreeEntry* sceSblKeymgrGetKey(unsigned int p_Handle);

	static int SceSblDriverSendMsgHook(SblMsg* p_Message, size_t p_Size);
	static int SceSblPfsSetKeysHook(uint32_t* ekh, uint32_t* skh, uint8_t* eekpfs, Ekc* eekc, uint32_t pubkey_ver, uint32_t key_ver, PfsHeader* hdr, size_t hdr_size, uint32_t type, uint32_t finalized, uint32_t is_disc);
	static int NpdrmDecryptIsolatedRifHook(KeymgrPayload* p_Payload);
	static int NpdrmDecryptRifNewHook(KeymgrPayload* p_Payload);
	static int SceSblKeymgrInvalidateKeySxXlockHook(struct sx* p_Sx, int p_Opts, const char* p_File, int p_Line);
};
