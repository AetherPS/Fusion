PS4_HOST ?= ps4
PS4_PORT ?= 9021

ifdef PS4_PAYLOAD_SDK
    include $(PS4_PAYLOAD_SDK)/toolchain/orbis.mk
else
    $(error PS4_PAYLOAD_SDK is undefined)
endif

MAKEFLAGS += -j$(shell nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

# Project paths
FUSION_SHARED = ../../External/FusionShared
PLUGIN_DIR = ../../External/Fusion-Plugins/Build
OUTPUT_DIR = ../../Build
KERNEL_SDK = ../../External/PS4-Kernel-SDK

# Directories
SDIR = source
IDIR = include
ODIR = build
RDIR = resources

# Compiler flags for size optimization
CFLAGS := -Os -Wall -Werror -g \
          -ffunction-sections -fdata-sections \
          -I$(IDIR) \
          -I$(FUSION_SHARED) \
          -I$(KERNEL_SDK)/loader/include \
          -D_LOADER

# Linker flags with dead code elimination
LDFLAGS := -Wl,--gc-sections -Wl,--strip-all \
           -lkernel -lc -lc++ \
           $(KERNEL_SDK)/loader/lib/libkloader.a

# Source files
CPPFILES := $(wildcard $(SDIR)/*.cpp)
ASMFILES := $(wildcard $(SDIR)/*.s)
RFILES := $(shell find $(RDIR) -type f 2>/dev/null)

# Object files
OBJS := $(patsubst $(SDIR)/%.cpp, $(ODIR)/%.o, $(CPPFILES)) \
        $(patsubst $(SDIR)/%.s, $(ODIR)/%.o, $(ASMFILES)) \
        $(patsubst $(RDIR)/%, $(ODIR)/resources_%.o, $(RFILES))

# Target
ELF := Fusion.elf
TARGET := $(OUTPUT_DIR)/$(ELF)
TARGET_SYM := $(TARGET).sym
TARGET_UNSTRIPPED := $(OUTPUT_DIR)/Fusion_debug.elf

.PHONY: all clean test prepare_resources rebuild

all: $(TARGET)

# Prepare compressed resources
prepare_resources:
	@mkdir -p $(RDIR)
	@echo "Compressing resources..."
	@python3 ../../Scripts/create_zlib_blob.py \
		"$(OUTPUT_DIR)/Kernel.elf" "$(RDIR)/Kernel.elf.compressed"
	@for sprx in $(PLUGIN_DIR)/*.sprx; do \
		[ -f "$$sprx" ] || continue; \
		python3 ../../Scripts/create_zlib_blob.py \
			"$$sprx" "$(RDIR)/$$(basename $$sprx).compressed"; \
	done

# Build final ELF (unstripped)
$(TARGET_UNSTRIPPED): prepare_resources $(OBJS)
	@mkdir -p $(OUTPUT_DIR)
	@echo "Linking $(ELF) (with debug info)..."
	$(CXX) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

# Extract symbols and strip to create final target
$(TARGET): $(TARGET_UNSTRIPPED)
	@echo "Extracting debug symbols to $(TARGET_SYM)..."
	@$(OBJCOPY) --only-keep-debug $< $(TARGET_SYM)
	@echo "Stripping $(ELF)..."
	@cp $< $@
	@$(STRIP) --strip-all $@
	@$(OBJCOPY) --add-gnu-debuglink=$(TARGET_SYM) $@
	@echo "Built: $@ ($$(stat -c%s $@ 2>/dev/null || stat -f%z $@) bytes)"
	@echo "Debug symbols: $(TARGET_SYM) ($$(stat -c%s $(TARGET_SYM) 2>/dev/null || stat -f%z $(TARGET_SYM)) bytes)"

# Create build directory
$(ODIR):
	@mkdir -p $(ODIR)

# Compile C++ sources
$(ODIR)/%.o: $(SDIR)/%.cpp $(ODIR)
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	@$(CXX) -c -o $@ $< $(CFLAGS)

# Compile assembly sources  
$(ODIR)/%.o: $(SDIR)/%.s $(ODIR)
	@mkdir -p $(dir $@)
	@echo "Assembling $<"
	@$(CC) -c -o $@ $< -g

# Embed binary resources - use system ld
$(ODIR)/resources_%.o: $(RDIR)/%
	@mkdir -p $(dir $@)
	@echo "Embedding $<"
	@ld -r -b binary -o $@ "$<"
	@$(OBJCOPY) --rename-section .data=.rodata,alloc,load,readonly,data,contents --set-section-alignment .rodata=4096 $@

test: $(TARGET)
	$(PS4_DEPLOY) -h $(PS4_HOST) -p $(PS4_PORT) $^

rebuild: clean all

clean:
	@echo "Cleaning project..."
	@rm -rf $(ODIR) $(TARGET) $(TARGET_SYM) $(TARGET_UNSTRIPPED)