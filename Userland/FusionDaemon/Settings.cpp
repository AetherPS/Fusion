#include "stdafx.h"
#include "Settings.h"

#define SETTINGS_DIR "/data/Fusion/Settings.cfg"

using nlohmann::json;

void Settings::Init()
{
    if (!FileSystem::Exists(SETTINGS_DIR))
    {
        Logger::Info("Create Settings.");
        Create();
    }
    
    Read();
}

void Settings::Create()
{
    json j;
    j["EnableFTP"] = true;
    j["Plugins"] = json::array();
    j["DMReserveSize"] = 50; // Default value for new settings files.
    
    // Pretty-print with 4-space indent.
    std::string out = j.dump(4);
    
    // Write to disk using existing FileSystem::Write signature (pointer, size).
    FileSystem::Write(SETTINGS_DIR, (void*)out.c_str(), out.size());
}

void Settings::Read()
{
    std::vector<uint8_t> data;
    data.resize(FileSystem::GetSize(SETTINGS_DIR));
    FileSystem::Read(SETTINGS_DIR, data.data(), data.size());

    json j;
    try
    {
        j = json::parse(std::string(data.begin(), data.end()));
    }
    catch (const json::parse_error& e)
    {
        Logger::Info("Failed to parse settings JSON.");
        return;
    }

    if (j.contains("DMReserveSize") && j["DMReserveSize"].is_number_integer())
    {
        DMReserveSize = j["DMReserveSize"].get<int>();
    }
    else
    {
        DMReserveSize = 50;
    }

    if (j.contains("EnableFTP") && j["EnableFTP"].is_boolean())
    {
        EnableFTP = j["EnableFTP"].get<bool>();
    }
    else
    {
        EnableFTP = false;
    }

    PluginList.clear();
    if (j.contains("Plugins") && j["Plugins"].is_array())
    {
        for (const auto& plugin : j["Plugins"])
        {
            if (plugin.is_string())
            {
                PluginList.push_back(plugin.get<std::string>());
            }
        }
    }
}