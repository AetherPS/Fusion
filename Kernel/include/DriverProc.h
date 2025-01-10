#pragma once

class DriverProc
{
private:
    static int GetProccessInfo(caddr_t data, thread* td);
    static int GetProccessModuleList(caddr_t data, thread* td);
    static int ProcessReadWrite(caddr_t data, thread* td);
    static int ProcessAlloc(caddr_t data, thread* td);
    static int ProcessFree(caddr_t data);
    static int StartThread(caddr_t data);
    static int Resolve(caddr_t data);
    static int Jailbreak(caddr_t data, thread* td);
    static int RestoreJail(caddr_t data, thread* td);
    static int SandBoxPath(caddr_t data, thread* td);

public:
    static int OnIoctl(cdev* dev, unsigned long cmd, caddr_t data, int fflag, thread* td);
};
