#pragma once

class DirectMemory
{
public:
	static void Init();
	static void Term();

private:
	static Detour64* OnDirectMemoryIoctlDetour;
	static int OnDirectMemoryIoctl(cdev* device, unsigned long cmd, caddr_t data, int fflag, thread* td);
};

