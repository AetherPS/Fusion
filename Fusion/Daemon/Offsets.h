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
			IsGenuineCEX1 = 0x0016EAA4;
			IsGenuineCEX2 = 0x008621D4;
			IsGenuineCEX3 = 0x008AFBC2;
			IsGenuineCEX4 = 0x00A27BD4;
			IsAssistMode1 = 0x0016EAD2;
			IsAssistMode2 = 0x00249F7B;
			IsAssistMode3 = 0x00862202;
			IsAssistMode4 = 0x00A27C02;
			EnableFakePkg = 0x003D7AFF;
			FakeText = 0x00FD3211;
			MountDataIntoSandbox = 0x0032079B;
			DisablePkgPatchCheck1 = 0x00138E90;
			DisablePkgPatchCheck2 = 0x003C5EA7;
			DisablePkgPatchCheck3 = 0x003C8540;

			// ShellUI
			DebugMenuPatch1 = 0x0001D1C0;
			DebugMenuPatch2 = 0x0001D520;

			// DECI
			StartDecidServer = 0x251F60;
			MountFuse = 0x12C0;
			DevPortThread = 0x1480;

			break;

		case 0x12020001:

			// ShellCore
			IsGenuineCEX1 = 0x0016F5A4;
			IsGenuineCEX2 = 0x00873754;
			IsGenuineCEX3 = 0x008C3A52;
			IsGenuineCEX4 = 0x00A27304;
			IsAssistMode1 = 0x0016F5D2;
			IsAssistMode2 = 0x0024E14C;
			IsAssistMode3 = 0x00873782;
			IsAssistMode4 = 0x00A27332;
			EnableFakePkg = 0x003DE23F;
			FakeText = 0x00FCFDF9;
			MountDataIntoSandbox = 0x003233B0;
			DisablePkgPatchCheck1 = 0x001389A0;
			DisablePkgPatchCheck2 = 0x003CA567;
			DisablePkgPatchCheck3 = 0x003CD7B0;

			// ShellUI
			DebugMenuPatch1 = 0x0001D100;
			DebugMenuPatch2 = 0x0001D460;

			// DECI
			StartDecidServer = 0x255EC0;
			MountFuse = 0x12A0;
			DevPortThread = 0x1480;

			break;

		case 0x13008002:

			// ShellCore
			IsGenuineCEX1 = 0x0;
			IsGenuineCEX2 = 0x0;
			IsGenuineCEX3 = 0x0;
			IsGenuineCEX4 = 0x0;
			IsAssistMode1 = 0x0;
			IsAssistMode2 = 0x0;
			IsAssistMode3 = 0x0;
			IsAssistMode4 = 0x0;
			EnableFakePkg = 0x0;
			FakeText = 0x0;
			MountDataIntoSandbox = 0x0;
			DisablePkgPatchCheck1 = 0x0;
			DisablePkgPatchCheck2 = 0x0;
			DisablePkgPatchCheck3 = 0x0;

			// ShellUI
			DebugMenuPatch1 = 0x0;
			DebugMenuPatch2 = 0x0;

			// DECI
			StartDecidServer = 0x0;
			MountFuse = 0x0;
			DevPortThread = 0x0;

			break;

		default:
			return false;
		}

		return true;
	}

	// ShellCore IsGenuineCEX
	static inline uint64_t IsGenuineCEX1;
	static inline uint64_t IsGenuineCEX2;
	static inline uint64_t IsGenuineCEX3;
	static inline uint64_t IsGenuineCEX4;
	static inline uint64_t IsAssistMode1;
	static inline uint64_t IsAssistMode2;
	static inline uint64_t IsAssistMode3;
	static inline uint64_t IsAssistMode4;
	static inline uint64_t EnableFakePkg;
	static inline uint64_t FakeText;
	static inline uint64_t MountDataIntoSandbox;
	static inline uint64_t DisablePkgPatchCheck1;
	static inline uint64_t DisablePkgPatchCheck2;
	static inline uint64_t DisablePkgPatchCheck3;

	// ShellUI
	static inline uint64_t DebugMenuPatch1;
	static inline uint64_t DebugMenuPatch2;

	// DECI
	static inline uint64_t StartDecidServer;
	static inline uint64_t MountFuse;
	static inline uint64_t DevPortThread;

private:

};
