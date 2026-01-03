#pragma once

class PluginExtension 
{
public:
    static void Init();
    static void Term();

private:
    static constexpr const char* INI_PATH = "/data/Fusion/PluginList.ini";
    static std::atomic<bool> IsRunning;
    static std::atomic<int> LastPid;
    
    static void Monitor();
    static void LoadPlugins(int pid);
    static bool IsProcessReady(int pid);
};