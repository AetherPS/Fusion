#include "common.h"
#include "plugins.h"

int InstallPlugins()
{

	return 0;
}

int LoadPlugins()
{
	int res = LoadSprx("SceShellCore", "/data/Fusion/Plugins/ShellCore.sprx");
	if (res <= 0)
	{
		klog("[Fusion] Failed to load 'SceShellCore' plugin %llX.\n", res);
		return res;
	}

	res = LoadSprx("SceSysCore", "/data/Fusion/Plugins/SysCore.sprx");
	if (res <= 0)
	{
		klog("[Fusion] Failed to load 'SceSysCore' plugin %llX.\n", res);
		return res;
	}

	// TODO: AOT verison.
	//res = LoadSprx("SceShellUI", "/user/data/Fusion/Plugins/ShellUI.sprx");
	//if (res <= 0)
	//{
	//	klog("[Fusion] Failed to load 'SceShellUI' plugin %llX.\n", res);
	//	return res;
	//}

	return 0;
}