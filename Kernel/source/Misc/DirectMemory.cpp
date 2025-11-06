#include "Common.h"
#include "DirectMemory.h"
#include "Misc.h"

Detour* DirectMemory::OnDirectMemoryIoctlDetour;

int DirectMemory::OnDirectMemoryIoctl(cdev* device, unsigned long cmd, caddr_t data, int fflag, thread* td)
{
    auto res = OnDirectMemoryIoctlDetour->Invoke<int>(device, cmd, data, fflag, td);

    // Skip ghosts for now since it is broken.
    if (strstr(td->td_proc->titleId, "CUSA00018"))
        return res;

    switch (cmd)
    {
    case 0x4008800A:
    {
        size_t allocationSpace = 50 * 1024 * 1024;

        *(uint64_t*)data -= allocationSpace;

        uint64_t currentSize = *(uint64_t*)data;

#ifdef DEBUG
        kprintf("[Direct Memory Init] %lu MB\n", currentSize / (1024 * 1024));
        kprintf(" - Max: %lu MB\n", (currentSize + allocationSpace) / (1024 * 1024));
        kprintf(" - Reserved: %lu MB\n", allocationSpace / (1024 * 1024));
#endif

    }
    break;
    default:
        break;
    }

    return res;
}

void DirectMemory::Init()
{
    Detour::Detour64(&OnDirectMemoryIoctlDetour, KernelBase + addr_DirectMemoryHook, (void*)OnDirectMemoryIoctl);
}

void DirectMemory::Term()
{
    delete OnDirectMemoryIoctlDetour;
}