#pragma once

#include "Config.h"
#include <vector>
#include <string>

#define FUSION_DAEMON_DIR "/data/Fusion/Daemons"
#define SYSTEM_APP_DIR "/system/vsh/app"
#define FUSION_BACKUP_DIR "/mnt/fusion"

class DaemonExtension
{
public:
	DaemonExtension() = default;
	explicit DaemonExtension(Config& config);

	// Initialize the overlay system (call once at startup)
	bool SetupOverlay();

	// Mount a daemon from FUSION_DAEMON_DIR to SYSTEM_APP_DIR
	int MountDaemon(const char* titleId);

	// Mount and launch a daemon
	int MountAndLaunchDaemon(const char* titleId);

	// Load and launch all daemons from config
	int LoadDaemonsFromConfig(Config& config);

	// Add a daemon to config (doesn't mount/launch)
	static void AddDaemonToConfig(Config& config, const std::string& titleId);

	// Remove a daemon from config
	static void RemoveDaemonFromConfig(Config& config, const std::string& titleId);

	// Create default daemon config entries
	static void CreateDefaultConfig(Config& config);

	// Check if overlay is setup
	bool IsOverlayReady() const { return overlayReady; }

	// Get list of loaded daemons
	const std::vector<std::string>& GetLoadedDaemons() const { return loadedDaemons; }

private:
	bool overlayReady = false;
	std::vector<std::string> loadedDaemons;
};