#include "Common.h"
#include "DipSwitchSpoofer.h"

Detour* DipSwitchSpoofer::sceKernelCheckDipswDetour;

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
	Detour::Detour32(&sceKernelCheckDipswDetour, KernelBase + addr_sceKernelCheckDipsw_Hook, (void*)sceKernelCheckDipswHook);
}

void DipSwitchSpoofer::Term()
{
	delete sceKernelCheckDipswDetour;
}