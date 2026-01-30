PS4_HOST ?= ps4
PS4_PORT ?= 9021

ifdef PS4_PAYLOAD_SDK
    include $(PS4_PAYLOAD_SDK)/toolchain/orbis.mk
else
    $(error PS4_PAYLOAD_SDK is undefined)
endif

# External dependencies
FUSION_SHARED = ../../External/FusionShared
PLUGIN_DIR = ../../External/Fusion-Plugins/Build
OUTPUT_DIR = ../../Build

# Kloader SDK paths (your existing structure)
KLOADER_SDK = ../../External/PS4-Kernel-SDK
KLOADER_DIR = $(KLOADER_SDK)/loader
KLOADER_LIB = $(KLOADER_DIR)/lib/libkloader.a
KLOADER_INCLUDE = $(KLOADER_DIR)/include
KLOADER_SDK_INCLUDE = $(KLOADER_SDK)/include

# Directories
SDIR = source
IDIR = include
ODIR = build
RDIR = resources

# Include paths
CFLAGS := -Wall -Werror -g \
          -I$(IDIR) \
          -I$(FUSION_SHARED) \
          -I$(KLOADER_INCLUDE) \
          -I$(KLOADER_SDK_INCLUDE) \
          -D_LOADER

# SCE Libraries to link
LDFLAGS := -lkernel -lc

# Assembly flags (no preprocessor defines or include paths needed)
ASFLAGS := -g

# Debug build
ifeq ($(DEBUG), 1)
    CFLAGS += -DDEBUG
endif

# Source files
CFILES := $(wildcard $(SDIR)/*.c)
CPPFILES := $(wildcard $(SDIR)/*.cpp)
ASMFILES := $(wildcard $(SDIR)/*.s)

# Find all resource files
RFILES := $(shell find $(RDIR) -type f 2>/dev/null)

# Plugin files
PLUGIN_SPRX_FILES := $(wildcard $(PLUGIN_DIR)/*.sprx)

# Object files
OBJS := $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(CFILES)) \
        $(patsubst $(SDIR)/%.cpp, $(ODIR)/%.o, $(CPPFILES)) \
        $(patsubst $(SDIR)/%.s, $(ODIR)/%.o, $(ASMFILES)) \
        $(patsubst $(RDIR)/%, $(ODIR)/resources_%.o, $(RFILES))

# Targets
ELF := Fusion.elf
TARGET_PATH := $(OUTPUT_DIR)/$(ELF)

.PHONY: all clean test prepare_resources kloader rebuild

all: $(TARGET_PATH)

# Build kloader library if it doesn't exist
kloader:
	@if [ ! -f $(KLOADER_LIB) ]; then \
		echo "Building kloader library..."; \
		$(MAKE) -C $(KLOADER_DIR); \
	fi

# Prepare resources: compress kernel module and plugins
prepare_resources:
	@mkdir -p $(RDIR)
	@echo "Compressing Kernel.elf with zlib..."
	@python3 ../../Scripts/create_zlib_blob.py "$(OUTPUT_DIR)/Kernel.elf" "$(RDIR)/Kernel.elf.compressed"
	@echo "Compressing plugin SPRX files..."
	@for sprx in $(PLUGIN_SPRX_FILES); do \
		basename=$$(basename $$sprx); \
		echo "  Compressing $$basename..."; \
		python3 ../../Scripts/create_zlib_blob.py "$$sprx" "$(RDIR)/$$basename.compressed"; \
	done
	@echo "Compressing ShellUIManaged_dll.sprx..."
	@if [ -f "$(PLUGIN_DIR)/ShellUIManaged_dll.sprx" ]; then \
		python3 ../../Scripts/create_zlib_blob.py "$(PLUGIN_DIR)/ShellUIManaged_dll.sprx" "$(RDIR)/ShellUIManaged_dll.sprx.compressed"; \
	else \
		echo "Warning: ShellUIManaged_dll.sprx not found, skipping..."; \
	fi
	@echo "Resource preparation complete."

# Create object directory
$(ODIR):
	@mkdir -p $@

# Link final ELF
$(TARGET_PATH): kloader prepare_resources $(ODIR) $(OBJS)
	@mkdir -p $(OUTPUT_DIR)
	@echo "Linking $(ELF)..."
	$(CXX) $(CFLAGS) -o $@ $(OBJS) $(KLOADER_LIB) $(LDFLAGS)
	@echo "Payload built: $(TARGET_PATH) (size: $$(stat -c%s $(TARGET_PATH) 2>/dev/null || stat -f%z $(TARGET_PATH)) bytes)"

# Compile C sources
$(ODIR)/%.o: $(SDIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling: $<"
	$(CC) -c -o $@ $< $(CFLAGS)

# Compile C++ sources
$(ODIR)/%.o: $(SDIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling: $<"
	$(CXX) -c -o $@ $< $(CFLAGS)

# Compile assembly sources (use ASFLAGS instead of CFLAGS)
$(ODIR)/%.o: $(SDIR)/%.s
	@mkdir -p $(dir $@)
	@echo "Assembling: $<"
	$(CC) -c -o $@ $< $(ASFLAGS)

# Embed resource files as binary data
# Use plain ld without PIE flags for relocatable linking
$(ODIR)/resources_%.o: $(RDIR)/%
	@mkdir -p $(dir $@)
	@echo "Embedding resource: $<"
	@ld -r -b binary -o "$@" "$<"
	@$(OBJCOPY) --rename-section .data=.rodata,alloc,load,readonly,data,contents \
	           --set-section-alignment .rodata=4096 "$@"
	@$(OBJCOPY) --add-section .note.GNU-stack=/dev/null \
	           --set-section-flags .note.GNU-stack=noload,readonly "$@"

# Deploy to PS4
test: $(TARGET_PATH)
	$(PS4_DEPLOY) -h $(PS4_HOST) -p $(PS4_PORT) $^

# Rebuild everything
rebuild: clean all

# Clean build artifacts
clean:
	@echo "Cleaning project..."
	@rm -rf $(ODIR) $(TARGET_PATH)

# Clean everything including kloader
distclean: clean
	@echo "Cleaning kloader library..."
	@$(MAKE) -C $(KLOADER_DIR) clean