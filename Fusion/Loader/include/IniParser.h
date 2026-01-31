#pragma once

class IniParser
{
public:
    bool Load(const std::string& filename)
    {
        std::ifstream file(filename);
        if (!file.is_open()) return false;

        std::string line, currentSection;
        while (std::getline(file, line))
        {
            line = trim(line);
            if (line.empty() || line[0] == ';' || line[0] == '#') continue;

            if (line.front() == '[' && line.back() == ']')
            {
                currentSection = line.substr(1, line.size() - 2);
            }
            else
            {
                auto pos = line.find('=');
                if (pos != std::string::npos)
                {
                    std::string key = trim(line.substr(0, pos));
                    std::string value = trim(line.substr(pos + 1));

                    // Check if key already exists (for multi-value support)
                    auto& section = sections_[currentSection];
                    auto it = section.find(key);
                    if (it != section.end())
                    {
                        // Append to existing value with comma separator
                        it->second += "," + value;
                    }
                    else
                    {
                        section[key] = value;
                    }
                }
            }
        }
        return true;
    }

    std::string GetString(const std::string& section, const std::string& key,
        const std::string& defaultValue = "") const
    {
        auto secIt = sections_.find(section);
        if (secIt == sections_.end()) return defaultValue;

        auto keyIt = secIt->second.find(key);
        return keyIt != secIt->second.end() ? keyIt->second : defaultValue;
    }

    int GetInt(const std::string& section, const std::string& key,
        int defaultValue = 0) const
    {
        std::string value = GetString(section, key);
        if (value.empty()) return defaultValue;

        try
        {
            return std::stoi(value);
        }
        catch (...)
        {
            return defaultValue;
        }
    }

    bool GetBool(const std::string& section, const std::string& key,
        bool defaultValue = false) const
    {
        std::string value = GetString(section, key);
        if (value.empty()) return defaultValue;

        std::string lower = value;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

        if (lower == "true" || lower == "1" || lower == "yes" || lower == "on")
            return true;
        if (lower == "false" || lower == "0" || lower == "no" || lower == "off")
            return false;

        return defaultValue;
    }

    std::vector<std::string> GetList(const std::string& section, const std::string& key,
        const std::vector<std::string>& defaultValue = {}) const
    {
        std::string value = GetString(section, key);
        if (value.empty()) return defaultValue;

        std::vector<std::string> result;
        std::stringstream ss(value);
        std::string item;

        while (std::getline(ss, item, ','))
        {
            item = trim(item);
            if (!item.empty())
                result.push_back(item);
        }

        return result.empty() ? defaultValue : result;
    }

    bool HasSection(const std::string& section) const
    {
        return sections_.find(section) != sections_.end();
    }

    bool HasKey(const std::string& section, const std::string& key) const
    {
        auto secIt = sections_.find(section);
        if (secIt == sections_.end()) return false;
        return secIt->second.find(key) != secIt->second.end();
    }

    const auto& sections() const { return sections_; }

private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> sections_;

    static std::string trim(const std::string& s)
    {
        auto start = s.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) return "";
        auto end = s.find_last_not_of(" \t\r\n");
        return s.substr(start, end - start + 1);
    }
};