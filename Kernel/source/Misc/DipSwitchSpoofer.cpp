#include "Common.h"
#include "DipSwitchSpoofer.h"

Detour64* DipSwitchSpoofer::DipSwitchOnIoctlDetour;
Detour32* DipSwitchSpoofer::sceKernelCheckDipswDetour;

int DipSwitchSpoofer::DipSwitchOnIoctlHook(cdev* dev, unsigned long cmd, caddr_t data, int fflag, thread* td)
{
	switch (cmd)
	{
	case 0x20008800:
		// Initialize Dipsw
		break;

	case 0x80028801:
		// Set Dipsw
		break;

	case 0x80028802:
		// UnSet Dipsw
		break;

	case 0xC0088803:	// Check Dipsw
		switch (*(uint16_t*)data)
		{
		case 0:			// IsDevelopmentMode
		case 1:			// Don't use DEVLAN
		case 95:		// UNK - CP box
		case 102:		// Disable DEV USB?
			*(int*)(data + 4) = 1;
			return 0;

		case 2:			// IsAssistMode
			*(int*)(data + 4) = 0;
			return 0;

		default:
			break;
		}
		break;

	case 0x80108804:
		// Read Dipsw Data
		break;

	case 0x80108805:
		// Write Dipsw Data
		break;

	case 0x8010880A:
		// Unknown
		break;
	}

	return DipSwitchOnIoctlDetour->Invoke<int>(dev, cmd, data, fflag, td);
}

int DipSwitchSpoofer::sceKernelCheckDipswHook(unsigned int dipswitch)
{
	switch (dipswitch)
	{
	case 0:			// IsDevelopmentMode
	case 95:		// UNK - CP box
	case 102:		// Disable DEV USB?
		return 1;

	case 1:			// Don't use DEVLAN
	case 2:			// IsAssistMode

		return 0;

	default:
		return sceKernelCheckDipswDetour->Invoke<int>(dipswitch);
	}
}

void DipSwitchSpoofer::Init()
{
	sceKernelCheckDipswDetour = new Detour32(&sceKernelCheckDipswDetour, KernelBase + addr_sceKernelCheckDipsw_Hook, sceKernelCheckDipswHook);
	//DipSwitchOnIoctlDetour = new Detour64(KernelBase + addr_dipsw_onioctl_hook, DipSwitchOnIoctlHook);
}

void DipSwitchSpoofer::Term()
{
	delete sceKernelCheckDipswDetour;
	//delete DipSwitchOnIoctlDetour;
}