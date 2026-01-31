#include "common.h"
#include "process.h"

int GetProcessList(std::vector<kinfo_proc>& ProcessList)
{
    size_t length;
    static int name[] = { CTL_KERN, KERN_PROC, KERN_PROC_PROC, 0 };

    // Get the size of buffer needed.
    if (sysctl(name, 3, nullptr, &length, nullptr, 0) < 0)
        return -1;

    // Resize our vector to accommodate.
    try
    {
        ProcessList.resize(length / sizeof(kinfo_proc));
    }
    catch (const std::bad_alloc&)
    {
        return -1;
    }

    // Retrive the processes.
    if (sysctl(name, 3, ProcessList.data(), &length, nullptr, 0) < 0)
        return -1;

    // Fix names.
    for (auto& proc : ProcessList)
    {
        sceKernelGetProcessName(proc.ki_pid, (char*)proc.ki_comm);
    }

    return 0;
}

int GetPidByName(const char* ProcessName)
{
    // Get the list of running processes.
    std::vector<kinfo_proc> processList;
    GetProcessList(processList);

    // Get the pid of the process.
    for (const auto& i : processList)
    {
        if (strstr(i.ki_comm, ProcessName))
            return i.ki_pid;
    }

    return -1;
}
