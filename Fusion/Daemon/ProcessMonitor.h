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
	std::atomic<bool> ShouldRun;
	std::atomic<int> ProcessId;
	std::string ProcessName;

	void WatchThread();
};