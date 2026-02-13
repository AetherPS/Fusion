#pragma once

extern uint8_t _binary_resources_Settings_ini_start[];
extern uint8_t _binary_resources_Settings_ini_end[];

#define CONFIG_PATH "/data/Fusion/Settings.ini"
#define LOADED_MARKER_PATH "/user/temp/Fusion.Loaded"

int EnsureDefaultConfig();
void ApplyFeatureConfiguration();
bool IsAlreadyLoaded();
void MarkAsLoaded();