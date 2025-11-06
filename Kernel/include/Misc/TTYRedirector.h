#pragma once

class TTYRedirector
{
public:
	static void Init();
	static void Term();

private:
	static void* DeciTTYWriteOriginal;
	static int OnDeciTTYWrite(struct cdev* dev, struct uio* uio, int ioflag);
};
