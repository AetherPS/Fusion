SDK = ../../External/PS4-Kernel-SDK

# Set output directory for final artifacts
OUTPUT_DIR = ../../Build
export OUTPUT_DIR

# Use loader.mk for common definitions
include $(SDK)/loader/loader.mk

CFLAGS += -I../../External/FusionShared -D_LOADER

# Override linker script
LFLAGS := -Xlinker -T Linker -Wl,--build-id=none -Wl,--gc-sections

# Source files
CFILES := $(wildcard $(SDIR)/*.c)

# Find all resource files
RFILES := $(shell find $(RDIR) -type f 2>/dev/null)

# Object files (C sources + embedded resources)
OBJS := $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(CFILES)) $(patsubst $(RDIR)/%, $(ODIR)/resources_%.o, $(RFILES))

ifeq ($(DEBUG), 1)
    CFLAGS += -DDEBUG
endif

# Targets
TARGET = Fusion.bin
ELF_TARGET = Fusion.elf
TARGET_PATH := $(OUTPUT_DIR)/$(TARGET)
ELF_TARGET_PATH := $(OUTPUT_DIR)/$(ELF_TARGET)

all: $(TARGET_PATH)

$(TARGET_PATH): $(ELF_TARGET_PATH)
	@echo "Converting to binary..."
	@$(OBJCOPY) -O binary $(ELF_TARGET_PATH) $(TARGET_PATH)
	@echo "Payload built: $(TARGET_PATH) (size: $$(stat -c%s $(TARGET_PATH) 2>/dev/null || stat -f%z $(TARGET_PATH)) bytes)"

$(ELF_TARGET_PATH): prepare_resources $(ODIR) $(OBJS)
	@mkdir -p $(OUTPUT_DIR)
	@echo "Linking payload ELF..."
	@$(CC) crt0.s $(OBJS) -o $(ELF_TARGET_PATH) $(CFLAGS) $(LFLAGS) $(LIBS)

# Compile C sources
$(ODIR)/%.o: $(SDIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling: $<"
	@$(CC) -c -o $@ $< $(CFLAGS)

# Embed resource files as binary data
$(ODIR)/resources_%.o: $(RDIR)/%
	@mkdir -p $(dir $@)
	@echo "Embedding resource: $<"
	@$(LD) -r -b binary -o "$@" "$<"
	@$(OBJCOPY) --rename-section .data=.rodata,alloc,load,readonly,data,contents --set-section-alignment .rodata=4096 "$@"
	@$(OBJCOPY) --add-section .note.GNU-stack=/dev/null --set-section-flags .note.GNU-stack=noload,readonly "$@"

$(ODIR):
	@mkdir -p $@

rebuild: clean all

clean:
	@echo "Cleaning project..."
	@rm -f $(TARGET_PATH) $(ELF_TARGET_PATH)
	@rm -rf $(ODIR)

# Prepare resources: build kernel module and compress it
prepare_resources:
	@mkdir -p $(RDIR)
	@echo "Compressing Kernel.elf with zlib..."
	@python3 ../../Scripts/create_zlib_blob.py "$(OUTPUT_DIR)/Kernel.elf" "$(RDIR)/Kernel.elf.compressed"
	# TODO Add other Kernel Modules here & plugins.

.PHONY: all clean rebuild prepare_resources