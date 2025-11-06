#include "Common.h"
#include "FusionDriver.h"
#include "DriverKernel.h"
#include "DriverProc.h"

cdev* FusionDriver::m_Device;
cdevsw FusionDriver::m_DeviceSw;
Detour* FusionDriver::getnewvnodeDetour;

#define DRIVER_NAME "Fusion"

void FusionDriver::Init()
{
	m_DeviceSw.d_version = D_VERSION;
	m_DeviceSw.d_name = DRIVER_NAME;
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

	Detour::Detour32(&getnewvnodeDetour, KernelBase + addr_getnewvnode, (uint8_t*)&getnewvnodeHook);
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

int FusionDriver::getnewvnodeHook(const char* tag, mount* mp, void* vops, vnode** vpp)
{
	auto res = getnewvnodeDetour->Invoke<int>(tag, mp, vops, vpp);

	if (strstr(tag, "devfs") == nullptr)
		return res;

	struct devfs_rule ruleset;
	memset(&ruleset, 0, sizeof(struct devfs_rule));
	ruleset.dr_id = 0;
	ruleset.dr_magic = DEVFS_MAGIC;

	strcpy(ruleset.dr_pathptrn, DRIVER_NAME);
	ruleset.dr_icond |= DRC_PATHPTRN;
	ruleset.dr_iacts |= DRA_BACTS;
	ruleset.dr_bacts |= DRB_UNHIDE;

	auto dm = (devfs_mount*)mp->mnt_data;
	auto dk = (devfs_krule*)malloc(sizeof(devfs_krule), M_TEMP, 258);
	memcpy(&dk->dk_rule, &ruleset, sizeof(struct devfs_rule));
	devfs_rule_applyde_recursive(dk, dm->dm_rootdir);
	free(dk, M_TEMP);

	return res;
}