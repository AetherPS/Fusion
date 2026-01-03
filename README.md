# 🌀 Fusion

Fusion is a minimal kernel utility designed for PlayStation 4 research and development. It maintains a small kernel footprint while exposing essential kernel functionality to userland through a daemon and driver interface. By keeping the kernel layer minimal and offloading logic to userland processes, Fusion enables safer experimentation and development on jailbroken PS4 systems.

**Supported Firmware Versions:** Fusion is currently version-specific and supports firmware **9.00** and **12.02**.

# ✨ Features

## Daemon Configuration System
The Fusion daemon includes a configuration system that allows it to automatically manage and launch various services:

- **FTP Server** - The daemon can launch the FTP server for file transfer capabilities
- **DECI Daemon Auto-Start** - If the DECI environment has been configured on your system, the daemon will automatically start the DECI daemon. For setup instructions, see the [PS4 RetailKit series](https://www.hackingadventures.ca/series/ps4-retailkit/).

## Daemon Loader
Automatically mounts and launches custom daemon applications at Fusion startup. Uses an overlay filesystem to seamlessly integrate custom daemons alongside system applications without modifying the original `/system/vsh/app` directory. Daemons are configured via `/data/Fusion/Settings.cfg` and stored in `/data/Fusion/Daemons/`.

## Plugin Loader
Automatically loads SPRX plugins when applications launch. Supports both global plugins (loaded for all apps) and title-specific plugins. Configure via `/data/Fusion/PluginList.ini`.

## Library Replacer
Kernel-level library interception system that allows replacing system libraries with custom versions. Supports both title-specific and global library replacements.

**📖 For detailed configuration instructions, see [CONFIGURATION.md](CONFIGURATION.md)**

# 🛠️ Build Steps

Fusion requires the **Official PS4 SDK** to build. The build environment is Linux-based, with WSL (Windows Subsystem for Linux) using Ubuntu 24.04 LTS recommended.

## Build Dependencies
- **Official PlayStation 4 SDK** - Required for building the Daemon
- **Python 3** - Required for build scripts and tooling
- **WSL Ubuntu 24.04 LTS** (Windows users) or native Linux environment
- **GCC/G++** - C/C++ compiler toolchain
- **Make** - Build automation tool

## Building Fusion

1. Create a WSL Instance (Windows users)
Open PowerShell (as Administrator) and run:
```
wsl --install -d Ubuntu-24.04
```

2. Launch the WSL Instance
```
wsl -d Ubuntu-24.04
```

3. Install Build Dependencies
```
sudo apt update && sudo apt install -y make g++ gcc
```

4. Clone the Repository
```
git clone --recurse-submodules https://github.com/AetherPS/Fusion
cd Fusion
```

5. Build External Dependencies
The FTP server and other dependencies in the `External` folder need to be built separately.

6. Build Fusion
Open the `Fusion.sln` and build the `Loader` project. This will compile the kernel driver and userland daemon, which exposes additional kernel functionality to userland.

7. Done!
You will find the builds in the `Build` directory in the root of the Fusion Solution.
