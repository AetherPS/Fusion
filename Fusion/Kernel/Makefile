SDK = ../../External/PS4-Kernel-SDK

# Set output directory for final artifacts
OUTPUT_DIR = ../../Build
export OUTPUT_DIR

# Override resource directory to point to shellcode
RDIR = $(OUTPUT_DIR)/ShellCode

include $(SDK)/sdk.mk

CFLAGS += -I../../External/FusionShared

# Module metadata
MODULE_NAME    = Fusion
MODULE_VERSION = 1.4.0
MODULE_AUTHOR  = OSM-Made
MODULE_DESC    = A development kit for building self-contained PlayStation 4 kernel modules with automatic symbol resolution across multiple firmware versions.

# Debug: print what resources we found
$(info RDIR = $(RDIR))
$(info RFILES = $(RFILES))
$(info RESOURCE_OBJS = $(RESOURCE_OBJS))

all: $(TARGET_PATH) symbols
	@echo "Build complete: $(TARGET) and $(TARGET).sym in $(OUTPUT_DIR)"

clean:
	rm -rf $(TARGET_PATH) $(TARGET_PATH).sym $(ODIR)

.PHONY: all clean