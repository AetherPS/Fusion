#pragma once

class DriverKernel
{
public:
	static int OnIoctl(cdev* dev, unsigned long cmd, caddr_t data, int fflag, thread* td);
};
