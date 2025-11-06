#include "Common.h"
#include "DevActSpoofer.h"

Detour64* DevActSpoofer::DevActOnIoctlDetour;

uint8_t actId[] = { 0x13, 0x37, 0x13, 0x37, 0x13, 0x37, 0x13, 0x37, 0x13, 0x37, 0x13, 0x37, 0x13, 0x37, 0x13, 0x37, 0x69, 0x69, 0x69, 0x69 };

int DevActSpoofer::DevActOnIoctlHook(cdev* dev, unsigned long cmd, caddr_t data, int fflag, thread* td)
{
	switch (cmd)
	{
	case 0x40105303:
	case 0x40105307:

		*(uint64_t*)(data) = 2969591295;
		*(int*)(data + 0x8) = 1;
		*(int*)(data + 0xC) = 0;

		return 0;

	case 0xC00C530D:
		*(int*)(data + 0x0) = 3;
		*(int*)(data + 0x4) = 0;
		*(int*)(data + 0x8) = 0;

		return 0;

	case 0x40185304:
		memcpy(data, actId, sizeof(actId));
		*(int*)(data + 0x14) = 0;

		return 0;

	default:
#ifdef DEBUG
		kprintf("devact_ioctl: %llX\n", cmd);
#endif
		return DevActOnIoctlDetour->Invoke<int>(dev, cmd, data, fflag, td);
	}
}

void DevActSpoofer::Init()
{
	DevActOnIoctlDetour = new Detour64(KernelBase + addr_devact_onioctl_hook, DevActOnIoctlHook);
}

void DevActSpoofer::Term()
{
	delete DevActOnIoctlDetour;
}