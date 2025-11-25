#pragma once

class SystemPatcher
{
public:
	static void Init();

private:
	static void ShellCore();
	static void ShellUI();

	static inline ProcessMonitor* ShellUIMonitor;
};
