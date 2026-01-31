#include "common.h"
#include "kernel.h"
#include "plugins.h"

#include "IniParser.h"
#include <iostream>

void PrintIniContents(const IniParser& config)
{
	klog("=== INI File Contents ===\n\n");
    for (const auto& [section, keys] : config.sections())
    {
		klog("[%s]\n", section.c_str());
        for (const auto& [key, value] : keys)
        {
			klog("  %s = %s\n", key.c_str(), value.c_str());
        }
		klog("\n");
    }
}

int main(void)
{
	ascii();

	/*if (FileExist("/data/Fusion/Settings.ini"))
	{
		klog("SettingsExists\n");
	}

	IniParser config;
	if (!config.Load("/data/Fusion/Settings.ini"))
	{
		std::cerr << "Failed to load config.ini\n";
		return 1;
	}
	klog("Config loaded successfully\n");
	klog("Number of sections: %lld\n", config.sections().size());
	PrintIniContents(config);
	klog("Finished printing\n");*/
	
	// Decompress and load the kernel module.
	if (LoadKernel() != 0)
	{
		klog("Failed to load the kernel module.\n");
		return 0;
	}

	if (InstallPlugins() != 0)
	{
		klog("Failed to install the plugins.\n");
		return 0;
	}

	if (LoadPlugins() != 0)
	{
		klog("Failed to load the plugins.\n");
		return 0;
	}

	return 0;
}