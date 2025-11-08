#pragma once

class Settings
{
public:
	static void Init();

	static inline bool EnableFTP;
	static inline std::vector<std::string> PluginList;

private:
	static void Create();
};
