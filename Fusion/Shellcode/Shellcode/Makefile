# Compiler and flags
NASM = nasm
NASM_FLAGS = -f bin

# Directories
BUILD_DIR = ../../../Build/ShellCode

# Find all .s files in current directory
SOURCES = $(wildcard *.s)
BINS = $(SOURCES:.s=.bin)

# Default target
all: build

build:
	@mkdir -p $(BUILD_DIR)
	@for src in $(SOURCES); do \
		base=$${src%.s}; \
		$(NASM) $(NASM_FLAGS) -o $(BUILD_DIR)/$${base}.bin $${src}; \
		echo "Built: $(BUILD_DIR)/$${base}.bin"; \
	done

# Clean target
clean:
	rm -f $(BUILD_DIR)/*.bin
	@echo "Cleaned build artifacts"

# Rebuild target
rebuild: clean all

# Phony targets
.PHONY: all build clean rebuild