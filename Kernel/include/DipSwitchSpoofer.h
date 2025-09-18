#pragma once

class DipSwitchSpoofer
{
public:
	static void Init();
	static void Term();

private:
	static Detour64* DipSwitchOnIoctlDetour;
	static int DipSwitchOnIoctlHook(cdev* dev, unsigned long cmd, caddr_t data, int fflag, thread* td);
};
