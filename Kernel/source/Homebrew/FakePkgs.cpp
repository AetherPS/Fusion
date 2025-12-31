#include "Common.h"
#include "FakePkgs.h"

// Credits: https://github.com/OpenOrbis/mira-project

const uint8_t FakePkgs::PkgKeyP[] =
{
	0x2D, 0xE8, 0xB4, 0x65, 0xBE, 0x05, 0x78, 0x6A, 0x89, 0x31, 0xC9, 0x5A, 0x44, 0xDE, 0x50, 0xC1,
	0xC7, 0xFD, 0x9D, 0x3E, 0x21, 0x42, 0x17, 0x40, 0x79, 0xF9, 0xC9, 0x41, 0xC1, 0xFC, 0xD7, 0x0F,
	0x34, 0x76, 0xA3, 0xE2, 0xC0, 0x1B, 0x5A, 0x20, 0x0F, 0xAF, 0x2F, 0x52, 0xCD, 0x83, 0x34, 0x72,
	0xAF, 0xB3, 0x12, 0x33, 0x21, 0x2C, 0x20, 0xB0, 0xC6, 0xA0, 0x2D, 0xB1, 0x59, 0xE3, 0xA7, 0xB0,
	0x4E, 0x1C, 0x4C, 0x5B, 0x5F, 0x10, 0x9A, 0x50, 0x18, 0xCC, 0x86, 0x79, 0x25, 0xFF, 0x10, 0x02,
	0x8F, 0x90, 0x03, 0xA9, 0x37, 0xBA, 0xF2, 0x1C, 0x13, 0xCC, 0x09, 0x45, 0x15, 0xB8, 0x55, 0x74,
	0x0A, 0x28, 0x24, 0x04, 0xD1, 0x19, 0xAB, 0xB3, 0xCA, 0x44, 0xB6, 0xF8, 0x3D, 0xB1, 0x2A, 0x72,
	0x88, 0x35, 0xE4, 0x86, 0x6B, 0x55, 0x47, 0x08, 0x25, 0x16, 0xAB, 0x69, 0x1D, 0xBF, 0xF6, 0xFE,
};

const uint8_t FakePkgs::PkgKeyQ[] =
{
	0x23, 0x80, 0x77, 0x84, 0x4D, 0x6F, 0x9B, 0x24, 0x51, 0xFE, 0x2A, 0x6B, 0x28, 0x80, 0xA1, 0x9E,
	0xBD, 0x6D, 0x18, 0xCA, 0x8D, 0x7D, 0x9E, 0x79, 0x5A, 0xE0, 0xB8, 0xEB, 0xD1, 0x3D, 0xF3, 0xD9,
	0x02, 0x90, 0x2A, 0xA7, 0xB5, 0x7E, 0x9A, 0xA2, 0xD7, 0x2F, 0x21, 0xA8, 0x50, 0x7D, 0x8C, 0xA1,
	0x91, 0x2F, 0xBF, 0x97, 0xBE, 0x92, 0xC2, 0xC1, 0x0D, 0x8C, 0x0C, 0x1F, 0xDE, 0x31, 0x35, 0x15,
	0x39, 0x90, 0xCC, 0x97, 0x47, 0x2E, 0x7F, 0x09, 0xE9, 0xC3, 0x9C, 0xCE, 0x91, 0xB2, 0xC8, 0x58,
	0x76, 0xE8, 0x70, 0x1D, 0x72, 0x5F, 0x4A, 0xE6, 0xAA, 0x36, 0x22, 0x94, 0xC6, 0x52, 0x90, 0xB3,
	0x9F, 0x9B, 0xF0, 0xEF, 0x57, 0x8E, 0x53, 0xC3, 0xE3, 0x30, 0xC9, 0xD7, 0xB0, 0x3A, 0x0C, 0x79,
	0x1B, 0x97, 0xA8, 0xD4, 0x81, 0x22, 0xD2, 0xB0, 0x82, 0x62, 0x7D, 0x00, 0x58, 0x47, 0x9E, 0xC7,
};

const uint8_t FakePkgs::PkgKeyDmp1[] =
{
	0x25, 0x54, 0xDB, 0xFD, 0x86, 0x45, 0x97, 0x9A, 0x1E, 0x17, 0xF0, 0xE3, 0xA5, 0x92, 0x0F, 0x12,
	0x2A, 0x5C, 0x4C, 0xA6, 0xA5, 0xCF, 0x7F, 0xE8, 0x5B, 0xF3, 0x65, 0x1A, 0xC8, 0xCF, 0x9B, 0xB9,
	0x2A, 0xC9, 0x90, 0x5D, 0xD4, 0x08, 0xCF, 0xF6, 0x03, 0x5A, 0x5A, 0xFC, 0x9E, 0xB6, 0xDB, 0x11,
	0xED, 0xE2, 0x3D, 0x62, 0xC1, 0xFC, 0x88, 0x5D, 0x97, 0xAC, 0x31, 0x2D, 0xC3, 0x15, 0xAD, 0x70,
	0x05, 0xBE, 0xA0, 0x5A, 0xE6, 0x34, 0x9C, 0x44, 0x78, 0x2B, 0xE5, 0xFE, 0x38, 0x56, 0xD4, 0x68,
	0x83, 0x13, 0xA4, 0xE6, 0xFA, 0xD2, 0x9C, 0xAB, 0xAC, 0x89, 0x5F, 0x10, 0x8F, 0x75, 0x6F, 0x04,
	0xBC, 0xAE, 0xB9, 0xBC, 0xB7, 0x1D, 0x42, 0xFA, 0x4E, 0x94, 0x1F, 0xB4, 0x0A, 0x27, 0x9C, 0x6B,
	0xAB, 0xC7, 0xD2, 0xEB, 0x27, 0x42, 0x52, 0x29, 0x41, 0xC8, 0x25, 0x40, 0x54, 0xE0, 0x48, 0x6D,
};
const uint8_t FakePkgs::PkgKeyDmq1[] =
{
	0x4D, 0x35, 0x67, 0x38, 0xBC, 0x90, 0x3E, 0x3B, 0xAA, 0x6C, 0xBC, 0xF2, 0xEB, 0x9E, 0x45, 0xD2,
	0x09, 0x2F, 0xCA, 0x3A, 0x9C, 0x02, 0x36, 0xAD, 0x2E, 0xC1, 0xB1, 0xB2, 0x6D, 0x7C, 0x1F, 0x6B,
	0xA1, 0x8F, 0x62, 0x20, 0x8C, 0xD6, 0x6C, 0x36, 0xD6, 0x5A, 0x54, 0x9E, 0x30, 0xA9, 0xA8, 0x25,
	0x3D, 0x94, 0x12, 0x3E, 0x0D, 0x16, 0x1B, 0xF0, 0x86, 0x42, 0x72, 0xE0, 0xD6, 0x9C, 0x39, 0x68,
	0xDB, 0x11, 0x80, 0x96, 0x18, 0x2B, 0x71, 0x41, 0x48, 0x78, 0xE8, 0x17, 0x8B, 0x7D, 0x00, 0x1F,
	0x16, 0x68, 0xD2, 0x75, 0x97, 0xB5, 0xE0, 0xF2, 0x6D, 0x0C, 0x75, 0xAC, 0x16, 0xD9, 0xD5, 0xB1,
	0xB5, 0x8B, 0xE8, 0xD0, 0xBF, 0xA7, 0x1F, 0x61, 0x5B, 0x08, 0xF8, 0x68, 0xE7, 0xF0, 0xD1, 0xBC,
	0x39, 0x60, 0xBF, 0x55, 0x9C, 0x7C, 0x20, 0x30, 0xE8, 0x50, 0x28, 0x44, 0x02, 0xCE, 0x51, 0x2A,
};

const uint8_t FakePkgs::PkgKeyIqmp[] =
{
	0xF5, 0x73, 0xB8, 0x7E, 0x5C, 0x98, 0x7C, 0x87, 0x67, 0xF1, 0xDA, 0xAE, 0xA0, 0xF9, 0x4B, 0xAB,
	0x77, 0xD8, 0xCE, 0x64, 0x6A, 0xC1, 0x4F, 0xA6, 0x9B, 0xB9, 0xAA, 0xCC, 0x76, 0x09, 0xA4, 0x3F,
	0xB9, 0xFA, 0xF5, 0x62, 0x84, 0x0A, 0xB8, 0x49, 0x02, 0xDF, 0x9E, 0xC4, 0x1A, 0x37, 0xD3, 0x56,
	0x0D, 0xA4, 0x6E, 0x15, 0x07, 0x15, 0xA0, 0x8D, 0x97, 0x9D, 0x92, 0x20, 0x43, 0x52, 0xC3, 0xB2,
	0xFD, 0xF7, 0xD3, 0xF3, 0x69, 0xA2, 0x28, 0x4F, 0x62, 0x6F, 0x80, 0x40, 0x5F, 0x3B, 0x80, 0x1E,
	0x5E, 0x38, 0x0D, 0x8B, 0x56, 0xA8, 0x56, 0x58, 0xD8, 0xD9, 0x6F, 0xEA, 0x12, 0x2A, 0x40, 0x16,
	0xC1, 0xED, 0x3D, 0x27, 0x16, 0xA0, 0x63, 0x97, 0x61, 0x39, 0x55, 0xCC, 0x8A, 0x05, 0xFA, 0x08,
	0x28, 0xFD, 0x55, 0x56, 0x31, 0x94, 0x65, 0x05, 0xE7, 0xD3, 0x57, 0x6C, 0x0D, 0x1C, 0x67, 0x0B,
};

const uint8_t FakePkgs::RifDebugKey[] =
{
	0x96, 0xC2, 0x26, 0x8D, 0x69, 0x26, 0x1C, 0x8B, 0x1E, 0x3B, 0x6B, 0xFF, 0x2F, 0xE0, 0x4E, 0x12
};

const uint8_t FakePkgs::FakeKeySeed[] =
{
	0x46, 0x41, 0x4B, 0x45, 0x46, 0x41, 0x4B, 0x45, 0x46, 0x41, 0x4B, 0x45, 0x46, 0x41, 0x4B, 0x45,
};

Detour* FakePkgs::SceSblDriverSendMsgDetour;
Detour* FakePkgs::SceSblPfsSetKeysDetour;
Detour* FakePkgs::NpdrmDecryptIsolatedRifDetour;
Detour* FakePkgs::NpdrmDecryptRifNewDetour;
Detour* FakePkgs::SceSblKeymgrInvalidateKeySxXlockDetour;

void FakePkgs::Init()
{
	Detour::DetourCall(&SceSblDriverSendMsgDetour, KernelBase + addr_SceSblDriverSendMsgHook, (void*)SceSblDriverSendMsgHook);
	Detour::DetourCall(&SceSblPfsSetKeysDetour, KernelBase + addr_SceSblPfsSetKeysHook, (void*)SceSblPfsSetKeysHook);
	Detour::DetourCall(&NpdrmDecryptIsolatedRifDetour, KernelBase + addr_NpdrmDecryptIsolatedRifHook, (void*)NpdrmDecryptIsolatedRifHook);
	Detour::DetourCall(&NpdrmDecryptRifNewDetour, KernelBase + addr_NpdrmDecryptRifNewHook, (void*)NpdrmDecryptRifNewHook);
	Detour::DetourCall(&SceSblKeymgrInvalidateKeySxXlockDetour, KernelBase + addr_SceSblKeymgrInvalidateKeySxXlockHook, (void*)SceSblKeymgrInvalidateKeySxXlockHook);
}

void FakePkgs::Term()
{
	delete SceSblDriverSendMsgDetour;
	delete SceSblPfsSetKeysDetour;
	delete NpdrmDecryptIsolatedRifDetour;
	delete NpdrmDecryptRifNewDetour;
	delete SceSblKeymgrInvalidateKeySxXlockDetour;
}

void FakePkgs::GenPfsCryptoKey(uint8_t* encryptionKeyPFS, uint8_t seed[PFS_SEED_SIZE], uint32_t index, uint8_t key[PFS_FINAL_KEY_SIZE])
{
	auto td = CurrentThread();
	FakeKeyD fakeKeyD;
	memset(&fakeKeyD, 0, sizeof(fakeKeyD));

	fakeKeyD.index = index;
	memcpy(fakeKeyD.seed, seed, PFS_SEED_SIZE);

	fpu_kern_enter(td, fpu_ctx, 0);
	Sha256Hmac(key, (const uint8_t*)&fakeKeyD, sizeof(fakeKeyD), encryptionKeyPFS, EKPFS_SIZE);
	fpu_kern_leave(td, fpu_ctx);
}

void FakePkgs::GenPfsEncKey(uint8_t* encryptionKeyPFS, uint8_t seed[PFS_SEED_SIZE], uint8_t key[PFS_FINAL_KEY_SIZE])
{
	GenPfsCryptoKey(encryptionKeyPFS, seed, 1, key);
}

void FakePkgs::GenPfsSignKey(uint8_t* encryptionKeyPFS, uint8_t seed[PFS_SEED_SIZE], uint8_t key[PFS_FINAL_KEY_SIZE])
{
	GenPfsCryptoKey(encryptionKeyPFS, seed, 2, key);
}

int FakePkgs::DecryptNpdrmDebugRif(uint32_t type, uint8_t* data)
{
	auto td = CurrentThread();
	if (td == nullptr)
		return SCE_SBL_ERROR_NPDRM_ENOTSUP;

	fpu_kern_enter(td, fpu_ctx, 0);
	auto result = AesCbcCfb128Decrypt(data + RIF_DIGEST_SIZE, data + RIF_DIGEST_SIZE, RIF_DATA_SIZE, RifDebugKey, sizeof(RifDebugKey) * 8, data);
	fpu_kern_leave(td, fpu_ctx);
	if (result)
		return SCE_SBL_ERROR_NPDRM_ENOTSUP;

	return result;
}

SblMapListEntry* FakePkgs::SceSblDriverFindMappedPageListByGpuVa(vm_offset_t gpuVa)
{
	if (gpuVa == 0)
	{
		printf("invalid gpu va\n");
		return nullptr;
	}

	SblMapListEntry* gpuVaPageList = *(SblMapListEntry**)gpu_va_page_list;
	SblMapListEntry* finalEntry = nullptr;

	// Lock before we iterate this list, because other paths can absolutely use it concurrently
	_mtx_lock_flags(sbl_drv_msg_mtx, 0, __FILE__, __LINE__);

	while (gpuVaPageList)
	{
		if (gpuVaPageList->gpuVa == gpuVa)
		{
			finalEntry = gpuVaPageList;
			break;
		}

		gpuVaPageList = gpuVaPageList->next;
	}

	_mtx_unlock_flags(sbl_drv_msg_mtx, 0, __FILE__, __LINE__);

	return finalEntry;
}

vm_offset_t FakePkgs::SceSblDriverGpuVaToCpuVa(vm_offset_t gpuVa, size_t* numPageGroups)
{
	auto entry = SceSblDriverFindMappedPageListByGpuVa(gpuVa);
	if (entry == nullptr)
		return 0;

	if (numPageGroups != nullptr)
		*numPageGroups = entry->numPageGroups;

	return entry->cpuVa;
}

SblKeyRbtreeEntry* FakePkgs::sceSblKeymgrGetKey(unsigned int handle)
{
	SblKeyRbtreeEntry* sblKeymgrKeyRbTree = *(SblKeyRbtreeEntry**)sbl_keymgr_key_rbtree;

	while (sblKeymgrKeyRbTree)
	{
		if (sblKeymgrKeyRbTree->handle < handle)
			sblKeymgrKeyRbTree = sblKeymgrKeyRbTree->right;
		else if (sblKeymgrKeyRbTree->handle > handle)
			sblKeymgrKeyRbTree = sblKeymgrKeyRbTree->left;
		else if (sblKeymgrKeyRbTree->handle == handle)
			return sblKeymgrKeyRbTree;
	}

	return nullptr;
}

int FakePkgs::SceSblDriverSendMsgHook(SblMsg* message, size_t size)
{
	if (message->hdr.cmd != SBL_MSG_CCP)
		return sceSblDriverSendMsg(message, size);

	union ccp_op* op = &message->service.ccp.op;
	if (CCP_OP(op->common.cmd) != CCP_OP_AES)
		return sceSblDriverSendMsg(message, size);

	uint32_t s_Mask = CCP_USE_KEY_FROM_SLOT | CCP_GENERATE_KEY_AT_SLOT;
	if ((op->aes.cmd & s_Mask) != s_Mask || (op->aes.key_index != PFS_FAKE_OBF_KEY_ID))
		return sceSblDriverSendMsg(message, size);

	op->aes.cmd &= ~CCP_USE_KEY_FROM_SLOT;

	size_t key_len = 16;

	// reverse key bytes
	for (auto i = 0; i < key_len; ++i)
		op->aes.key[i] = FakeKeySeed[key_len - i - 1];

	return sceSblDriverSendMsg(message, size);
}

int FakePkgs::SceSblPfsSetKeysHook(uint32_t* ekh, uint32_t* skh, uint8_t* eekpfs, Ekc* eekc, uint32_t pubkeyVer, uint32_t keyVer, PfsHeader* hdr, size_t hdrSize, uint32_t type, uint32_t finalized, uint32_t isDisc)
{
	struct thread* td;
	RsaBuffer inData;
	RsaBuffer outData;
	RsaKey key;
	uint8_t ekpfs[EKPFS_SIZE];
	uint8_t iv[16];
	SblKeyDesc encKeyDesc;
	SblKeyDesc signKeyDesc;
	int32_t ret, originalRet = 0;

	ret = originalRet = SceSblPfsSetKeysDetour->Invoke<int>(ekh, skh, eekpfs, eekc, pubkeyVer, keyVer, hdr, hdrSize, type, finalized, isDisc);

	if (ret)
	{
		if (finalized && isDisc != 0)
		{
			// always use isDisc=0 here
			ret = SceSblPfsSetKeysDetour->Invoke<int>(ekh, skh, eekpfs, eekc, pubkeyVer, keyVer, hdr, hdrSize, type, finalized, 0);
			if (ret)
			{
				ret = originalRet;
				goto err;
			}
		}
		else
		{
			memset(&inData, 0, sizeof(inData));
			inData.ptr = eekpfs;
			inData.size = EEKPFS_SIZE;

			memset(&outData, 0, sizeof(outData));
			outData.ptr = ekpfs;
			outData.size = EKPFS_SIZE;

			memset(&key, 0, sizeof(key));
			key.p = (uint8_t*)PkgKeyP;
			key.q = (uint8_t*)PkgKeyQ;
			key.dmp1 = (uint8_t*)PkgKeyDmp1;
			key.dmq1 = (uint8_t*)PkgKeyDmq1;
			key.iqmp = (uint8_t*)PkgKeyIqmp;

			td = CurrentThread();

			fpu_kern_enter(td, fpu_ctx, 0);
			{
				ret = RsaesPkcs1v15Dec2048CRT(&outData, &inData, &key);
			}
			fpu_kern_leave(td, fpu_ctx);

			if (ret)
			{
				ret = originalRet;
				goto err;
			}

			sx_xlock(sbl_pfs_sx, 0);
			{
				memset(&encKeyDesc, 0, sizeof(encKeyDesc));
				{
					encKeyDesc.Pfs.obfuscatedKeyId = PFS_FAKE_OBF_KEY_ID;
					encKeyDesc.Pfs.keySize = sizeof(encKeyDesc.Pfs.escrowedKey);

					GenPfsEncKey(ekpfs, hdr->cryptSeed, encKeyDesc.Pfs.escrowedKey);

					fpu_kern_enter(td, fpu_ctx, 0);
					{
						memset(iv, 0, sizeof(iv));
						ret = AesCbcCfb128Encrypt(encKeyDesc.Pfs.escrowedKey, encKeyDesc.Pfs.escrowedKey, sizeof(encKeyDesc.Pfs.escrowedKey), FakeKeySeed, sizeof(FakeKeySeed) * 8, iv);
					}
					fpu_kern_leave(td, fpu_ctx);
				}

				if (ret)
				{
					printf("AesCbcCfb128Encrypt returned (%d).\n", ret);
					sx_xunlock(sbl_pfs_sx);
					ret = originalRet;
					goto err;
				}

				memset(&signKeyDesc, 0, sizeof(signKeyDesc));
				{
					signKeyDesc.Pfs.obfuscatedKeyId = PFS_FAKE_OBF_KEY_ID;
					signKeyDesc.Pfs.keySize = sizeof(signKeyDesc.Pfs.escrowedKey);

					GenPfsSignKey(ekpfs, hdr->cryptSeed, signKeyDesc.Pfs.escrowedKey);

					fpu_kern_enter(td, fpu_ctx, 0);
					{
						memset(iv, 0, sizeof(iv));
						ret = AesCbcCfb128Encrypt(signKeyDesc.Pfs.escrowedKey, signKeyDesc.Pfs.escrowedKey, sizeof(signKeyDesc.Pfs.escrowedKey), FakeKeySeed, sizeof(FakeKeySeed) * 8, iv);
					}
					fpu_kern_leave(td, fpu_ctx);
				}

				if (ret)
				{
					printf("AesCbcCfb128Encrypt returned (%d).\n", ret);
					sx_xunlock(sbl_pfs_sx);
					ret = originalRet;
					goto err;
				}

				ret = sceSblKeymgrSetKeyForPfs(&encKeyDesc, ekh);
				if (ret)
				{
					if (*ekh != 0xFFFFFFFF)
						sceSblKeymgrClearKey(*ekh);

					sx_xunlock(sbl_pfs_sx);
					ret = originalRet;
					goto err;
				}

				ret = sceSblKeymgrSetKeyForPfs(&signKeyDesc, skh);
				if (ret)
				{
					if (*skh != 0xFFFFFFFF)
						sceSblKeymgrClearKey(*skh);
					sx_xunlock(sbl_pfs_sx);
					ret = originalRet;
					goto err;
				}
			}
			sx_xunlock(sbl_pfs_sx);
		}
	}

err:
	return ret;
}

int FakePkgs::NpdrmDecryptIsolatedRifHook(KeymgrPayload* payload)
{
	// it's SM request, thus we have the GPU address here, so we need to convert it to the CPU address
	KeymgrRequest* request = reinterpret_cast<KeymgrRequest*>(SceSblDriverGpuVaToCpuVa(payload->data, nullptr));

	// try to decrypt rif normally
	int result = NpdrmDecryptIsolatedRifDetour->Invoke<int>(payload);
	if ((result != 0 || payload->status != 0) && request)
	{
		if (request->DecryptRif.type == 0x200)
		{
			// fake?
			result = DecryptNpdrmDebugRif(request->DecryptRif.type, request->DecryptRif.data);
			payload->status = result;
			result = 0;
		}
	}

	return result;
}

int FakePkgs::NpdrmDecryptRifNewHook(KeymgrPayload* payload)
{
	// it's SM request, thus we have the GPU address here, so we need to convert it to the CPU address
	uint64_t bufferGpuVa = payload->data;
	auto request = reinterpret_cast<KeymgrRequest*>(SceSblDriverGpuVaToCpuVa(bufferGpuVa, nullptr));
	auto response = reinterpret_cast<KeymgrResponse*>(request);

	// try to decrypt rif normally
	int result = NpdrmDecryptRifNewDetour->Invoke<int>(payload);
	int originalResult = result;

	// and if it fails then we check if it's fake rif and try to decrypt it by ourselves
	if ((result != 0 || payload->status != 0) && request)
	{
		if (request->DecryptEntireRif.rif.format != 2)
		{
			// not fake?
			goto err;
		}

		result = DecryptNpdrmDebugRif(request->DecryptEntireRif.rif.format, request->DecryptEntireRif.rif.digest);

		if (result)
		{
			result = originalResult;
			goto err;
		}

		/* XXX: sorry, i'm lazy to refactor this crappy code :D basically, we're copying decrypted data to proper place,
		consult with kernel code if offsets needs to be changed */
		memcpy(response->DecryptEntireRif.raw, request->DecryptEntireRif.rif.digest, sizeof(request->DecryptEntireRif.rif.digest));
		memcpy(response->DecryptEntireRif.raw + sizeof(request->DecryptEntireRif.rif.digest), request->DecryptEntireRif.rif.data, sizeof(request->DecryptEntireRif.rif.data));

		memset(response->DecryptEntireRif.raw +
			sizeof(request->DecryptEntireRif.rif.digest) +
			sizeof(request->DecryptEntireRif.rif.data),
			0,
			sizeof(response->DecryptEntireRif.raw) -
			(sizeof(request->DecryptEntireRif.rif.digest) +
				sizeof(request->DecryptEntireRif.rif.data)));

		payload->status = result;
	}

err:
	return result;
}

int FakePkgs::SceSblKeymgrInvalidateKeySxXlockHook(struct sx* sx, int opts, const char* file, int line)
{
	auto sblKeymgrBufGva = sbl_keymgr_buf_gva;
	auto sblKeymgrBufVa = sbl_keymgr_buf_va;
	auto sblKeymgrKeySlots = sbl_keymgr_key_slots;

	SblKeyRbtreeEntry* keyDesc;
	SblKeySlotDesc* keySlotDesc;

	unsigned keyHandle;
	auto lockResult = sx_xlock(sx, opts);

	if (TAILQ_EMPTY(sblKeymgrKeySlots))
		goto done;

	TAILQ_FOREACH(keySlotDesc, sblKeymgrKeySlots, list)
	{
		keyHandle = keySlotDesc->keyHandle;
		if (keyHandle == (unsigned int)-1)
		{
			// unbounded
			continue;
		}

		keyDesc = sceSblKeymgrGetKey(keyHandle);
		if (!keyDesc)
		{
			// Shouldn't happen in normal situations.
			continue;
		}

		if (!keyDesc->occupied)
		{
			continue;
		}

		if (keyDesc->desc.Pfs.obfuscatedKeyId != PFS_FAKE_OBF_KEY_ID)
		{
			// not our key, just skip, so it will be handled by original code.
			continue;
		}

		if (keyDesc->desc.Pfs.keySize != sizeof(keyDesc->desc.Pfs.escrowedKey))
		{
			// something weird with key params, just ignore and app will just crash...
			continue;
		}

		memcpy(sblKeymgrBufVa, keyDesc->desc.Pfs.escrowedKey, keyDesc->desc.Pfs.keySize);
		if (sceSblKeymgrSetKeyStorage(*sblKeymgrBufGva, keyDesc->desc.Pfs.keySize, keyDesc->desc.Pfs.obfuscatedKeyId, keySlotDesc->keyId))
		{
			continue;
		}
	}

done:
	// XXX: no need to call SX unlock because we'll jump to original code which expects SX is already locked
	return lockResult;
}