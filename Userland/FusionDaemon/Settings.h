#pragma once

class Settings
{
public:
	static void Init();
	static void Read();

	static inline int DMReserveSize;
	static inline bool EnableFTP;
	static inline std::vector<std::string> PluginList;

private:
	static void Create();
};
