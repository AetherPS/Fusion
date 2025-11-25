#include "stdafx.h"
#include "ProcessMonitor.h"

ProcessMonitor::ProcessMonitor(int pid)
{
	kinfo_proc info;
	if (GetProcInfo(pid, &info) != 0)
	{
		Logger::Error("Invalid PID(%d) passed to ProcessMonitor.", pid);
		return;
	}

	ShouldRun = true;
	ProcessId = pid;
	ProcessName = info.ki_comm;

	ThreadPool::QueueJob([=] { WatchThread(); });
}

ProcessMonitor::~ProcessMonitor()
{
	ShouldRun = false;
}

void ProcessMonitor::WatchThread()
{
	while (ShouldRun)
	{
		std::vector<kinfo_proc> procList;
		GetProcessList(procList);

		if (std::find_if(procList.begin(), procList.end(), [=](const kinfo_proc& arg) { return arg.ki_pid == ProcessId; }) == procList.end())
		{
			if (OnExit != nullptr)
				OnExit();

			ProcessId = -1;
			while (ProcessId == -1)
			{
				ProcessId = GetPidByName(ProcessName.c_str());
				sceKernelSleep(1);
			}

			if (OnRespawn != nullptr)
				OnRespawn();
		}

		int status;
		auto debuggeePid = wait4(ProcessId, &status, WNOHANG, nullptr);
		if (debuggeePid == ProcessId)
		{
			if (OnException != nullptr)
				OnException(status);
		}

		sceKernelSleep(1);
	}
}