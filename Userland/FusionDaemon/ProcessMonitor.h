#pragma once

class ProcessMonitor
{
public:
	ProcessMonitor(int pid);
	~ProcessMonitor();

	std::function<void()> OnRespawn;
	std::function<void()> OnExit;
	std::function<void(int)> OnException;
private:
	bool ShouldRun;
	int ProcessId;
	std::string ProcessName;

	void WatchThread();
};