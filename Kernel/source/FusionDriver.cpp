#include "Common.h"
#include "FusionDriver.h"
#include "DriverKernel.h"
#include "DriverProc.h"

cdev* FusionDriver::m_Device;
cdevsw FusionDriver::m_DeviceSw;

void FusionDriver::Init()
{
	m_DeviceSw.d_version = D_VERSION;
	m_DeviceSw.d_name = "Fusion";
	m_DeviceSw.d_ioctl = OnIoctl;

	int ret = make_dev_p(MAKEDEV_CHECKNAME | MAKEDEV_WAITOK,
		&m_Device,
		&m_DeviceSw,
		nullptr,
		UID_ROOT,
		GID_WHEEL,
		S_IRWXU | S_IRWXG | S_IRWXO,
		m_DeviceSw.d_name);

	if (ret == EEXIST)
		kprintf("could not create device driver, device driver already exists.");
	else if (ret != 0)
		kprintf("could not create device driver (%d).", ret);
}

void FusionDriver::Term()
{
	destroy_dev(m_Device);
}

int FusionDriver::OnIoctl(cdev* dev, unsigned long cmd, caddr_t data, int fflag, thread* td)
{
	// Preserves the lower 32 bits of 'cmd'
	cmd = cmd & 0xFFFFFFFF;

	// Parse the info about this ioctl call.
	auto group = IOCGROUP(cmd);
	auto baseCmd = IOCTL_NUM(cmd);

	switch (group)
	{
	case 'P':
		return DriverProc::OnIoctl(dev, baseCmd, data, fflag, td);
	
	case 'K':
		return DriverKernel::OnIoctl(dev, baseCmd, data, fflag, td);

	default:
		kprintf("[FusionDriver] Not Implemented. :(\n");
		return ENOSYS;
	}
}