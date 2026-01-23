# Compiler and flags
NASM = nasm
NASM_FLAGS = -f bin

# Source and output
SRC = Bootstrapper.s
BUILD_DIR = ../../../Build
OUTPUT = $(BUILD_DIR)/Bootstrapper.bin

# Default target
all: $(OUTPUT)

# Build rule
$(OUTPUT): $(SRC)
	@mkdir -p $(BUILD_DIR)
	$(NASM) $(NASM_FLAGS) -o $(OUTPUT) $(SRC)
	@echo "Built: $(OUTPUT)"

# Clean target
clean:
	rm -f $(OUTPUT)
	@echo "Cleaned build artifacts"

# Rebuild target
rebuild: clean all

# Phony targets
.PHONY: all clean rebuild