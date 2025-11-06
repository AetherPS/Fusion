#pragma once

class Watcher
{
public:
	static void Init();
	static void Term();

private:
	static eventhandler_entry* SystemSuspendEvent;
	static eventhandler_entry* SystemResumeEvent;
	static eventhandler_entry* ProcessStartEvent;
	static eventhandler_entry* ProcessStartEndEvent;
	static eventhandler_entry* ProcessExitEvent;

	static void OnSystemSuspend(void* arg);
	static void OnSystemResume(void* arg);
	static void OnProcessStart(void* arg, struct proc* p);
	static void OnProcessStartEnd(void* arg, struct proc* p);
	static void OnProcessExit(void* arg, struct proc* p);
};