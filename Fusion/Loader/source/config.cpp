#include "common.h"
#include "config.h"

#include "IniParser.h"
#include <iostream>

int EnsureDefaultConfig()
{
	// Check if config file already exists
	if (FileExist(CONFIG_PATH))
	{
		return 0; // Already exists, nothing to do
	}

	klog("Creating default configuration file...\n");

	// Calculate the size of the embedded Settings.ini
	size_t configSize = _binary_resources_Settings_ini_end - _binary_resources_Settings_ini_start;

	if (WriteFile(CONFIG_PATH, _binary_resources_Settings_ini_start, configSize) != 0)
	{
		klog("Failed to write config file\n");
		return -1;
	}

	klog("Default configuration created successfully\n");
	return 0;
}

void ApplyFeatureConfiguration()
{
	// Ensure default configuration file exists
	if (EnsureDefaultConfig() != 0)
	{
		klog("Warning: Failed to create default configuration file\n");
	}

	// Load configuration file
	IniParser config;
	if (!config.Load("/data/Fusion/Settings.ini"))
	{
		klog("Failed to load configuration file - using defaults\n");
	}

	struct FeatureSetting {
		const char* sysctlName;
		const char* iniKey;
		bool defaultValue;
	};

	FeatureSetting features[] = {
		// Major features
		{"Fusion.FeatureFlag.Homebrew", "Homebrew", true},
		{"Fusion.FeatureFlag.Driver", "Driver", true},
		{"Fusion.FeatureFlag.Fuse", "Fuse", true},
		{"Fusion.FeatureFlag.TrapHooks", "TrapHooks", true},
		// Additional features
		{"Fusion.FeatureFlag.Jit", "Jit", false},
		{"Fusion.FeatureFlag.ExtendedLogging", "ExtendedLogging", true},
		{"Fusion.FeatureFlag.Bootstrapper", "Bootstrapper", true},
		{"Fusion.FeatureFlag.DirectMemory", "DirectMemory", true},
		{"Fusion.FeatureFlag.DevAct", "DevActSpoofer", true},
		{"Fusion.FeatureFlag.Dipsw", "DipSwitchSpoofer", true},
		{"Fusion.FeatureFlag.LibraryReplace", "LibraryReplacer", true},
		{"Fusion.FeatureFlag.TTYRedirect", "TTYRedirect", false}
	};

	for (const auto& feature : features)
	{
		bool enabled = config.GetBool("Features", feature.iniKey, feature.defaultValue);
		int value = enabled ? 1 : 0;
		size_t len = sizeof(value);

		if (sysctlbyname(feature.sysctlName, NULL, 0, &value, len) != 0)
		{
			klog("Failed to enable: %s\n", feature.sysctlName);
		}
	}
}