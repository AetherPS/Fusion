#include "common.h"
#include "plugins.h"

int DecompressFile(uint8_t* compressedData, const char* path)
{
	if (FileExist(path))
	{
		RemoveFile(path);
	}

	return DecompressToFile(compressedData, path);
}

int InstallPlugins()
{
	CreateDirectory(PLUGIN_PATH);

	int res = DecompressFile(_binary_resources_ShellCore_sprx_compressed_start, SHELLCORE_PATH);
	if (res != 0)
	{
		klog("[Fusion] Failed to decompress/write 'ShellCore.sprx' plugin. %llx", res);
		return res;
	}

	res = DecompressFile(_binary_resources_SysCore_sprx_compressed_start, SYSCORE_PATH);
	if (res != 0)
	{
		klog("[Fusion] Failed to decompress/write 'SysCore.sprx' plugin. %llx", res);
		return res;
	}

	res = DecompressFile(_binary_resources_ShellUI_sprx_compressed_start, SHELLUI_PATH);
	if (res != 0)
	{
		klog("[Fusion] Failed to decompress/write 'ShellUI.sprx' plugin. %llx", res);
		return res;
	}

	res = DecompressFile(_binary_resources_ShellUIManaged_dll_sprx_compressed_start, SHELLUIMANAGED_PATH);
	if (res != 0)
	{
		klog("[Fusion] Failed to decompress/write 'ShellUI.sprx' plugin. %llx", res);
		return res;
	}

	res = DecompressFile(_binary_resources_Bootstrapper_sprx_compressed_start, BOOTSTRAPPER_PATH);
	if (res != 0)
	{
		klog("[Fusion] Failed to decompress/write 'Bootstrapper.sprx' plugin. %llx", res);
		return res;
	}

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

	res = LoadSprx("SceShellUI", "/user/data/Fusion/Plugins/ShellUI.sprx");
	if (res <= 0)
	{
		klog("[Fusion] Failed to load 'SceShellUI' plugin %llX.\n", res);
		return res;
	}

	return 0;
}