#include "Common.h"
#include "FusionSysctl.h"

/*
 * Fusion Sysctl Implementation
 *
 * This module creates a hierarchical sysctl tree:
 * Fusion.Version
 * Fusion.BuildDate
 * Fusion.BuildTime
 * Fusion.FeatureFlag.Jit
 * Fusion.FeatureFlag.DirectMemory
 * Fusion.FeatureFlag.LibraryReplacer
 * Fusion.FeatureFlag.DevAct
 * Fusion.FeatureFlag.Dipsw
 * Fusion.FeatureFlag.TTYRedirect
 * Fusion.DirectMemoryReservationSize
 */

// Static member definitions
char FusionSysctl::fusion_version[64] = "1.4.167";
char FusionSysctl::fusion_build_date[64] = __DATE__;
char FusionSysctl::fusion_build_time[64] = __TIME__;
int FusionSysctl::fusion_direct_memory_pages = 300;
int FusionSysctl::fusion_feature_jit = 1;
int FusionSysctl::fusion_feature_direct_memory = 1;
int FusionSysctl::fusion_feature_library_replacer = 1;
int FusionSysctl::fusion_feature_devact = 1;
int FusionSysctl::fusion_feature_dipsw = 1;
int FusionSysctl::fusion_feature_tty_redirect = 0;
sysctl_ctx_list FusionSysctl::sysctl_ctx = { nullptr };
sysctl_oid* FusionSysctl::oid_fusion_root = nullptr;
sysctl_oid* FusionSysctl::oid_fusion_featureflag = nullptr;

void FusionSysctl::Init()
{
	// Initialize the sysctl context
	sysctl_ctx_init(&sysctl_ctx);

	// Create the main Fusion node at root level
	oid_fusion_root = SYSCTL_ADD_NODE(&sysctl_ctx, sysctl__children,
		OID_AUTO, "Fusion", CTLFLAG_RD, NULL,
		"Fusion kernel module");

	if (!oid_fusion_root) {
		printf("FusionSysctl: Failed to create Fusion root node\n");
		return;
	}

	// Add version string
	SYSCTL_ADD_STRING(&sysctl_ctx, SYSCTL_CHILDREN(oid_fusion_root),
		OID_AUTO, "Version", CTLFLAG_RD,
		fusion_version, sizeof(fusion_version), "Fusion version");

	// Add build date
	SYSCTL_ADD_STRING(&sysctl_ctx, SYSCTL_CHILDREN(oid_fusion_root),
		OID_AUTO, "BuildDate", CTLFLAG_RD,
		fusion_build_date, sizeof(fusion_build_date), "Build date");

	// Add build time
	SYSCTL_ADD_STRING(&sysctl_ctx, SYSCTL_CHILDREN(oid_fusion_root),
		OID_AUTO, "BuildTime", CTLFLAG_RD,
		fusion_build_time, sizeof(fusion_build_time), "Build time");

	// Create FeatureFlag node
	oid_fusion_featureflag = SYSCTL_ADD_NODE(&sysctl_ctx, SYSCTL_CHILDREN(oid_fusion_root),
		OID_AUTO, "FeatureFlag", CTLFLAG_RD, NULL,
		"Fusion feature flags");

	if (!oid_fusion_featureflag) {
		printf("FusionSysctl: Failed to create FeatureFlag node\n");
		return;
	}

	// Add feature flags
	SYSCTL_ADD_INT(&sysctl_ctx, SYSCTL_CHILDREN(oid_fusion_featureflag),
		OID_AUTO, "Jit", CTLFLAG_RW,
		&fusion_feature_jit, 0, "ShellUI JIT support enabled");

	SYSCTL_ADD_INT(&sysctl_ctx, SYSCTL_CHILDREN(oid_fusion_featureflag),
		OID_AUTO, "DirectMemory", CTLFLAG_RW,
		&fusion_feature_direct_memory, 0, "Direct memory support enabled");

	SYSCTL_ADD_INT(&sysctl_ctx, SYSCTL_CHILDREN(oid_fusion_featureflag),
		OID_AUTO, "LibraryReplace", CTLFLAG_RW,
		&fusion_feature_library_replacer, 0, "Library replacer enabled");

	SYSCTL_ADD_INT(&sysctl_ctx, SYSCTL_CHILDREN(oid_fusion_featureflag),
		OID_AUTO, "DevAct", CTLFLAG_RW,
		&fusion_feature_devact, 0, "Dev activation spoofer enabled");

	SYSCTL_ADD_INT(&sysctl_ctx, SYSCTL_CHILDREN(oid_fusion_featureflag),
		OID_AUTO, "Dipsw", CTLFLAG_RW,
		&fusion_feature_dipsw, 0, "Dip switch spoofer enabled");

	SYSCTL_ADD_INT(&sysctl_ctx, SYSCTL_CHILDREN(oid_fusion_featureflag),
		OID_AUTO, "TTYRedirect", CTLFLAG_RW,
		&fusion_feature_tty_redirect, 0, "TTY redirector enabled");

	// Add direct memory reservation size
	SYSCTL_ADD_INT(&sysctl_ctx, SYSCTL_CHILDREN(oid_fusion_root),
		OID_AUTO, "DirectMemoryReservationSize", CTLFLAG_RW,
		&fusion_direct_memory_pages, 0, "Direct memory reservation size (pages)");

	EnableSysctlRecursive(sysctl__children);

	printf("Fusion Version: %s\nBuild Date: %s\nBuild Time: %s\n", fusion_version, fusion_build_date, fusion_build_time);
}

void FusionSysctl::Term()
{
	sysctl_ctx_free(&sysctl_ctx);

}

void FusionSysctl::EnableSysctlRecursive(struct sysctl_oid_list* parentList)
{
	struct sysctl_oid* oidp;

	SLIST_FOREACH(oidp, parentList, oid_link)
	{
		// Make it readable and accessible by anybody
		oidp->oid_kind |= CTLFLAG_RD | CTLFLAG_ANYBODY;

		// Remove any restrictive flags
		oidp->oid_kind &= ~(CTLFLAG_SECURE | CTLFLAG_PRISON | CTLFLAG_SKIP);

		// Bypass sony check.
		oidp->oid_allowed = 1;

		// If this is a node (has children), recurse into it
		if ((oidp->oid_kind & CTLTYPE) == CTLTYPE_NODE && oidp->oid_handler == NULL)
		{
			struct sysctl_oid_list* childList = (struct sysctl_oid_list*)oidp->oid_arg1;
			if (childList != NULL)
			{
				EnableSysctlRecursive(childList);
			}
		}
	}
}
