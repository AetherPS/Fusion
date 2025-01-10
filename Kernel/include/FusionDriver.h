#pragma once

class FusionDriver
{
public:
	static void Init();
	static void Term();

	static int OnIoctl(cdev* p_Device, unsigned long p_Command, caddr_t p_Data, int p_FFlag, thread* p_Thread);

private:
	static cdev* m_Device;
	static cdevsw m_DeviceSw;

	static int GetKDriverInfo(caddr_t data);
};