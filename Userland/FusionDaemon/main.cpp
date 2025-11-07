#include "stdafx.h"
#include <iostream>

// TODO: Add Daemon helper class that can Start and/or Install Daemons. We will want to add a list of Daemons to launch at startup to the config.

int main(int argc, char** arg)
{
	// Set up the Logger.
	Logger::Init(true, Logger::LoggingLevels::LogLevelAll);
	Logger::Info("Fusion Daemon started.");

	// Jailbreak our current process.
	if (!Jailbreak())
	{
		Notify("Failed to jailbreak Process...");
		ExitGraceful();
		return 0;
	}

	// Initialize and load required modules.
	if (!LoadModules())
	{
		Notify("Failed to Load Modules...");
		ExitGraceful();
		return 0;
	}

	// // Initialize Settings config.
	// Settings::Init();
	// 
	// if (Settings::EnableFTP)
	// {
	// 	// Start FTP Server.
	// 	Logger::Info("Starting FTP Server...");
	// 	Notify("Starting FTP Server...");
	// }

	ExitGraceful();
	return 0;
}