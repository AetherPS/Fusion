#include "stdafx.h"
#include "Process.h"

int GetProcessList(std::vector<kinfo_proc>& ProcessList)
{
	size_t length;
	static int name[] = { CTL_KERN, KERN_PROC, KERN_PROC_PROC, 0 };

	// Get the size of buffer needed.
	if (sysctl(name, 3, nullptr, &length, nullptr, 0) < 0) // TODO: OOSDK PR
		return -1;

	// Resize our vector to accommodate.
	try
	{
		ProcessList.resize(length / sizeof(kinfo_proc));
	}
	catch (const std::bad_alloc&)
	{
		return -1;
	}

	// Retrive the processes.
	if (sysctl(name, 3, ProcessList.data(), &length, nullptr, 0) < 0)
		return -1;

	// Fix names.
	for (auto& proc : ProcessList)
	{
		sceKernelGetProcessName(proc.ki_pid, (char*)proc.ki_comm);
	}

	return 0;
}

int GetProcInfo(int pid, kinfo_proc* out)
{
	size_t length = sizeof(kinfo_proc);
	int name[] = { CTL_KERN, KERN_PROC, KERN_PROC_PID, pid };

	// Retrive the processes.
	if (sysctl(name, 4, out, &length, nullptr, 0) < 0)
		return -1;

	// Fix name.
	sceKernelGetProcessName(out->ki_pid, (char*)out->ki_comm); // TODO: PR on OOSDK

	return 0;
}

int GetPidByName(const char* ProcessName)
{
	// Get the list of running processes.
	std::vector<kinfo_proc> processList;
	GetProcessList(processList);

	// Get the pid of the process.
	for (const auto& i : processList)
	{
		if (strstr(i.ki_comm, ProcessName))
			return i.ki_pid;
	}

	return -1;
}

int GetPidByAppId(int appId)
{
	int pid = 0;

	// Get the list of running processes.
	std::vector<kinfo_proc> processList;
	GetProcessList(processList);

	for (const auto& i : processList)
	{
		// Get the app info using the pid.
		OrbisAppInfo appInfo;
		sceKernelGetAppInfo(i.ki_pid, &appInfo);

		// Using the titleId match our desired app and return the pid from the kinfo_proc.
		if (appInfo.AppId == appId)
		{
			pid = i.ki_pid;

			break;
		}
	}

	return pid;
}
