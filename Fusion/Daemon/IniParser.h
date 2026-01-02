#pragma once

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

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
                sections_[currentSection].push_back(line);
            }
        }
        return true;
    }

    const std::vector<std::string>* GetSection(const std::string& section) const 
    {
        auto it = sections_.find(section);
        return it != sections_.end() ? &it->second : nullptr;
    }

    bool HasSection(const std::string& section) const 
    {
        return sections_.find(section) != sections_.end();
    }

    const auto& sections() const { return sections_; }

private:
    std::unordered_map<std::string, std::vector<std::string>> sections_;

    static std::string trim(const std::string& s) {
        auto start = s.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) return "";
        auto end = s.find_last_not_of(" \t\r\n");
        return s.substr(start, end - start + 1);
    }
};