#include "Common.h"
#include "Watcher.h"

eventhandler_entry* Watcher::SystemSuspendEvent;
eventhandler_entry* Watcher::SystemResumeEvent;
eventhandler_entry* Watcher::ProcessStartEvent;
eventhandler_entry* Watcher::ProcessStartEndEvent;
eventhandler_entry* Watcher::ProcessExitEvent;

void Watcher::Init()
{
    SystemSuspendEvent = EVENTHANDLER_REGISTER(system_suspend_phase1, (void*)OnSystemSuspend, nullptr, EVENTHANDLER_PRI_FIRST);
    SystemResumeEvent = EVENTHANDLER_REGISTER(system_resume_phase1, (void*)OnSystemResume, nullptr, EVENTHANDLER_PRI_FIRST);
    ProcessStartEvent = EVENTHANDLER_REGISTER(process_exec, (void*)OnProcessStart, nullptr, EVENTHANDLER_PRI_ANY);
    ProcessStartEndEvent = EVENTHANDLER_REGISTER(process_exec_end, (void*)OnProcessStartEnd, nullptr, EVENTHANDLER_PRI_ANY);
    ProcessExitEvent = EVENTHANDLER_REGISTER(process_exit, (void*)OnProcessExit, nullptr, EVENTHANDLER_PRI_ANY);
}

void Watcher::Term()
{
    EVENTHANDLER_DEREGISTER(system_suspend_phase1, SystemSuspendEvent);
    EVENTHANDLER_DEREGISTER(system_resume_phase1, SystemResumeEvent);
    EVENTHANDLER_DEREGISTER(process_exec, ProcessStartEvent);
    EVENTHANDLER_DEREGISTER(process_exec_end, ProcessStartEndEvent);
    EVENTHANDLER_DEREGISTER(process_exit, ProcessExitEvent);
}

void Watcher::OnSystemSuspend(void* arg)
{
    kprintf("System is Suspending...\n");

}

void Watcher::OnSystemResume(void* arg)
{
    kprintf("System is Resuming...\n");

}

void Watcher::OnProcessStart(void* arg, struct proc* p)
{

}

void Watcher::OnProcessStartEnd(void* arg, struct proc* p)
{
    if (!p)
        return;

#ifdef FF_HomeBrew
    FakePkgs::OnProcessStart(arg, p);
#endif
}

void Watcher::OnProcessExit(void* arg, struct proc* p)
{

}