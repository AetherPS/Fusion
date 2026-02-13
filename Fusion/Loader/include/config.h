#pragma once

extern uint8_t _binary_resources_Settings_ini_start[];
extern uint8_t _binary_resources_Settings_ini_end[];

#define CONFIG_PATH "/data/Fusion/Settings.ini"

int EnsureDefaultConfig();
void ApplyFeatureConfiguration();