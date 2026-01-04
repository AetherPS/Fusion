#pragma once

using json = nlohmann::json;

class Config
{
public:
    Config() = default;
    explicit Config(const std::string& configPath);

    // Load config from file
    bool Load(const std::string& configPath);

    // Save config to file
    bool Save(const std::string& configPath) const;

    // Get a value by name with template type
    template<typename T>
    std::optional<T> Get(const std::string& name) const;

    // Get a value with a default fallback
    template<typename T>
    T Get(const std::string& name, const T& defaultValue) const;

    // Set a value
    template<typename T>
    void Set(const std::string& name, const T& value);

    // Check if a key exists
    bool Has(const std::string& name) const;

    // Get raw JSON object
    const json& GetRawJson() const { return jsonConfig; }
    json& GetRawJson() { return jsonConfig; }

private:
    json jsonConfig;
    std::string lastLoadPath;

    json* GetJsonValue(const std::string& name);
    const json* GetJsonValue(const std::string& name) const;
};

// Template implementations
template<typename T>
std::optional<T> Config::Get(const std::string& name) const
{
    try
    {
        const auto* value = GetJsonValue(name);
        if (value == nullptr || value->is_null())
        {
            klog("Config key '%s' not found or is null", name.c_str());
            return std::nullopt;
        }

        return value->get<T>();
    }
    catch (const nlohmann::json::exception& e)
    {
        klog("Failed to retrieve config value '%s': %s", name.c_str(), e.what());
        return std::nullopt;
    }
}

template<typename T>
T Config::Get(const std::string& name, const T& defaultValue) const
{
    auto result = Get<T>(name);
    return result.has_value() ? result.value() : defaultValue;
}

template<typename T>
void Config::Set(const std::string& name, const T& value)
{
    try
    {
        auto* jsonValue = GetJsonValue(name);
        if (jsonValue != nullptr)
        {
            *jsonValue = value;
            klog("Config value '%s' set successfully", name.c_str());
        }
        else
        {
            klog("Failed to set config value '%s': path not found", name.c_str());
        }
    }
    catch (const nlohmann::json::exception& e)
    {
        klog("Failed to set config value '%s': %s", name.c_str(), e.what());
    }
}
