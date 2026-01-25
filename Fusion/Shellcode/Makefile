# Compiler and flags
NASM = nasm
NASM_FLAGS = -f bin

# Directories
KERNEL_DIR = Kernel
LOADER_DIR = Loader
KERNEL_BUILD_DIR = ../../Build/ShellCode
LOADER_BUILD_DIR = ../Loader/resources

# Find all .s files in respective directories
KERNEL_SOURCES = $(wildcard $(KERNEL_DIR)/*.s)
LOADER_SOURCES = $(wildcard $(LOADER_DIR)/*.s)

# Default target
all: build

build:
	@mkdir -p $(KERNEL_BUILD_DIR)
	@mkdir -p $(LOADER_BUILD_DIR)
	@echo "Building Kernel shellcode..."
	@for src in $(KERNEL_SOURCES); do \
		base=$$(basename $${src%.s}); \
		$(NASM) $(NASM_FLAGS) -o $(KERNEL_BUILD_DIR)/$${base}.bin $${src}; \
		echo "  Built: $(KERNEL_BUILD_DIR)/$${base}.bin"; \
	done
	@echo "Building Loader shellcode..."
	@for src in $(LOADER_SOURCES); do \
		base=$$(basename $${src%.s}); \
		$(NASM) $(NASM_FLAGS) -o $(LOADER_BUILD_DIR)/$${base}.bin $${src}; \
		echo "  Built: $(LOADER_BUILD_DIR)/$${base}.bin"; \
	done

# Clean target
clean:
	rm -f $(KERNEL_BUILD_DIR)/*.bin
	rm -f $(LOADER_BUILD_DIR)/*.bin
	@echo "Cleaned build artifacts"

# Rebuild target
rebuild: clean all

# Phony targets
.PHONY: all build clean rebuild