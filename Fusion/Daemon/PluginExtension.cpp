#include "stdafx.h"
#include "PluginExtension.h"

std::atomic<bool> PluginExtension::IsRunning{ false };
std::atomic<int> PluginExtension::LastPid{ -1 };

void PluginExtension::Init()
{
    IsRunning = true;
    LastPid = -1;
    ThreadPool::QueueJob([]()
    {
        Monitor();
    });
}

void PluginExtension::Term() 
{
    IsRunning = false;
}

void PluginExtension::Monitor()
{
    while (IsRunning)
    {
        int appId = sceLncUtilGetAppIdOfBigApp();
        if (appId > 0)
        {
            int currentPid = GetPidByAppId(appId);
            if (currentPid > 0 && currentPid != LastPid)
            {
                LoadPlugins(currentPid);
                LastPid = currentPid;
            }
        }
        else
        {
            LastPid = -1;  // Reset when no app running
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void PluginExtension::LoadPlugins(int pid)
{
	// Small delay to ensure the app is fully initialized
    std::this_thread::sleep_for(std::chrono::milliseconds(1800));

    // Get the app info for the title Id.
    SceAppInfo info{};
    if (sceKernelGetAppInfo(pid, &info) != 0) return;

	// Fetch the INI file.
    IniParser ini;
    if (!ini.Load(INI_PATH)) return;

	// Keep track of loaded plugin paths to avoid duplicates.
    std::set<std::string> loadedPaths;

    // Load plugins that apply to all titles.
    if (auto* defaults = ini.GetSection("default"))
    {
        for (const auto& path : *defaults)
        {
            if (loadedPaths.insert(path).second)
            {
                Fusion::LoadSprx(pid, path);
            }
        }
    }

    // Load title-specific plugins.
    std::string titleId(info.TitleId, strnlen(info.TitleId, 10));
    if (auto* titlePlugins = ini.GetSection(titleId))
    {
        for (const auto& path : *titlePlugins)
        {
            if (loadedPaths.insert(path).second)
            {
                Fusion::LoadSprx(pid, path);
            }
        }
    }
}