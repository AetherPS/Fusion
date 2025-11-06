#pragma once

class FakeSelf
{
public:
    static void Init();
    static void Term();

private:
    static const uint8_t AuthInfoForExec[0x88];
    static const uint8_t AuthInfoForDynlib[0x88];

    static Detour* SceSblAuthMgrVerifyHeaderDetour1;
    static Detour* SceSblAuthMgrVerifyHeaderDetour2;;
    static Detour* SceSblAuthMgrIsLoadable2Detour;
    static Detour* SceSblAuthMgrSmLoadSelfSegment_MailboxDetour;
    static Detour* SceSblAuthMgrSmLoadSelfBlock_MailboxDetour;

    static SblMapListEntry* SceSblDriverFindMappedPageListByGpuVa(vm_offset_t p_GpuVa);
    static vm_offset_t SceSblDriverGpuVaToCpuVa(vm_offset_t p_GpuVa, size_t* p_NumPageGroups);
    static bool IsFakeSelf(SelfContext* pSelfContext);
    static int SceSblAuthMgrGetElfHeader(SelfContext* pSelfContext, Elf64_Ehdr** pOutElfHeader);
    static int SceSblAuthMgrGetSelfAuthInfoFake(SelfContext* pSelfContext, SelfAuthInfo* pInfo);
    static int BuildFakeSelfAuthInfo(SelfContext* pSelfContext, SelfAuthInfo* pParentAuthInfo, SelfAuthInfo* pAuthInfo);
    static int AuthSelfHeader(SelfContext* pContext);

    static int SceSblAuthMgrVerifyHeaderHook(SelfContext* pSelfContext);
    static int SceSblAuthMgrIsLoadable2Hook(SelfContext* p_Context, SelfAuthInfo* pOldAuthInfo, int32_t pPathId, SelfAuthInfo* pNewAuthInfo);
    static int SceSblAuthMgrSmLoadSelfSegment_MailboxHook(uint64_t pServiceId, void* pRequest, void* pResponse);
    static int SceSblAuthMgrSmLoadSelfBlock_MailboxHook(uint64_t pServiceId, uint8_t* pRequest, void* pResponse);
};
