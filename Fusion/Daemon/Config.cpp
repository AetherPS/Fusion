#include "stdafx.h"
#include "Config.h"

Config::Config(const std::string& configPath)
{
    Load(configPath);
}

bool Config::Load(const std::string& configPath)
{
    try
    {
        // Check if file exists
        if (!FileSystem::Exists(configPath))
        {
            klog("Config file does not exist: %s", configPath.c_str());
            klog("Creating default config...");

            // Create default config
            jsonConfig = json::object();
            jsonConfig["EnableFTP"] = false;
            jsonConfig["StartDECI"] = true;

            // Save default config
            if (!Save(configPath))
            {
                klog("Failed to save default config");
                return false;
            }

            lastLoadPath = configPath;
            klog("Default config created and saved to: %s", configPath.c_str());
            return true;
        }

        // Read file content using FileSystem
        size_t fileSize = FileSystem::GetSize(configPath);
        if (fileSize == 0)
        {
            klog("Config file is empty: %s", configPath.c_str());
            return false;
        }

        std::string buffer(fileSize, '\0');
        int res = FileSystem::Read(configPath, &buffer[0], fileSize);

        if (res != 0)
        {
            klog("Failed to read config file: %s", configPath.c_str());
            return false;
        }

        // Parse JSON
        jsonConfig = json::parse(buffer);
        lastLoadPath = configPath;
        klog("Config loaded successfully from: %s", configPath.c_str());
        return true;
    }
    catch (const nlohmann::json::parse_error& e)
    {
        klog("JSON parse error while loading config: %s", e.what());
        return false;
    }
    catch (const std::exception& e)
    {
        klog("Unexpected error loading config: %s", e.what());
        return false;
    }
}

bool Config::Save(const std::string& configPath) const
{
    try
    {
        // Serialize JSON to string with pretty printing
        std::string jsonContent = jsonConfig.dump(4);

        // Write to file using FileSystem
        int res = FileSystem::Write(configPath, jsonContent.c_str(), jsonContent.size());

        if (res != 0)
        {
            klog("Failed to write config file: %s", configPath.c_str());
            return false;
        }

        klog("Config saved successfully to: %s", configPath.c_str());
        return true;
    }
    catch (const std::exception& e)
    {
        klog("Failed to save config: %s", e.what());
        return false;
    }
}

json* Config::GetJsonValue(const std::string& name)
{
    // Handle simple keys or nested keys like "database.host"
    size_t pos = 0;
    json* current = &jsonConfig;

    while (pos < name.length())
    {
        size_t nextPos = name.find('.', pos);
        if (nextPos == std::string::npos)
            nextPos = name.length();

        std::string key = name.substr(pos, nextPos - pos);

        if (!current->is_object())
        {
            klog("Cannot traverse config path '%s': expected object at '%s'",
                name.c_str(), key.c_str());
            return nullptr;
        }

        if (current->find(key) == current->end())
        {
            klog("Config key '%s' not found in path '%s'", key.c_str(), name.c_str());
            return nullptr;
        }

        current = &(*current)[key];
        pos = nextPos + 1;
    }

    return current;
}

const json* Config::GetJsonValue(const std::string& name) const
{
    // Handle simple keys or nested keys like "database.host"
    size_t pos = 0;
    const json* current = &jsonConfig;

    while (pos < name.length())
    {
        size_t nextPos = name.find('.', pos);
        if (nextPos == std::string::npos)
            nextPos = name.length();

        std::string key = name.substr(pos, nextPos - pos);

        if (!current->is_object())
        {
            klog("Cannot traverse config path '%s': expected object at '%s'",
                name.c_str(), key.c_str());
            return nullptr;
        }

        auto it = current->find(key);
        if (it == current->end())
        {
            klog("Config key '%s' not found in path '%s'", key.c_str(), name.c_str());
            return nullptr;
        }

        current = &(*it);
        pos = nextPos + 1;
    }

    return current;
}

bool Config::Has(const std::string& name) const
{
    return GetJsonValue(name) != nullptr;
}
