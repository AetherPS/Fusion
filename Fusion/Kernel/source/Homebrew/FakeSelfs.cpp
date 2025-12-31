#include "Common.h"
#include "FakeSelfs.h"
#include "Types/All.h"

// Credits: https://github.com/OpenOrbis/mira-project

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

Detour* FakeSelf::SceSblAuthMgrVerifyHeaderDetour1;
Detour* FakeSelf::SceSblAuthMgrVerifyHeaderDetour2;
Detour* FakeSelf::SceSblAuthMgrIsLoadable2Detour;
Detour* FakeSelf::SceSblAuthMgrSmLoadSelfSegment_MailboxDetour;
Detour* FakeSelf::SceSblAuthMgrSmLoadSelfBlock_MailboxDetour;

void FakeSelf::Init()
{
    Detour::DetourCall(&SceSblAuthMgrVerifyHeaderDetour1, KernelBase + addr_sceSblAuthMgrVerifyHeaderHook1, (void*)SceSblAuthMgrVerifyHeaderHook);
    Detour::DetourCall(&SceSblAuthMgrVerifyHeaderDetour1, KernelBase + addr_sceSblAuthMgrVerifyHeaderHook2, (void*)SceSblAuthMgrVerifyHeaderHook);
    Detour::DetourCall(&SceSblAuthMgrIsLoadable2Detour, KernelBase + addr_SceSblAuthMgrIsLoadable2Hook, (void*)SceSblAuthMgrIsLoadable2Hook);
    Detour::DetourCall(&SceSblAuthMgrSmLoadSelfSegment_MailboxDetour, KernelBase + addr_SceSblAuthMgrSmLoadSelfSegment_Mailbox, (void*)SceSblAuthMgrSmLoadSelfSegment_MailboxHook);
    Detour::DetourCall(&SceSblAuthMgrSmLoadSelfBlock_MailboxDetour, KernelBase + addr_SceSblAuthMgrSmLoadSelfBlock_Mailbox, (void*)SceSblAuthMgrSmLoadSelfBlock_MailboxHook);
}

void FakeSelf::Term()
{
    delete SceSblAuthMgrVerifyHeaderDetour1;
    delete SceSblAuthMgrVerifyHeaderDetour2;
    delete SceSblAuthMgrIsLoadable2Detour;
    delete SceSblAuthMgrSmLoadSelfSegment_MailboxDetour;
    delete SceSblAuthMgrSmLoadSelfBlock_MailboxDetour;
}

SblMapListEntry* FakeSelf::SceSblDriverFindMappedPageListByGpuVa(vm_offset_t gpuVa)
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

vm_offset_t FakeSelf::SceSblDriverGpuVaToCpuVa(vm_offset_t gpuVa, size_t* numPageGroups)
{
    SblMapListEntry* entry = SceSblDriverFindMappedPageListByGpuVa(gpuVa);
    if (entry == nullptr)
    {
        printf("invalid gpu va entry\n");
        return 0;
    }

    if (numPageGroups != nullptr)
        *numPageGroups = entry->numPageGroups;

    return entry->cpuVa;
}

bool FakeSelf::IsFakeSelf(SelfContext* selfContext)
{
    if (selfContext == nullptr)
    {
        printf("pSelfContext was NULL\n");
        return false;
    }

    SelfExInfo* info = nullptr;
    if (selfContext != nullptr && selfContext->format == SelfFormat::Self)
    {
        (void)sceSblAuthMgrGetSelfInfo(selfContext, &info);

        // printf("ptype: (%d)", info->ptype);
        return (int32_t)info->ptype == 1;
    }
    else
        return false;
}

int FakeSelf::SceSblAuthMgrGetElfHeader(SelfContext* selfContext, Elf64_Ehdr** outElfHeader)
{
    if (selfContext == nullptr)
        return -EAGAIN;

    if (selfContext->format == SelfFormat::Elf)
    {
        auto elfHeader = reinterpret_cast<Elf64_Ehdr*>(selfContext->header);
        if (elfHeader != nullptr)
            *outElfHeader = elfHeader;

        return 0;
    }
    else if (selfContext->format == SelfFormat::Self)
    {
        auto selfHeader = reinterpret_cast<SelfHeader*>(selfContext->header);
        size_t dataSize = selfHeader->headerSize - sizeof(SelfEntry) * selfHeader->numEntries - sizeof(SelfHeader);
        if (dataSize >= sizeof(Elf64_Ehdr) && (dataSize & 0xF) == 0)
        {
            auto elfHeader = reinterpret_cast<Elf64_Ehdr*>((uint8_t*)selfHeader + sizeof(SelfHeader) + sizeof(SelfEntry) * selfHeader->numEntries);
            if (elfHeader)
                *outElfHeader = elfHeader;

            return 0;
        }

        return -EALREADY;
    }

    return -EAGAIN;
}

int FakeSelf::SceSblAuthMgrGetSelfAuthInfoFake(SelfContext* selfContext, SelfAuthInfo* info)
{
    if (selfContext == nullptr)
    {
        printf("Invalid context.\n");
        return -EAGAIN;
    }

    if (selfContext->format == SelfFormat::Elf)
    {
        printf("Invalid format.\n");
        return -EAGAIN;
    }

    SelfHeader* header = (SelfHeader*)selfContext->header;
    auto data = reinterpret_cast<const char*>(selfContext->header);
    auto fakeInfo = reinterpret_cast<const SelfFakeAuthInfo*>(data + header->headerSize + header->metaSize - 0x100);
    if (fakeInfo->size == sizeof(fakeInfo->info))
    {
        memcpy(info, &fakeInfo->info, sizeof(*info));
        return 0;
    }

    return -EALREADY;
}

int FakeSelf::BuildFakeSelfAuthInfo(SelfContext* selfContext, SelfAuthInfo* parentAuthInfo, SelfAuthInfo* authInfo)
{
    if (selfContext == nullptr || parentAuthInfo == nullptr || authInfo == nullptr)
    {
        printf("Invalid context (%p) || parentAuthInfo (%p) || authInfo (%p)\n", selfContext, parentAuthInfo, authInfo);
        return -EINVAL;
    }

    if (!IsFakeSelf(selfContext))
    {
        printf("Not fake self\n");
        return -EINVAL;
    }

    SelfExInfo* exInfo = nullptr;
    int32_t result = sceSblAuthMgrGetSelfInfo(selfContext, &exInfo);
    if (result)
    {
        printf("Could not get self info (%d).\n", result);
        return result;
    }

    Elf64_Ehdr* elfHeader = nullptr;
    result = SceSblAuthMgrGetElfHeader(selfContext, &elfHeader);
    if (result)
    {
        printf("Could not get elf header (%d).\n", result);
        return result;
    }

    if (elfHeader == nullptr)
    {
        printf("ELF header invalid\n");
        return -ESRCH;
    }

    SelfAuthInfo info = { 0 };
    result = SceSblAuthMgrGetSelfAuthInfoFake(selfContext, &info);
    if (result)
    {
        switch (elfHeader->e_type)
        {
        case ET_EXEC:
        case ET_SCE_EXEC:
        case ET_SCE_EXEC_ASLR:
            memcpy(&info, AuthInfoForExec, sizeof(info));
            result = 0;
            break;
        case ET_SCE_DYNAMIC:
            memcpy(&info, AuthInfoForDynlib, sizeof(info));
            result = 0;
            break;
        default:
            result = 45;
            return result;
        }

        info.paid = exInfo->paid;
    }

    if (authInfo)
        memcpy(authInfo, &info, sizeof(*authInfo));

    return result;
}

int FakeSelf::AuthSelfHeader(SelfContext* pContext)
{
    bool isUnsigned = pContext->format == SelfFormat::Elf || IsFakeSelf(pContext);
    if (isUnsigned)
    {
        auto oldFormat = pContext->format;
        auto oldTotalHeaderSize = pContext->totalHeaderSize;
        auto newTotalHeaderSize = mini_syscore_self_binary->headerSize + mini_syscore_self_binary->metaSize;

        // Allocate some memory to hold our header size
        auto temp = new uint8_t[newTotalHeaderSize];
        if (temp == nullptr)
        {
            printf("could not allocate new total header size (%x).\n", newTotalHeaderSize);
            return ENOMEM;
        }

        // Backup our current header
        memcpy(temp, pContext->header, newTotalHeaderSize);

        // Copy over mini-syscore.elf's header
        memcpy(pContext->header, mini_syscore_self_binary, newTotalHeaderSize);

        // Change the format
        pContext->format = SelfFormat::Self;
        pContext->totalHeaderSize = newTotalHeaderSize;

        // xxx: call the original method using a real SELF file
        auto result = sceSblAuthMgrVerifyHeader(pContext);
        if (result != 0)
        {
            delete[] temp;
            return result;
        }

        // Restore everything
        memcpy(pContext->header, temp, newTotalHeaderSize);
        pContext->format = oldFormat;
        pContext->totalHeaderSize = oldTotalHeaderSize;

        delete[] temp;

        return result;
    }
    else
        return sceSblAuthMgrVerifyHeader(pContext);
}

int FakeSelf::SceSblAuthMgrVerifyHeaderHook(SelfContext* selfContext)
{
    void* temp = nullptr;
    sceSblAuthMgrSmStart(&temp);

    return AuthSelfHeader(selfContext);
}

int FakeSelf::SceSblAuthMgrIsLoadable2Hook(SelfContext* selfContext, SelfAuthInfo* oldAuthInfo, int32_t pathId, SelfAuthInfo* newAuthInfo)
{
    if (selfContext == nullptr)
    {
        printf("pSelfContext was NULL\n");
        return SceSblAuthMgrIsLoadable2Detour->Invoke<int>(selfContext, oldAuthInfo, pathId, newAuthInfo);
    }

    if (selfContext->format == SelfFormat::Elf || IsFakeSelf(selfContext))
    {
        return BuildFakeSelfAuthInfo(selfContext, oldAuthInfo, newAuthInfo);
    }
    else
        return SceSblAuthMgrIsLoadable2Detour->Invoke<int>(selfContext, oldAuthInfo, pathId, newAuthInfo);
}

int FakeSelf::SceSblAuthMgrSmLoadSelfSegment_MailboxHook(uint64_t serviceId, void* request, void* response)
{
    // self_context is first param of caller. 0x08 = sizeof(struct self_context*)
    uint8_t* frame = (uint8_t*)__builtin_frame_address(1);
    SelfContext* context = *(SelfContext**)(frame - 0x08);

    auto requestMessage = static_cast<MailboxMessage*>(request);
    if (requestMessage == nullptr)
    {
        printf("invalid response\n");
        return SceSblAuthMgrSmLoadSelfSegment_MailboxDetour->Invoke<int>(serviceId, request, response);
    }

    if (context == nullptr)
    {
        printf("could not load segment, could not get self context.\n");
        return SceSblAuthMgrSmLoadSelfSegment_MailboxDetour->Invoke<int>(serviceId, request, response);
    }

    bool isUnsigned = context && IsFakeSelf(context);
    if (isUnsigned)
    {
        requestMessage->retVal = 0;
        return 0;
    }

    return SceSblAuthMgrSmLoadSelfSegment_MailboxDetour->Invoke<int>(serviceId, request, response);
}

int FakeSelf::SceSblAuthMgrSmLoadSelfBlock_MailboxHook(uint64_t serviceId, uint8_t* request, void* response)
{
    // self_context is first param of caller. 0x08 = sizeof(struct self_context*)
    uint8_t* frame = (uint8_t*)__builtin_frame_address(1);
    SelfContext* context = *(SelfContext**)(frame - 0x08);

    bool isUnsigned = context && (context->format == SelfFormat::Elf || IsFakeSelf((SelfContext*)context));

    if (!isUnsigned)
    {
        return SceSblAuthMgrSmLoadSelfBlock_MailboxDetour->Invoke<int>(serviceId, request, response);
    }
    else
    {

        vm_offset_t segmentDataGpuVa = *(uint64_t*)(request + 0x08);
        vm_offset_t currentDataGpuVa = *(uint64_t*)(request + 0x50);
        vm_offset_t currentData2GpuVa = *(uint64_t*)(request + 0x58);

        uint32_t dataOffset = *(uint32_t*)(request + 0x44);
        uint32_t dataSize = *(uint32_t*)(request + 0x48);

        // looking into lists of GPU's mapped memory regions
        vm_offset_t segmentDataCpuVa = SceSblDriverGpuVaToCpuVa(segmentDataGpuVa, NULL);
        vm_offset_t currentDataCpuVa = SceSblDriverGpuVaToCpuVa(currentDataGpuVa, NULL);
        vm_offset_t currentData2CpuVa = currentData2GpuVa ? SceSblDriverGpuVaToCpuVa(currentData2GpuVa, NULL) : 0;

        if (segmentDataCpuVa && currentDataCpuVa)
        {
            if (currentData2GpuVa && currentData2GpuVa != currentDataGpuVa && dataOffset > 0)
            {

                // data spans two consecutive memory's pages, so we need to copy twice
                uint32_t size = PAGE_SIZE - dataOffset;
                memcpy((char*)segmentDataCpuVa, (char*)currentDataCpuVa + dataOffset, size);

                // prevent *potential* kpanic here
                if (currentData2CpuVa)
                {
                    memcpy((char*)segmentDataCpuVa + size, (char*)currentData2CpuVa, dataSize - size);
                }
            }
            else
            {
                memcpy((char*)segmentDataCpuVa, (char*)currentDataCpuVa + dataOffset, dataSize);
            }
        }

        // setting error field to zero, thus we have no errors
        *(int*)(request + 0x04) = 0;

        return 0;
    }
}
