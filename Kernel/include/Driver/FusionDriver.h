#pragma once

class FusionDriver
{
public:
	static void Init();
	static void Term();

	static int OnIoctl(cdev* p_Device, unsigned long p_Command, caddr_t p_Data, int p_FFlag, thread* p_Thread);

    template<typename T>
    static int GetProcessThreadInput(caddr_t data, T* input, thread** td, proc** p)
    {
        if (!input || !td || !p)
            return EINVAL;

        // Copy input from userland
        int res = copyin(data, input, sizeof(T));
        if (res != 0)
        {
            printf("%s: copyin failed with error %d\n", __FUNCTION__, res);
            return res;
        }

        // Find process
        *p = pfind(input->ProcessId);
        if (*p == nullptr)
        {
            printf("%s: Failed to find Process with the pid %i\n", __FUNCTION__, input->ProcessId);
            return ESRCH;
        }

        // Find main thread
        *td = (*p)->p_threads.tqh_first;
        if (*td == nullptr)
        {
            printf("%s: Failed to find Thread for Process with the pid %i\n", __FUNCTION__, input->ProcessId);
            mtx_unlock_flags(&(*p)->p_lock, 0);
            return ESRCH;
        }

        return 0;
    }

private:
	static cdev* m_Device;
	static cdevsw m_DeviceSw;
	static Detour* getnewvnodeDetour;

	static int getnewvnodeHook(const char* tag, mount* mp, void* vops, vnode** vpp);
	static int GetKDriverInfo(caddr_t data);
};