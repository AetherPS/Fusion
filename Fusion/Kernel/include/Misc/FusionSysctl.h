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
	static int fusion_feature_extended_logging;
	static int fusion_feature_bootstrapper;
	static int fusion_feature_direct_memory;
	static int fusion_feature_library_replacer;
	static int fusion_feature_devact;
	static int fusion_feature_dipsw;
	static int fusion_feature_tty_redirect;
	static int fusion_feature_homebrew;
	static int fusion_feature_driver;
	static int fusion_feature_fuse;
	static int fusion_feature_traphooks;

private:
	static void EnableSysctlRecursive(struct sysctl_oid_list* parentList);

	// Sysctl handlers for all features
	static int SysctlHandlerJit(SYSCTL_HANDLER_ARGS);
	static int SysctlHandlerExtendedLogging(SYSCTL_HANDLER_ARGS);
	static int SysctlHandlerBootstrapper(SYSCTL_HANDLER_ARGS);
	static int SysctlHandlerDirectMemory(SYSCTL_HANDLER_ARGS);
	static int SysctlHandlerLibraryReplacer(SYSCTL_HANDLER_ARGS);
	static int SysctlHandlerDevAct(SYSCTL_HANDLER_ARGS);
	static int SysctlHandlerDipsw(SYSCTL_HANDLER_ARGS);
	static int SysctlHandlerTTYRedirect(SYSCTL_HANDLER_ARGS);
	static int SysctlHandlerHomebrew(SYSCTL_HANDLER_ARGS);
	static int SysctlHandlerDriver(SYSCTL_HANDLER_ARGS);
	static int SysctlHandlerFuse(SYSCTL_HANDLER_ARGS);
	static int SysctlHandlerTrapHooks(SYSCTL_HANDLER_ARGS);

	// Sysctl context
	static sysctl_ctx_list sysctl_ctx;

	// OID nodes
	static sysctl_oid* oid_fusion_root;
	static sysctl_oid* oid_fusion_featureflag;
};
