#include "stdafx.h"
#include "DaemonExtension.h"

DaemonExtension::DaemonExtension(Config& config)
{
	if (SetupOverlay())
	{
		LoadDaemonsFromConfig(config);
	}
}

bool DaemonExtension::SetupOverlay()
{
	if (overlayReady)
		return true;

	sceKernelMkdir(FUSION_DAEMON_DIR, 0777);
	sceKernelMkdir(FUSION_BACKUP_DIR, 0777);

	// Backup original /system/vsh/app
	auto res = MountNullfs(SYSTEM_APP_DIR, FUSION_BACKUP_DIR);
	if (res < 0)
	{
		klog("Failed to backup %s %X", SYSTEM_APP_DIR, res);
		return false;
	}

	// Overlay tmpfs on /system/vsh/app
	if (MountTmpfs(SYSTEM_APP_DIR) < 0)
	{
		klog("Failed to mount tmpfs on %s", SYSTEM_APP_DIR);
		return false;
	}

	// Restore original content
	int fd = sceKernelOpen(FUSION_BACKUP_DIR, 0, 0);
	if (fd < 0)
	{
		klog("Failed to open %s", FUSION_BACKUP_DIR);
		return false;
	}

	char buf[2048];
	int nread;

	while ((nread = sceKernelGetdents(fd, buf, sizeof(buf))) > 0)
	{
		struct dirent* entry;
		for (int pos = 0; pos < nread;)
		{
			entry = (struct dirent*)(buf + pos);

			if (entry->d_type == DT_DIR && entry->d_name[0] != '.')
			{
				char src[256], dst[256];
				snprintf(src, sizeof(src), "%s/%s", FUSION_BACKUP_DIR, entry->d_name);
				snprintf(dst, sizeof(dst), "%s/%s", SYSTEM_APP_DIR, entry->d_name);

				sceKernelMkdir(dst, 0777);
				MountNullfs(src, dst);
			}

			pos += entry->d_reclen;
		}
	}
	sceKernelClose(fd);

	overlayReady = true;
	return true;
}

int DaemonExtension::MountDaemon(const char* titleId)
{
	if (!overlayReady)
	{
		klog("Overlay not ready, cannot mount daemon: %s", titleId);
		return -1;
	}

	char src[256], dst[256];
	snprintf(src, sizeof(src), "%s/%s", FUSION_DAEMON_DIR, titleId);
	snprintf(dst, sizeof(dst), "%s/%s", SYSTEM_APP_DIR, titleId);

	sceKernelMkdir(dst, 0777);
	int ret = MountNullfs(src, dst);

	if (ret < 0)
	{
		klog("Failed to mount daemon: %s", titleId);
	}

	return ret;
}

int DaemonExtension::MountAndLaunchDaemon(const char* titleId)
{
	int ret = MountDaemon(titleId);
	if (ret < 0)
		return ret;

	auto result = StartRestartApp(titleId, nullptr, -1);
	if (result < 0)
	{
		klog("Failed to launch daemon: %s", titleId);
		return result;
	}

	loadedDaemons.push_back(titleId);
	klog("Launched daemon: %s", titleId);

	return 0;
}

int DaemonExtension::LoadDaemonsFromConfig(Config& config)
{
	auto daemons = config.Get<std::vector<std::string>>("Daemons");
	if (!daemons.has_value())
	{
		klog("No daemons configured");
		return 0;
	}

	int count = 0;
	for (const auto& titleId : daemons.value())
	{
		if (MountAndLaunchDaemon(titleId.c_str()) == 0)
		{
			sceKernelUsleep(1000 * 500); // Small delay between launches
			count++;
		}
	}

	klog("Loaded %d/%zu daemons from config", count, daemons.value().size());
	return count;
}

void DaemonExtension::AddDaemonToConfig(Config& config, const std::string& titleId)
{
	auto daemons = config.Get<std::vector<std::string>>("Daemons", std::vector<std::string>{});

	// Check if already exists
	for (const auto& d : daemons)
	{
		if (d == titleId)
		{
			klog("Daemon %s already in config", titleId.c_str());
			return;
		}
	}

	daemons.push_back(titleId);
	config.Set("Daemons", daemons);
}

void DaemonExtension::RemoveDaemonFromConfig(Config& config, const std::string& titleId)
{
	auto daemons = config.Get<std::vector<std::string>>("Daemons", std::vector<std::string>{});

	auto it = std::find(daemons.begin(), daemons.end(), titleId);
	if (it != daemons.end())
	{
		daemons.erase(it);
		config.Set("Daemons", daemons);
	}
	else
	{
		klog("Daemon %s not found in config", titleId.c_str());
	}
}

void DaemonExtension::CreateDefaultConfig(Config& config)
{
	if (!config.Has("Daemons"))
	{
		config.Set("Daemons", std::vector<std::string>{});
	}
}