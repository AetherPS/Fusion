#include "Common.h"
#include "TTYRedirector.h"

void* TTYRedirector::DeciTTYWriteOriginal = 0;

void TTYRedirector::Init()
{
    cpu_disable_wp();
    DeciTTYWriteOriginal = *(void**)(KernelBase + 0x01A7ED98);
    *(void**)(KernelBase + 0x01A7ED98) = reinterpret_cast<void*>(OnDeciTTYWrite);
    cpu_enable_wp();
}

void TTYRedirector::Term()
{

}

int TTYRedirector::OnDeciTTYWrite(struct cdev* dev, struct uio* uio, int ioflag)
{
    struct uio* cloned_uio = NULL;
    int ret;

    auto cloneuio = (struct uio* (*)(struct uio* uiop))(KernelBase + 0x44E7E0);
    auto console_write = (int(*)(struct cdev* dev, struct uio* uio, int ioflag))(KernelBase + 0x2D6EB0);
    auto deci_tty_write = (int(*)(struct cdev* dev, struct uio* uio, int ioflag))(KernelBase + 0x48CDE0);
    auto M_IOV = (struct malloc_type*)(KernelBase + 0x1A792C0);
    auto console_cdev = (struct cdev**)(KernelBase + 0x21F1128);

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