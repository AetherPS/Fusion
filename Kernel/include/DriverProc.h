#pragma once

class DriverProc
{
private:
    static int Jailbreak(caddr_t data);
    static int RestoreJail(caddr_t data);
    static int GetProccessModuleList(caddr_t data);
    static int ProcessReadWrite(caddr_t data);
    static int ProcessAlloc(caddr_t data);
    static int ProcessFree(caddr_t data);
    static int StartThread(caddr_t data);
    static int Resolve(caddr_t data);
    static int GetAuthId(caddr_t data);
    static int SetAuthId(caddr_t data);

    template<typename T>
    static int GetProcessThreadInput(caddr_t data, T* input, thread** td, proc** p)
    {
        if (!input || !td || !p)
            return EINVAL;

        // Copy input from userland
        int res = copyin(data, input, sizeof(T));
        if (res != 0)
        {
            kprintf("%s: copyin failed with error %d\n", __FUNCTION__, res);
            return res;
        }

        // Find process
        *p = pfind(input->ProcessId);
        if (*p == nullptr)
        {
            kprintf("%s: Failed to find Process with the pid %i\n", __FUNCTION__, input->ProcessId);
            return ESRCH;
        }

        // Find main thread
        *td = (*p)->p_threads.tqh_first;
        if (*td == nullptr)
        {
            kprintf("%s: Failed to find Thread for Process with the pid %i\n", __FUNCTION__, input->ProcessId);
            return ESRCH;
        }

        return 0;
    }

public:
    static int OnIoctl(cdev* dev, unsigned long cmd, caddr_t data, int fflag, thread* td);
};
