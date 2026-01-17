#include "Common.h"
#include "TTYRedirector.h"

// Credits: https://github.com/OpenOrbis/mira-project

void* TTYRedirector::DeciTTYWriteOriginal = 0;

void TTYRedirector::Init()
{
    cpu_disable_wp();
    DeciTTYWriteOriginal = *(void**)((void*)g_KernelAddrs.DeciTTYWriteHook);
    *(void**)((void*)g_KernelAddrs.DeciTTYWriteHook) = reinterpret_cast<void*>(OnDeciTTYWrite);
    cpu_enable_wp();
}

void TTYRedirector::Term()
{

}

int TTYRedirector::OnDeciTTYWrite(struct cdev* dev, struct uio* uio, int ioflag)
{
    auto cloneuio = (struct uio* (*)(struct uio* uiop))((void*)g_KernelAddrs.cloneuio);
    auto console_write = (int(*)(struct cdev* dev, struct uio* uio, int ioflag))((void*)g_KernelAddrs.console_write);
    auto deci_tty_write = (int(*)(struct cdev* dev, struct uio* uio, int ioflag))((void*)g_KernelAddrs.deci_tty_write);
    auto M_IOV = (struct malloc_type*)((void*)g_KernelAddrs.M_IOV);
    auto console_cdev = (struct cdev**)((void*)g_KernelAddrs.console_cdev);

    if (FusionSysctl::fusion_feature_tty_redirect)
    {
        struct uio* cloned_uio = NULL;
        int ret;

        cloned_uio = cloneuio(uio);

        ret = deci_tty_write(dev, uio, ioflag);

        if (cloned_uio)
        {
            if (*console_cdev)
                console_write(*console_cdev, cloned_uio, ioflag);
            free(cloned_uio, M_IOV);
        }

        return ret;
    }
    else
    {
        return deci_tty_write(dev, uio, ioflag);
    }
}