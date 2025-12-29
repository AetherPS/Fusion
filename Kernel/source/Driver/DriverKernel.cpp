#include "Common.h"
#include "DriverKernel.h"
#include "DriverProc.h"

int DriverKernel::OnIoctl(cdev* dev, unsigned long cmd, caddr_t data, int fflag, thread* td)
{
    switch (cmd)
    {
    case CMD_KERN_GET_BASE:
		return KernelBase(data);

	case CMD_KERN_READ_WRITE_MEMORY:
		return KernelReadWrite(data);

	case CMD_KERN_ICC_NVS_READ_WRITE:
		return KernelIccNvsReadWrite(data);
    default:
        kprintf("Not Implemented. :(");
        return ENOSYS;
    }
}

int DriverKernel::KernelBase(caddr_t data)
{
    Input_KernelBase* input = (Input_KernelBase*)data;
    input->KernelBase = (uint64_t)KernelBase;
	return 0;
}

int DriverKernel::KernelReadWrite(caddr_t data)
{
    Input_ReadWriteMemory input;
    thread* td = nullptr;
    proc* p = nullptr;

    int res = FusionDriver::GetProcessThreadInput(data, &input, &td, &p);
    if (res != 0)
        return res;

    mtx_unlock_flags(&p->p_lock, 0);

    cpu_disable_wp();

    if (input.IsWrite)
    {
        copyin_nofault((void*)input.DataAddress, (void*)input.ProcessAddress, input.Length);
    }
    else
    {
        copyout_nofault((void*)input.ProcessAddress, (void*)input.DataAddress, input.Length);
	}

    cpu_enable_wp();

    return 0;
}

int DriverKernel::KernelIccNvsReadWrite(caddr_t data)
{
    Input_IccNvsReadWrite input;
    copyin(data, &input, sizeof(Input_IccNvsReadWrite));

    if (input.IsWrite)
    {
		icc_nvs_write(input.Block, input.Offset, input.Size, input.Value);
    }
    else
    {
		icc_nvs_read(input.Block, input.Offset, input.Size, input.Value);
    }

	return 0;
}