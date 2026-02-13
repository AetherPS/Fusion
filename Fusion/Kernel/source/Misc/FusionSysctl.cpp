#include "Common.h"
#include "FusionSysctl.h"
#include "TrapFatalExtension.h"
#include "FakePkgs.h"
#include "FakeSelfs.h"
#include "FusionDriver.h"
#include "ExtendedLogging.h"
#include "Bootstrapper.h"
#include "DirectMemory.h"
#include "DevActSpoofer.h"
#include "DipSwitchSpoofer.h"
#include "LibraryReplacer.h"
#include "TTYRedirector.h"

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
char FusionSysctl::fusion_version[64] = "1.5.28";
char FusionSysctl::fusion_build_date[64] = __DATE__;
char FusionSysctl::fusion_build_time[64] = __TIME__;
int FusionSysctl::fusion_direct_memory_pages = 300;

// Feature state tracking (0 = disabled/not initialized, 1 = enabled/initialized)
// All features disabled by default, loader sets them based on Settings.ini
int FusionSysctl::fusion_feature_jit = 0;
int FusionSysctl::fusion_feature_extended_logging = 0;
int FusionSysctl::fusion_feature_bootstrapper = 0;
int FusionSysctl::fusion_feature_direct_memory = 0;
int FusionSysctl::fusion_feature_library_replacer = 0;
int FusionSysctl::fusion_feature_devact = 0;
int FusionSysctl::fusion_feature_dipsw = 0;
int FusionSysctl::fusion_feature_tty_redirect = 0;
int FusionSysctl::fusion_feature_homebrew = 0;
int FusionSysctl::fusion_feature_driver = 0;
int FusionSysctl::fusion_feature_fuse = 0;
int FusionSysctl::fusion_feature_traphooks = 0;

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

	// Add feature flags with handlers
	SYSCTL_ADD_PROC(&sysctl_ctx, SYSCTL_CHILDREN(oid_fusion_featureflag),
		OID_AUTO, "Jit", CTLTYPE_INT | CTLFLAG_RW | CTLFLAG_MPSAFE,
		&fusion_feature_jit, 0, (int (*)())SysctlHandlerJit, "I",
		"ShellUI JIT support enabled");

	SYSCTL_ADD_PROC(&sysctl_ctx, SYSCTL_CHILDREN(oid_fusion_featureflag),
		OID_AUTO, "ExtendedLogging", CTLTYPE_INT | CTLFLAG_RW | CTLFLAG_MPSAFE,
		&fusion_feature_extended_logging, 0, (int (*)())SysctlHandlerExtendedLogging, "I",
		"Extended logging enabled");

	SYSCTL_ADD_PROC(&sysctl_ctx, SYSCTL_CHILDREN(oid_fusion_featureflag),
		OID_AUTO, "Bootstrapper", CTLTYPE_INT | CTLFLAG_RW | CTLFLAG_MPSAFE,
		&fusion_feature_bootstrapper, 0, (int (*)())SysctlHandlerBootstrapper, "I",
		"Bootstrapper enabled");

	SYSCTL_ADD_PROC(&sysctl_ctx, SYSCTL_CHILDREN(oid_fusion_featureflag),
		OID_AUTO, "DirectMemory", CTLTYPE_INT | CTLFLAG_RW | CTLFLAG_MPSAFE,
		&fusion_feature_direct_memory, 0, (int (*)())SysctlHandlerDirectMemory, "I",
		"Direct memory support enabled");

	SYSCTL_ADD_PROC(&sysctl_ctx, SYSCTL_CHILDREN(oid_fusion_featureflag),
		OID_AUTO, "LibraryReplace", CTLTYPE_INT | CTLFLAG_RW | CTLFLAG_MPSAFE,
		&fusion_feature_library_replacer, 0, (int (*)())SysctlHandlerLibraryReplacer, "I",
		"Library replacer enabled");

	SYSCTL_ADD_PROC(&sysctl_ctx, SYSCTL_CHILDREN(oid_fusion_featureflag),
		OID_AUTO, "DevAct", CTLTYPE_INT | CTLFLAG_RW | CTLFLAG_MPSAFE,
		&fusion_feature_devact, 0, (int (*)())SysctlHandlerDevAct, "I",
		"Dev activation spoofer enabled");

	SYSCTL_ADD_PROC(&sysctl_ctx, SYSCTL_CHILDREN(oid_fusion_featureflag),
		OID_AUTO, "Dipsw", CTLTYPE_INT | CTLFLAG_RW | CTLFLAG_MPSAFE,
		&fusion_feature_dipsw, 0, (int (*)())SysctlHandlerDipsw, "I",
		"Dip switch spoofer enabled");

	SYSCTL_ADD_PROC(&sysctl_ctx, SYSCTL_CHILDREN(oid_fusion_featureflag),
		OID_AUTO, "TTYRedirect", CTLTYPE_INT | CTLFLAG_RW | CTLFLAG_MPSAFE,
		&fusion_feature_tty_redirect, 0, (int (*)())SysctlHandlerTTYRedirect, "I",
		"TTY redirector enabled");

	// Add major features with dynamic init/term handlers
	SYSCTL_ADD_PROC(&sysctl_ctx, SYSCTL_CHILDREN(oid_fusion_featureflag),
		OID_AUTO, "Homebrew", CTLTYPE_INT | CTLFLAG_RW | CTLFLAG_MPSAFE,
		&fusion_feature_homebrew, 0, (int (*)())SysctlHandlerHomebrew, "I",
		"Enable/disable homebrew (FakePkgs + FakeSelf)");

	SYSCTL_ADD_PROC(&sysctl_ctx, SYSCTL_CHILDREN(oid_fusion_featureflag),
		OID_AUTO, "Driver", CTLTYPE_INT | CTLFLAG_RW | CTLFLAG_MPSAFE,
		&fusion_feature_driver, 0, (int (*)())SysctlHandlerDriver, "I",
		"Enable/disable Fusion driver (/dev/Fusion)");

	SYSCTL_ADD_PROC(&sysctl_ctx, SYSCTL_CHILDREN(oid_fusion_featureflag),
		OID_AUTO, "Fuse", CTLTYPE_INT | CTLFLAG_RW | CTLFLAG_MPSAFE,
		&fusion_feature_fuse, 0, (int (*)())SysctlHandlerFuse, "I",
		"Enable/disable Fuse filesystem");

	SYSCTL_ADD_PROC(&sysctl_ctx, SYSCTL_CHILDREN(oid_fusion_featureflag),
		OID_AUTO, "TrapHooks", CTLTYPE_INT | CTLFLAG_RW | CTLFLAG_MPSAFE,
		&fusion_feature_traphooks, 0, (int (*)())SysctlHandlerTrapHooks, "I",
		"Enable/disable fatal trap hooks");

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

// Sysctl handler for Homebrew feature (FakePkgs + FakeSelf)
int FusionSysctl::SysctlHandlerHomebrew(SYSCTL_HANDLER_ARGS)
{
	int error, value = fusion_feature_homebrew;

	error = ((int (*)(struct sysctl_oid*, void*, intptr_t, struct sysctl_req*))sysctl_handle_int)(oidp, &value, 0, req);
	if (error != 0 || req->newptr == NULL)
		return error;

	// Validate: only 0 or 1
	if (value != 0 && value != 1)
		return EINVAL;

	// No change needed
	if (value == fusion_feature_homebrew)
		return 0;

	if (value == 1)
	{
		printf("[FusionSysctl] Enabling Homebrew...\n");
		FakePkgs::Init();
		FakeSelf::Init();
		fusion_feature_homebrew = 1;
		printf("[FusionSysctl] Homebrew enabled.\n");
	}
	else
	{
		printf("[FusionSysctl] Disabling Homebrew...\n");
		FakeSelf::Term();
		FakePkgs::Term();
		fusion_feature_homebrew = 0;
		printf("[FusionSysctl] Homebrew disabled.\n");
	}

	return 0;
}

// Sysctl handler for Driver feature
int FusionSysctl::SysctlHandlerDriver(SYSCTL_HANDLER_ARGS)
{
	int error, value = fusion_feature_driver;

	error = ((int (*)(struct sysctl_oid*, void*, intptr_t, struct sysctl_req*))sysctl_handle_int)(oidp, &value, 0, req);
	if (error != 0 || req->newptr == NULL)
		return error;

	// Validate: only 0 or 1
	if (value != 0 && value != 1)
		return EINVAL;

	// No change needed
	if (value == fusion_feature_driver)
		return 0;

	if (value == 1)
	{
		printf("[FusionSysctl] Enabling Driver...\n");
		FusionDriver::Init();
		fusion_feature_driver = 1;
		printf("[FusionSysctl] Driver enabled (/dev/Fusion created).\n");
	}
	else
	{
		printf("[FusionSysctl] Disabling Driver...\n");
		FusionDriver::Term();
		fusion_feature_driver = 0;
		printf("[FusionSysctl] Driver disabled (/dev/Fusion removed).\n");
	}

	return 0;
}

// Sysctl handler for Fuse feature
int FusionSysctl::SysctlHandlerFuse(SYSCTL_HANDLER_ARGS)
{
	int error, value = fusion_feature_fuse;

	error = ((int (*)(struct sysctl_oid*, void*, intptr_t, struct sysctl_req*))sysctl_handle_int)(oidp, &value, 0, req);
	if (error != 0 || req->newptr == NULL)
		return error;

	// Validate: only 0 or 1
	if (value != 0 && value != 1)
		return EINVAL;

	// No change needed
	if (value == fusion_feature_fuse)
		return 0;

	if (value == 1)
	{
		printf("[FusionSysctl] Enabling Fuse...");
		int result = fuse_loader(NULL, 0, NULL);
		if (result == 0)
		{
			fusion_feature_fuse = 1;
			printf("Done.\n");
		}
		else
		{
			printf("Failed (error %d).\n", result);
			return EIO;
		}
	}
	else
	{
		printf("[FusionSysctl] Disabling Fuse...\n");
		fusion_feature_fuse = 0;
		printf("[FusionSysctl] Fuse disabled (warning: teardown may be incomplete).\n");
	}

	return 0;
}

// Sysctl handler for TrapHooks feature
int FusionSysctl::SysctlHandlerTrapHooks(SYSCTL_HANDLER_ARGS)
{
	int error, value = fusion_feature_traphooks;

	error = ((int (*)(struct sysctl_oid*, void*, intptr_t, struct sysctl_req*))sysctl_handle_int)(oidp, &value, 0, req);
	if (error != 0 || req->newptr == NULL)
		return error;

	// Validate: only 0 or 1
	if (value != 0 && value != 1)
		return EINVAL;

	// No change needed
	if (value == fusion_feature_traphooks)
		return 0;

	if (value == 1)
	{
		printf("[FusionSysctl] Enabling TrapHooks...\n");
		TrapFatalExtension::Init();
		fusion_feature_traphooks = 1;
		printf("[FusionSysctl] TrapHooks enabled.\n");
	}
	else
	{
		printf("[FusionSysctl] Disabling TrapHooks...\n");
		TrapFatalExtension::Term();
		fusion_feature_traphooks = 0;
		printf("[FusionSysctl] TrapHooks disabled.\n");
	}

	return 0;
}

// Sysctl handler for JIT feature
int FusionSysctl::SysctlHandlerJit(SYSCTL_HANDLER_ARGS)
{
	int error, value = fusion_feature_jit;

	error = ((int (*)(struct sysctl_oid*, void*, intptr_t, struct sysctl_req*))sysctl_handle_int)(oidp, &value, 0, req);
	if (error != 0 || req->newptr == NULL)
		return error;

	if (value != 0 && value != 1)
		return EINVAL;

	if (value == fusion_feature_jit)
		return 0;

	if (value == 1)
	{
		printf("[FusionSysctl] Enabling JIT...\n");
		*(uint8_t*)(g_KernelAddrs.QAFlags + 0x58) |= 0x14;
		fusion_feature_jit = 1;
		printf("[FusionSysctl] JIT enabled.\n");
	}
	else
	{
		printf("[FusionSysctl] Disabling JIT...\n");
		*(uint8_t*)(g_KernelAddrs.QAFlags + 0x58) &= ~0x14;
		fusion_feature_jit = 0;
		printf("[FusionSysctl] JIT disabled.\n");
	}

	return 0;
}

// Sysctl handler for ExtendedLogging feature
int FusionSysctl::SysctlHandlerExtendedLogging(SYSCTL_HANDLER_ARGS)
{
	int error, value = fusion_feature_extended_logging;

	error = ((int (*)(struct sysctl_oid*, void*, intptr_t, struct sysctl_req*))sysctl_handle_int)(oidp, &value, 0, req);
	if (error != 0 || req->newptr == NULL)
		return error;

	if (value != 0 && value != 1)
		return EINVAL;

	if (value == fusion_feature_extended_logging)
		return 0;

	if (value == 1)
	{
		printf("[FusionSysctl] Enabling ExtendedLogging...\n");
		ExtendedLogging::Init();
		fusion_feature_extended_logging = 1;
		printf("[FusionSysctl] ExtendedLogging enabled.\n");
	}
	else
	{
		printf("[FusionSysctl] Disabling ExtendedLogging...\n");
		ExtendedLogging::Term();
		fusion_feature_extended_logging = 0;
		printf("[FusionSysctl] ExtendedLogging disabled.\n");
	}

	return 0;
}

// Sysctl handler for Bootstrapper feature
int FusionSysctl::SysctlHandlerBootstrapper(SYSCTL_HANDLER_ARGS)
{
	int error, value = fusion_feature_bootstrapper;

	error = ((int (*)(struct sysctl_oid*, void*, intptr_t, struct sysctl_req*))sysctl_handle_int)(oidp, &value, 0, req);
	if (error != 0 || req->newptr == NULL)
		return error;

	if (value != 0 && value != 1)
		return EINVAL;

	if (value == fusion_feature_bootstrapper)
		return 0;

	if (value == 1)
	{
		printf("[FusionSysctl] Enabling Bootstrapper...\n");
		Bootstrapper::Init();
		fusion_feature_bootstrapper = 1;
		printf("[FusionSysctl] Bootstrapper enabled.\n");
	}
	else
	{
		printf("[FusionSysctl] Disabling Bootstrapper...\n");
		Bootstrapper::Term();
		fusion_feature_bootstrapper = 0;
		printf("[FusionSysctl] Bootstrapper disabled.\n");
	}

	return 0;
}

// Sysctl handler for DirectMemory feature
int FusionSysctl::SysctlHandlerDirectMemory(SYSCTL_HANDLER_ARGS)
{
	int error, value = fusion_feature_direct_memory;

	error = ((int (*)(struct sysctl_oid*, void*, intptr_t, struct sysctl_req*))sysctl_handle_int)(oidp, &value, 0, req);
	if (error != 0 || req->newptr == NULL)
		return error;

	if (value != 0 && value != 1)
		return EINVAL;

	if (value == fusion_feature_direct_memory)
		return 0;

	if (value == 1)
	{
		printf("[FusionSysctl] Enabling DirectMemory...\n");
		DirectMemory::Init();
		fusion_feature_direct_memory = 1;
		printf("[FusionSysctl] DirectMemory enabled.\n");
	}
	else
	{
		printf("[FusionSysctl] Disabling DirectMemory...\n");
		DirectMemory::Term();
		fusion_feature_direct_memory = 0;
		printf("[FusionSysctl] DirectMemory disabled.\n");
	}

	return 0;
}

// Sysctl handler for LibraryReplacer feature
int FusionSysctl::SysctlHandlerLibraryReplacer(SYSCTL_HANDLER_ARGS)
{
	int error, value = fusion_feature_library_replacer;

	error = ((int (*)(struct sysctl_oid*, void*, intptr_t, struct sysctl_req*))sysctl_handle_int)(oidp, &value, 0, req);
	if (error != 0 || req->newptr == NULL)
		return error;

	if (value != 0 && value != 1)
		return EINVAL;

	if (value == fusion_feature_library_replacer)
		return 0;

	if (value == 1)
	{
		printf("[FusionSysctl] Enabling LibraryReplacer...\n");
		LibraryReplacer::Init();
		fusion_feature_library_replacer = 1;
		printf("[FusionSysctl] LibraryReplacer enabled.\n");
	}
	else
	{
		printf("[FusionSysctl] Disabling LibraryReplacer...\n");
		LibraryReplacer::Term();
		fusion_feature_library_replacer = 0;
		printf("[FusionSysctl] LibraryReplacer disabled.\n");
	}

	return 0;
}

// Sysctl handler for DevAct feature
int FusionSysctl::SysctlHandlerDevAct(SYSCTL_HANDLER_ARGS)
{
	int error, value = fusion_feature_devact;

	error = ((int (*)(struct sysctl_oid*, void*, intptr_t, struct sysctl_req*))sysctl_handle_int)(oidp, &value, 0, req);
	if (error != 0 || req->newptr == NULL)
		return error;

	if (value != 0 && value != 1)
		return EINVAL;

	if (value == fusion_feature_devact)
		return 0;

	if (value == 1)
	{
		printf("[FusionSysctl] Enabling DevActSpoofer...\n");
		DevActSpoofer::Init();
		fusion_feature_devact = 1;
		printf("[FusionSysctl] DevActSpoofer enabled.\n");
	}
	else
	{
		printf("[FusionSysctl] Disabling DevActSpoofer...\n");
		DevActSpoofer::Term();
		fusion_feature_devact = 0;
		printf("[FusionSysctl] DevActSpoofer disabled.\n");
	}

	return 0;
}

// Sysctl handler for Dipsw feature
int FusionSysctl::SysctlHandlerDipsw(SYSCTL_HANDLER_ARGS)
{
	int error, value = fusion_feature_dipsw;

	error = ((int (*)(struct sysctl_oid*, void*, intptr_t, struct sysctl_req*))sysctl_handle_int)(oidp, &value, 0, req);
	if (error != 0 || req->newptr == NULL)
		return error;

	if (value != 0 && value != 1)
		return EINVAL;

	if (value == fusion_feature_dipsw)
		return 0;

	if (value == 1)
	{
		printf("[FusionSysctl] Enabling DipSwitchSpoofer...\n");
		DipSwitchSpoofer::Init();
		fusion_feature_dipsw = 1;
		printf("[FusionSysctl] DipSwitchSpoofer enabled.\n");
	}
	else
	{
		printf("[FusionSysctl] Disabling DipSwitchSpoofer...\n");
		DipSwitchSpoofer::Term();
		fusion_feature_dipsw = 0;
		printf("[FusionSysctl] DipSwitchSpoofer disabled.\n");
	}

	return 0;
}

// Sysctl handler for TTYRedirect feature
int FusionSysctl::SysctlHandlerTTYRedirect(SYSCTL_HANDLER_ARGS)
{
	int error, value = fusion_feature_tty_redirect;

	error = ((int (*)(struct sysctl_oid*, void*, intptr_t, struct sysctl_req*))sysctl_handle_int)(oidp, &value, 0, req);
	if (error != 0 || req->newptr == NULL)
		return error;

	if (value != 0 && value != 1)
		return EINVAL;

	if (value == fusion_feature_tty_redirect)
		return 0;

	if (value == 1)
	{
		printf("[FusionSysctl] Enabling TTYRedirect...\n");
		TTYRedirector::Init();
		fusion_feature_tty_redirect = 1;
		printf("[FusionSysctl] TTYRedirect enabled.\n");
	}
	else
	{
		printf("[FusionSysctl] Disabling TTYRedirect...\n");
		TTYRedirector::Term();
		fusion_feature_tty_redirect = 0;
		printf("[FusionSysctl] TTYRedirect disabled.\n");
	}

	return 0;
}
