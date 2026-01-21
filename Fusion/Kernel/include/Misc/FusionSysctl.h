#pragma once

class FusionSysctl
{
public:
	static void Init();
	static void Term();
	
	// Version string for Fusion
	static char fusion_version[64];

	// Build information
	static char fusion_build_date[64];
	static char fusion_build_time[64];

	// Direct memory reservation size (in pages)
	static int fusion_direct_memory_pages;

	// Feature flags (boolean states)
	static int fusion_feature_jit;
	static int fusion_feature_direct_memory;
	static int fusion_feature_library_replacer;
	static int fusion_feature_devact;
	static int fusion_feature_dipsw;
	static int fusion_feature_tty_redirect;

private:
	static void EnableSysctlRecursive(struct sysctl_oid_list* parentList);

	// Sysctl context
	static sysctl_ctx_list sysctl_ctx;

	// OID nodes
	static sysctl_oid* oid_fusion_root;
	static sysctl_oid* oid_fusion_featureflag;
};
