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

    static SblMapListEntry* SceSblDriverFindMappedPageListByGpuVa(vm_offset_t gpuVa);
    static vm_offset_t SceSblDriverGpuVaToCpuVa(vm_offset_t gpuVa, size_t* numPageGroups);
    static bool IsFakeSelf(SelfContext* selfContext);
    static int SceSblAuthMgrGetElfHeader(SelfContext* selfContext, Elf64_Ehdr** outElfHeader);
    static int SceSblAuthMgrGetSelfAuthInfoFake(SelfContext* selfContext, SelfAuthInfo* info);
    static int BuildFakeSelfAuthInfo(SelfContext* selfContext, SelfAuthInfo* parentAuthInfo, SelfAuthInfo* authInfo);
    static int AuthSelfHeader(SelfContext* context);

    static int SceSblAuthMgrVerifyHeaderHook(SelfContext* selfContext);
    static int SceSblAuthMgrIsLoadable2Hook(SelfContext* context, SelfAuthInfo* oldAuthInfo, int32_t pathId, SelfAuthInfo* newAuthInfo);
    static int SceSblAuthMgrSmLoadSelfSegment_MailboxHook(uint64_t serviceId, void* request, void* response);
    static int SceSblAuthMgrSmLoadSelfBlock_MailboxHook(uint64_t serviceId, uint8_t* request, void* response);
};
