#pragma once

class Offsets
{
public:
	static bool Init()
	{
		SceKernelSwVersion SoftwareVersion;
		SoftwareVersion.Size = sizeof(SceKernelSwVersion);
		if (sceKernelGetSystemSwVersion(&SoftwareVersion) != 0)
		{
			Logger::Error("Failed to get the software version.");
			return false;
		}

		switch (SoftwareVersion.Version)
		{
		case 0x9008031:

			// ShellCore

			// ShellUI
			DebugMenuPatch1 = 0x0001D1C0;
			DebugMenuPatch2 = 0x0001D520;

			break;

		case 0x12020001:

			// ShellCore

			// ShellUI
			DebugMenuPatch1 = 0x0001D100;
			DebugMenuPatch2 = 0x0001D460;

			break;

		default:
			return false;
		}

		return true;
	}

	// ShellCore

	// ShellUI
	static inline uint64_t DebugMenuPatch1;
	static inline uint64_t DebugMenuPatch2;

private:

};
