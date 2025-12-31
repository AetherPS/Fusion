#pragma once

class FakePkgs
{
public:
	static void Init();
	static void Term();

private:
	const static uint8_t PkgKeyP[];
	const static uint8_t PkgKeyQ[];
	const static uint8_t PkgKeyDmp1[];
	const static uint8_t PkgKeyDmq1[];
	const static uint8_t PkgKeyIqmp[];
	const static uint8_t RifDebugKey[];
	const static uint8_t FakeKeySeed[];

	static Detour* SceSblDriverSendMsgDetour;
	static Detour* SceSblPfsSetKeysDetour;
	static Detour* NpdrmDecryptIsolatedRifDetour;
	static Detour* NpdrmDecryptRifNewDetour;
	static Detour* SceSblKeymgrInvalidateKeySxXlockDetour;

	static void GenPfsCryptoKey(uint8_t* encryptionKeyPFS, uint8_t seed[PFS_SEED_SIZE], uint32_t index, uint8_t key[PFS_FINAL_KEY_SIZE]);
	static void GenPfsEncKey(uint8_t* encryptionKeyPFS, uint8_t seed[PFS_SEED_SIZE], uint8_t key[PFS_FINAL_KEY_SIZE]);
	static void GenPfsSignKey(uint8_t* encryptionKeyPFS, uint8_t seed[PFS_SEED_SIZE], uint8_t key[PFS_FINAL_KEY_SIZE]);
	static int DecryptNpdrmDebugRif(uint32_t type, uint8_t* data);
	static SblMapListEntry* SceSblDriverFindMappedPageListByGpuVa(vm_offset_t gpuVa);
	static vm_offset_t SceSblDriverGpuVaToCpuVa(vm_offset_t gpuVa, size_t* numPageGroups);
	static SblKeyRbtreeEntry* sceSblKeymgrGetKey(unsigned int handle);

	static int SceSblDriverSendMsgHook(SblMsg* message, size_t size);
	static int SceSblPfsSetKeysHook(uint32_t* ekh, uint32_t* skh, uint8_t* eekpfs, Ekc* eekc, uint32_t pubkeyVer, uint32_t keyVer, PfsHeader* hdr, size_t hdrSize, uint32_t type, uint32_t finalized, uint32_t isDisc);
	static int NpdrmDecryptIsolatedRifHook(KeymgrPayload* payload);
	static int NpdrmDecryptRifNewHook(KeymgrPayload* payload);
	static int SceSblKeymgrInvalidateKeySxXlockHook(struct sx* sx, int opts, const char* file, int line);
};
