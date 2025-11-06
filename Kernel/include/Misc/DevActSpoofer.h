#pragma once

class DevActSpoofer
{
public:
	static void Init();
	static void Term();

private:
	static Detour* DevActOnIoctlDetour;
	static int DevActOnIoctlHook(cdev* dev, unsigned long cmd, caddr_t data, int fflag, thread* td);
};
