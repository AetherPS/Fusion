#pragma once

class DriverKernel
{
public:
	static int OnIoctl(cdev* dev, unsigned long cmd, caddr_t data, int fflag, thread* td);

private:
	static int KernelBase(caddr_t data);
	static int KernelReadWrite(caddr_t data);
	static int KernelIccNvsReadWrite(caddr_t data);
};
