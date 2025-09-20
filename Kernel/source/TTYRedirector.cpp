#include "Common.h"
#include "TTYRedirector.h"

// Credits: https://github.com/OpenOrbis/mira-project

void* TTYRedirector::DeciTTYWriteOriginal = 0;

void TTYRedirector::Init()
{
    cpu_disable_wp();
    DeciTTYWriteOriginal = *(void**)(KernelBase + addr_DeciTTYWriteHook);
    *(void**)(KernelBase + addr_DeciTTYWriteHook) = reinterpret_cast<void*>(OnDeciTTYWrite);
    cpu_enable_wp();
}

void TTYRedirector::Term()
{

}

int TTYRedirector::OnDeciTTYWrite(struct cdev* dev, struct uio* uio, int ioflag)
{
    struct uio* cloned_uio = NULL;
    int ret;

    auto cloneuio = (struct uio* (*)(struct uio* uiop))(KernelBase + addr_cloneuio);
    auto console_write = (int(*)(struct cdev* dev, struct uio* uio, int ioflag))(KernelBase + addr_console_write);
    auto deci_tty_write = (int(*)(struct cdev* dev, struct uio* uio, int ioflag))(KernelBase + addr_deci_tty_write);
    auto M_IOV = (struct malloc_type*)(KernelBase + addr_M_IOV);
    auto console_cdev = (struct cdev**)(KernelBase + addr_console_cdev);

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