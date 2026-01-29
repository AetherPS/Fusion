#pragma once

#define PLUGIN_PATH "/data/Fusion/Plugins"
#define SHELLUI_PATH "/data/Fusion/Plugins/ShellUI.sprx"
#define SHELLUIMANAGED_PATH "/data/Fusion/Plugins/ShellUIManaged.dll.sprx"
#define SHELLCORE_PATH "/data/Fusion/Plugins/ShellCore.sprx"
#define SYSCORE_PATH "/data/Fusion/Plugins/SysCore.sprx"
#define BOOTSTRAPPER_PATH "/data/Fusion/Plugins/Bootstrapper.sprx"

extern uint8_t _binary_resources_ShellUI_sprx_compressed_start[];
extern uint8_t _binary_resources_ShellUI_sprx_compressed_end[];

extern uint8_t _binary_resources_ShellUIManaged_dll_sprx_compressed_start[];
extern uint8_t _binary_resources_ShellUIManaged_dll_sprx_compressed_end[];

extern uint8_t _binary_resources_ShellCore_sprx_compressed_start[];
extern uint8_t _binary_resources_ShellCore_sprx_compressed_end[];

extern uint8_t _binary_resources_SysCore_sprx_compressed_start[];
extern uint8_t _binary_resources_SysCore_sprx_compressed_end[];

extern uint8_t _binary_resources_Bootstrapper_sprx_compressed_start[];
extern uint8_t _binary_resources_Bootstrapper_sprx_compressed_end[];

int InstallPlugins();
int LoadPlugins();