#include "Common.h"
#include "FakeSelfs.h"
#include "Types.h"

const uint8_t FakeSelf::AuthInfoForExec[] =
{
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03, 0x00, 0x20,
  0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00,
  0x00, 0x40, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const uint8_t FakeSelf::AuthInfoForDynlib[] =
{
  0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x30, 0x00, 0x30,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00,
  0x00, 0x40, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

DetourCall* FakeSelf::SceSblAuthMgrVerifyHeaderDetour1;
DetourCall* FakeSelf::SceSblAuthMgrVerifyHeaderDetour2;
DetourCall* FakeSelf::SceSblAuthMgrIsLoadable2Detour;
DetourCall* FakeSelf::SceSblAuthMgrSmLoadSelfSegment_MailboxDetour;
DetourCall* FakeSelf::SceSblAuthMgrSmLoadSelfBlock_MailboxDetour;

void FakeSelf::Init()
{
    SceSblAuthMgrVerifyHeaderDetour1 = new DetourCall(&SceSblAuthMgrVerifyHeaderDetour1, (uint64_t)(KernelBase + addr_sceSblAuthMgrVerifyHeaderHook1), SceSblAuthMgrVerifyHeaderHook);
    SceSblAuthMgrVerifyHeaderDetour2 = new DetourCall(&SceSblAuthMgrVerifyHeaderDetour1, (uint64_t)(KernelBase + addr_sceSblAuthMgrVerifyHeaderHook2), SceSblAuthMgrVerifyHeaderHook);
    SceSblAuthMgrIsLoadable2Detour = new DetourCall(&SceSblAuthMgrIsLoadable2Detour, (uint64_t)(KernelBase + addr_SceSblAuthMgrIsLoadable2Hook), SceSblAuthMgrIsLoadable2Hook);
    SceSblAuthMgrSmLoadSelfSegment_MailboxDetour = new DetourCall(&SceSblAuthMgrSmLoadSelfSegment_MailboxDetour, (uint64_t)(KernelBase + addr_SceSblAuthMgrSmLoadSelfSegment_Mailbox), SceSblAuthMgrSmLoadSelfSegment_MailboxHook);
    SceSblAuthMgrSmLoadSelfBlock_MailboxDetour = new DetourCall(&SceSblAuthMgrSmLoadSelfBlock_MailboxDetour, (uint64_t)(KernelBase + addr_SceSblAuthMgrSmLoadSelfBlock_Mailbox), SceSblAuthMgrSmLoadSelfBlock_MailboxHook);
}

void FakeSelf::Term()
{
    delete SceSblAuthMgrVerifyHeaderDetour1;
    delete SceSblAuthMgrVerifyHeaderDetour2;
    delete SceSblAuthMgrIsLoadable2Detour;
    delete SceSblAuthMgrSmLoadSelfSegment_MailboxDetour;
    delete SceSblAuthMgrSmLoadSelfBlock_MailboxDetour;
}

SblMapListEntry* FakeSelf::SceSblDriverFindMappedPageListByGpuVa(vm_offset_t p_GpuVa)
{
    if (p_GpuVa == 0)
    {
        kprintf("invalid gpu va\n");
        return nullptr;
    }

    SblMapListEntry* gpuVaPageList = *(SblMapListEntry**)gpu_va_page_list;
    SblMapListEntry* finalEntry = nullptr;

    // Lock before we iterate this list, because other paths can absolutely use it concurrently
    _mtx_lock_flags(sbl_drv_msg_mtx, 0, __FILE__, __LINE__);

    while (gpuVaPageList)
    {
        if (gpuVaPageList->gpuVa == p_GpuVa)
        {
            finalEntry = gpuVaPageList;
            break;
        }

        gpuVaPageList = gpuVaPageList->next;
    }

    _mtx_unlock_flags(sbl_drv_msg_mtx, 0, __FILE__, __LINE__);

    return finalEntry;
}

vm_offset_t FakeSelf::SceSblDriverGpuVaToCpuVa(vm_offset_t p_GpuVa, size_t* p_NumPageGroups)
{
    SblMapListEntry* s_Entry = SceSblDriverFindMappedPageListByGpuVa(p_GpuVa);
    if (s_Entry == nullptr)
    {
        kprintf("invalid gpu va entry\n");
        return 0;
    }

    if (p_NumPageGroups != nullptr)
        *p_NumPageGroups = s_Entry->numPageGroups;

    return s_Entry->cpuVa;
}

bool FakeSelf::IsFakeSelf(SelfContext* pSelfContext)
{
    if (pSelfContext == nullptr)
    {
        kprintf("pSelfContext was NULL\n");
        return false;
    }

    SelfExInfo* s_Info = nullptr;
    if (pSelfContext != nullptr && pSelfContext->format == SelfFormat::Self)
    {
        (void)sceSblAuthMgrGetSelfInfo(pSelfContext, &s_Info);

        //WriteLog(LL_Debug, "ptype: (%d)", s_Info->ptype);
        return (int32_t)s_Info->ptype == 1;
    }
    else
        return false;
}

int FakeSelf::SceSblAuthMgrGetElfHeader(SelfContext* pSelfContext, Elf64_Ehdr** pOutElfHeader)
{
    if (pSelfContext == nullptr)
        return -EAGAIN;

    if (pSelfContext->format == SelfFormat::Elf)
    {
        auto s_ElfHeader = reinterpret_cast<Elf64_Ehdr*>(pSelfContext->header);
        if (s_ElfHeader != nullptr)
            *pOutElfHeader = s_ElfHeader;

        return 0;
    }
    else if (pSelfContext->format == SelfFormat::Self)
    {
        auto s_SelfHeader = reinterpret_cast<SelfHeader*>(pSelfContext->header);
        size_t s_PdataSize = s_SelfHeader->headerSize - sizeof(SelfEntry) * s_SelfHeader->numEntries - sizeof(SelfHeader);
        if (s_PdataSize >= sizeof(Elf64_Ehdr) && (s_PdataSize & 0xF) == 0)
        {
            auto s_ElfHeader = reinterpret_cast<Elf64_Ehdr*>((uint8_t*)s_SelfHeader + sizeof(SelfHeader) + sizeof(SelfEntry) * s_SelfHeader->numEntries);
            if (s_ElfHeader)
                *pOutElfHeader = s_ElfHeader;

            return 0;
        }

        kprintf("-EALREADY\n");
        return -37;
    }

    kprintf("-EAGAIN\n");
    return -EAGAIN;
}

int FakeSelf::SceSblAuthMgrGetSelfAuthInfoFake(SelfContext* pSelfContext, SelfAuthInfo* pInfo)
{
    if (pSelfContext == nullptr)
    {
        kprintf("invalid context\n");
        return -EAGAIN;
    }

    if (pSelfContext->format == SelfFormat::Elf)
    {
        kprintf("invalid format\n");
        return -EAGAIN;
    }

    SelfHeader* s_Header = (SelfHeader*)pSelfContext->header;
    auto s_Data = reinterpret_cast<const char*>(pSelfContext->header);
    auto s_FakeInfo = reinterpret_cast<const SelfFakeAuthInfo*>(s_Data + s_Header->headerSize + s_Header->metaSize - 0x100);
    if (s_FakeInfo->size == sizeof(s_FakeInfo->info))
    {
        memcpy(pInfo, &s_FakeInfo->info, sizeof(*pInfo));
        return 0;
    }

    return -37;
}

int FakeSelf::BuildFakeSelfAuthInfo(SelfContext* pSelfContext, SelfAuthInfo* pParentAuthInfo, SelfAuthInfo* pAuthInfo)
{
    if (pSelfContext == nullptr || pParentAuthInfo == nullptr || pAuthInfo == nullptr)
    {
        kprintf("invalid context (%p) || parentAuthInfo (%p) || authInfo (%p)\n", pSelfContext, pParentAuthInfo, pAuthInfo);
        return -EINVAL;
    }

    if (!IsFakeSelf(pSelfContext))
    {
        kprintf("not fake self\n");
        return -EINVAL;
    }

    SelfExInfo* s_ExInfo = nullptr;
    int32_t s_Result = sceSblAuthMgrGetSelfInfo(pSelfContext, &s_ExInfo);
    if (s_Result)
    {
        kprintf("could not get self info (%d).\n", s_Result);
        return s_Result;
    }

    Elf64_Ehdr* sElfHeader = nullptr;
    s_Result = SceSblAuthMgrGetElfHeader(pSelfContext, &sElfHeader);
    if (s_Result)
    {
        kprintf("could not get elf header (%d).\n", s_Result);
        return s_Result;
    }

    if (sElfHeader == nullptr)
    {
        kprintf("elf header invalid\n");
        return -ESRCH;
    }

    SelfAuthInfo s_Info = { 0 };
    s_Result = SceSblAuthMgrGetSelfAuthInfoFake(pSelfContext, &s_Info);
    if (s_Result)
    {
        switch (sElfHeader->e_type)
        {
        case ET_EXEC:
        case ET_SCE_EXEC:
        case ET_SCE_EXEC_ASLR:
            memcpy(&s_Info, AuthInfoForExec, sizeof(s_Info));
            s_Result = 0;
            break;
        case ET_SCE_DYNAMIC:
            memcpy(&s_Info, AuthInfoForDynlib, sizeof(s_Info));
            s_Result = 0;
            break;
        default:
            s_Result = 45;
            return s_Result;
        }

        s_Info.paid = s_ExInfo->paid;
    }

    if (pAuthInfo)
        memcpy(pAuthInfo, &s_Info, sizeof(*pAuthInfo));

    return s_Result;
}

int FakeSelf::AuthSelfHeader(SelfContext* pContext)
{
    bool s_IsUnsigned = pContext->format == SelfFormat::Elf || IsFakeSelf(pContext);
    if (s_IsUnsigned)
    {
        auto s_OldFormat = pContext->format;
        auto s_OldTotalHeaderSize = pContext->totalHeaderSize;
        auto s_NewTotalHeaderSize = mini_syscore_self_binary->headerSize + mini_syscore_self_binary->metaSize;

        // Allocate some memory to hold our header size
        auto s_Temp = new uint8_t[s_NewTotalHeaderSize];
        if (s_Temp == nullptr)
        {
            kprintf("could not allocate new total header size (%x).\n", s_NewTotalHeaderSize);
            return ENOMEM;
        }

        // Backup our current header
        memcpy(s_Temp, pContext->header, s_NewTotalHeaderSize);

        // Copy over mini-syscore.elf's header
        memcpy(pContext->header, mini_syscore_self_binary, s_NewTotalHeaderSize);

        // Change the format
        pContext->format = SelfFormat::Self;
        pContext->totalHeaderSize = s_NewTotalHeaderSize;

        // xxx: call the original method using a real SELF file
        //auto s_Result = SceSblAuthMgrVerifyHeaderDetour->Invoke<int>(pContext);
        auto s_Result = sceSblAuthMgrVerifyHeader(pContext);
        if (s_Result != 0)
        {
            kprintf("Cunts fucked\n");
            delete[] s_Temp;
            return s_Result;
        }

        // Restore everything
        memcpy(pContext->header, s_Temp, s_NewTotalHeaderSize);
        pContext->format = s_OldFormat;
        pContext->totalHeaderSize = s_OldTotalHeaderSize;

        delete[] s_Temp;

        return s_Result;
    }
    else
        //return SceSblAuthMgrVerifyHeaderDetour->Invoke<int>(pContext);
        return sceSblAuthMgrVerifyHeader(pContext);
}

int FakeSelf::SceSblAuthMgrVerifyHeaderHook(SelfContext* pSelfContext)
{
    void* s_Temp = nullptr;
    sceSblAuthMgrSmStart(&s_Temp);

    return AuthSelfHeader(pSelfContext);
}

int FakeSelf::SceSblAuthMgrIsLoadable2Hook(SelfContext* pSelfContext, SelfAuthInfo* pOldAuthInfo, int32_t pPathId, SelfAuthInfo* pNewAuthInfo)
{
    if (pSelfContext == nullptr)
    {
        kprintf("pSelfContext was NULL\n");
        return SceSblAuthMgrIsLoadable2Detour->Invoke<int>(pSelfContext, pOldAuthInfo, pPathId, pNewAuthInfo);
    }

    if (pSelfContext->format == SelfFormat::Elf || IsFakeSelf(pSelfContext))
    {
        return BuildFakeSelfAuthInfo(pSelfContext, pOldAuthInfo, pNewAuthInfo);
    }
    else
        return SceSblAuthMgrIsLoadable2Detour->Invoke<int>(pSelfContext, pOldAuthInfo, pPathId, pNewAuthInfo);
}

int FakeSelf::SceSblAuthMgrSmLoadSelfSegment_MailboxHook(uint64_t pServiceId, void* pRequest, void* pResponse)
{
    // self_context is first param of caller. 0x08 = sizeof(struct self_context*)
    uint8_t* frame = (uint8_t*)__builtin_frame_address(1);
    SelfContext* s_Context = *(SelfContext**)(frame - 0x08);

    auto s_RequestMessage = static_cast<MailboxMessage*>(pRequest);
    if (s_RequestMessage == nullptr)
    {
        kprintf("invalid response\n");
        return SceSblAuthMgrSmLoadSelfSegment_MailboxDetour->Invoke<int>(pServiceId, pRequest, pResponse);
    }

    if (s_Context == nullptr)
    {
        kprintf("could not load segment, could not get self context.\n");
        return SceSblAuthMgrSmLoadSelfSegment_MailboxDetour->Invoke<int>(pServiceId, pRequest, pResponse);
    }

    bool s_IsUnsigned = s_Context && IsFakeSelf(s_Context);
    if (s_IsUnsigned)
    {
        s_RequestMessage->retVal = 0;
        return 0;
    }

    return SceSblAuthMgrSmLoadSelfSegment_MailboxDetour->Invoke<int>(pServiceId, pRequest, pResponse);
}

int FakeSelf::SceSblAuthMgrSmLoadSelfBlock_MailboxHook(uint64_t pServiceId, uint8_t* pRequest, void* pResponse)
{
    // self_context is first param of caller. 0x08 = sizeof(struct self_context*)
    uint8_t* frame = (uint8_t*)__builtin_frame_address(1);
    SelfContext* pContext = *(SelfContext**)(frame - 0x08);

    bool s_IsUnsigned = pContext && (pContext->format == SelfFormat::Elf || IsFakeSelf((SelfContext*)pContext));

    if (!s_IsUnsigned)
    {
        return SceSblAuthMgrSmLoadSelfBlock_MailboxDetour->Invoke<int>(pServiceId, pRequest, pResponse);
    }
    else
    {

        vm_offset_t s_SegmentDataGpuVa = *(uint64_t*)(pRequest + 0x08);
        vm_offset_t s_CurrentDataGpuVa = *(uint64_t*)(pRequest + 0x50);
        vm_offset_t s_CurrentData2GpuVa = *(uint64_t*)(pRequest + 0x58);

        uint32_t s_DataOffset = *(uint32_t*)(pRequest + 0x44);
        uint32_t s_DataSize = *(uint32_t*)(pRequest + 0x48);

        // looking into lists of GPU's mapped memory regions
        vm_offset_t s_SegmentDataCpuVa = SceSblDriverGpuVaToCpuVa(s_SegmentDataGpuVa, NULL);
        vm_offset_t s_CurrentDataCpuVa = SceSblDriverGpuVaToCpuVa(s_CurrentDataGpuVa, NULL);
        vm_offset_t s_CurrentData2CpuVa = s_CurrentData2GpuVa ? SceSblDriverGpuVaToCpuVa(s_CurrentData2GpuVa, NULL) : 0;

        if (s_SegmentDataCpuVa && s_CurrentDataCpuVa)
        {
            if (s_CurrentData2GpuVa && s_CurrentData2GpuVa != s_CurrentDataGpuVa && s_DataOffset > 0)
            {

                // data spans two consecutive memory's pages, so we need to copy twice
                uint32_t s_Size = PAGE_SIZE - s_DataOffset;
                memcpy((char*)s_SegmentDataCpuVa, (char*)s_CurrentDataCpuVa + s_DataOffset, s_Size);

                // prevent *potential* kpanic here
                if (s_CurrentData2CpuVa)
                {
                    memcpy((char*)s_SegmentDataCpuVa + s_Size, (char*)s_CurrentData2CpuVa, s_DataSize - s_Size);
                }
            }
            else
            {
                memcpy((char*)s_SegmentDataCpuVa, (char*)s_CurrentDataCpuVa + s_DataOffset, s_DataSize);
            }
        }

        // setting error field to zero, thus we have no errors
        *(int*)(pRequest + 0x04) = 0;

        return 0;
    }
}
