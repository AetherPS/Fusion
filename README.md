# 🌀 Fusion

Fusion is a lightweight HEN (Homebrew ENabler) and research utility designed for the PlayStation 4. It provides a minimal kernel driver that exposes essential kernel functionality such as jailbreaking and system modifications to userland processes.

The core philosophy behind Fusion is simplicity, stability, and modularity. By offloading most HEN logic from the kernel into userland, Fusion reduces kernel complexity, improving reliability and making experimentation safer and easier.

Fusion serves as a platform for developers, researchers, and enthusiasts who want a clean, efficient base for testing and developing on jailbroken PS4 systems. It’s built with maintainability and performance in mind, emphasizing a minimal kernel footprint while still offering powerful userland capabilities.

# 🛠️ Build Steps

Fusion is built using a Linux environment. The recommended setup is via WSL (Windows Subsystem for Linux) using Ubuntu 24.04 LTS.

Follow the steps below to set up your build environment and compile Fusion.

1. Create a WSL Instance for Fusion
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

5. Build Fusion
Open the `Fusion.sln` and build the `Loader` project.

6. Done!
You will find the builds in the `Build` directory in the root of the Fusion Solution. You’re ready to start testing and experimenting with Fusion.