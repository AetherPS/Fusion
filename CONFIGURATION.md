# Fusion Configuration Guide

This guide explains how to use Fusion's plugin loader, configuration system, and library replacer features.

## Table of Contents
- [Plugin Loader](#plugin-loader)
- [Configuration System](#configuration-system)
- [Library Replacer](#library-replacer)

---

## Plugin Loader

The Plugin Loader automatically monitors running applications and loads SPRX plugins when an application launches.

### How It Works

The plugin system monitors the active application every 500ms. When a new application is detected, the daemon waits 1.8 seconds for the app to initialize, then loads the appropriate plugins based on the configuration.

### Configuration File

Create an INI file at:
```
/data/Fusion/PluginList.ini
```

### INI File Format

The INI file supports two types of plugin entries:

#### Default Plugins (Loaded for All Applications)
```ini
[default]
/data/plugins/universal_overlay.sprx
/data/plugins/fps_counter.sprx
```

#### Title-Specific Plugins
```ini
[CUSA12345]
/data/plugins/game_specific_mod.sprx
/data/plugins/debug_helper.sprx
```

Replace `CUSA12345` with the actual Title ID of your target application.

### Complete Example

```ini
[default]
/data/plugins/overlay.sprx
/data/plugins/screenshot_tool.sprx

[CUSA12345]
/data/plugins/game_mod.sprx

[CUSA67890]
/data/plugins/trainer.sprx
/data/plugins/cheats.sprx
```

### Important Notes

- Each plugin path should be on its own line within the section
- Default plugins are loaded first, followed by title-specific plugins
- Duplicate plugin paths are automatically filtered out
- The system uses absolute file paths (starting with `/`)
- Plugins are loaded via the `Fusion::LoadSprx()` function

---

## Configuration System

The Fusion daemon uses a JSON-based configuration system for managing various settings.

### Configuration File Location

```
/data/Fusion/Settings.cfg
```

### Default Configuration

If the configuration file doesn't exist, Fusion will automatically create it with these defaults:

```json
{
    "EnableFTP": false,
    "StartDECI": true
}
```

### Available Options

| Option | Type | Default | Description |
|--------|------|---------|-------------|
| `EnableFTP` | boolean | `false` | Enables the FTP server for file transfer |
| `StartDECI` | boolean | `true` | Automatically starts the DECI daemon if configured |

### Nested Configuration

The configuration system supports nested keys using dot notation:

```json
{
    "EnableFTP": true,
    "StartDECI": false,
    "database": {
        "host": "localhost",
        "port": 5432
    }
}
```

Access nested values in code using: `config.Get<std::string>("database.host")`

### Manual Configuration

You can manually edit the configuration file:

1. Create or edit `/data/Fusion/Settings.cfg`
2. Use valid JSON format
3. Restart the Fusion daemon for changes to take effect

### Example Custom Configuration

```json
{
    "EnableFTP": true,
    "StartDECI": false
}
```

---

## Library Replacer

The Library Replacer is a kernel-level feature that intercepts library loading requests and allows you to substitute custom libraries.

### How It Works

The Library Replacer hooks the `sys_dynlib_load_prx` system call. When an application attempts to load a library, the system checks for replacement libraries in specific directories. If a replacement is found, it loads the custom library instead of the original.

### Directory Structure

Create replacement libraries in:

```
/data/Fusion/ReplacementLibs/
```

The replacer supports two types of replacements:

#### 1. Title-Specific Replacements
Place libraries in a folder named after the Title ID:
```
/data/Fusion/ReplacementLibs/[TITLEID]/[library_name].prx
```

Example:
```
/data/Fusion/ReplacementLibs/CUSA12345/libScePad.prx
```

#### 2. Global Replacements (Any Application)
Place libraries in the `Any` folder:
```
/data/Fusion/ReplacementLibs/Any/[library_name].prx
```

Example:
```
/data/Fusion/ReplacementLibs/Any/libScePad.prx
```

### Priority

The system checks for replacements in this order:
1. Title-specific replacement (`/data/Fusion/ReplacementLibs/[TITLEID]/`)
2. Global replacement (`/data/Fusion/ReplacementLibs/Any/`)
3. Original library (if no replacement found)

### Usage Examples

#### Example 1: Replace a System Library for All Apps
To replace `libScePad.prx` for all applications:
```
/data/Fusion/ReplacementLibs/Any/libScePad.prx
```

#### Example 2: Replace a Library for a Specific Game
To replace `libScePad.prx` only for title `CUSA12345`:
```
/data/Fusion/ReplacementLibs/CUSA12345/libScePad.prx
```

#### Example 3: Game-Specific and Global Replacements
```
/data/Fusion/ReplacementLibs/Any/libSceNet.prx           # Used by all apps
/data/Fusion/ReplacementLibs/CUSA00000/libGameEngine.prx # Used only by CUSA00000
/data/Fusion/ReplacementLibs/CUSA11111/libGameEngine.prx # Used only by CUSA11111
```

### Important Notes

- The replacement library must have the exact same filename as the original
- The replacement library must be compatible with the target application
- Title-specific replacements override global replacements
- The system automatically creates the required directories on initialization
- All replacement attempts are logged to the console
- This feature operates at the kernel level for maximum compatibility

---

## Additional Information

### File Paths

All Fusion data is stored under `/data/Fusion/`:
- `/data/Fusion/PluginList.ini` - Plugin loader configuration
- `/data/Fusion/Settings.cfg` - Daemon configuration
- `/data/Fusion/ReplacementLibs/` - Library replacer storage

### Finding Title IDs

To find the Title ID of an application:
1. Check the game's disc or digital store page
2. Title IDs typically follow the format `CUSAXXXXX` (US), `CUSA-XXXXX` (EU), or `CUSA-XXXXX` (JP)
3. You can also check the param.sfo file in the game's directory

### Troubleshooting

If plugins or replacements aren't loading:
1. Verify the file paths are correct and absolute
2. Check that the sprx files have be correctly fake signed
3. Ensure the Title ID matches exactly (case-sensitive)
4. Review the console output for error messages
5. Confirm the Fusion daemon is running
6. Verify the INI/JSON file syntax is valid
