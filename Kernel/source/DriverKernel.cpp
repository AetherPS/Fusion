#include "Common.h"
#include "DriverKernel.h"

int DriverKernel::OnIoctl(cdev* dev, unsigned long cmd, caddr_t data, int fflag, thread* td)
{
    switch (cmd)
    {

    default:
        kprintf("Not Implemented. :(");
        break;
    }
}