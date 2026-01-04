# Daemon targets
ELF_TARGET := Daemon.elf
OELF_TARGET := Daemon.oelf
EBOOT_TARGET := eboot.bin

# System process authentication
PAID := 0x3800000000010003
AUTHINFO := 070000000000003800000000001C004000FF000000000080000000000000000000000000000000000000008000400040000000000000008000000000000000080040FFFF000000F000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000

# Libraries linked into the ELF.
LIBS := -lc -lc++ -lkernel -lSceSysmodule -lSceSysCore

# External libraries
FUSIONDRIVER_LIB := ../../External/libFusionDriver/Build/libFusionDriverOOSDK.a

# Additional compile flags.
EXTRAFLAGS := -D_OOSDK -frtti -fexceptions

# Root vars
TOOLCHAIN := $(OO_PS4_TOOLCHAIN)

# Check if TOOLCHAIN is set
ifeq ($(TOOLCHAIN),)
$(error OO_PS4_TOOLCHAIN environment variable is not set. Please set it in WSL or configure WSLENV in Windows)
endif

SOLUTIONDIR := ../..
BUILDDIR := $(SOLUTIONDIR)/Build
INTDIR := x64/Debug

# Source files
CPPFILES := $(filter-out stdafx.cpp, $(wildcard *.cpp))

# Object files (including stdafx.o)
OBJS := $(INTDIR)/stdafx.o $(patsubst %.cpp, $(INTDIR)/%.o, $(CPPFILES))

# Define final C/C++ flags
CFLAGS := --target=x86_64-pc-freebsd12-elf -fPIC -funwind-tables -c $(EXTRAFLAGS) \
          -isysroot $(TOOLCHAIN) -isystem $(TOOLCHAIN)/include \
          -I../../External/libFusionDriver/Include \
          -I../../External/FusionShared
CXXFLAGS := $(CFLAGS) -isystem $(TOOLCHAIN)/include/c++/v1
LDFLAGS := -m elf_x86_64 -pie --script $(TOOLCHAIN)/link.x --eh-frame-hdr \
           -L$(TOOLCHAIN)/lib $(LIBS) $(TOOLCHAIN)/lib/crt1.o

# Create the intermediate and build directories if they don't already exist.
_unused := $(shell mkdir -p $(INTDIR))
_unused := $(shell mkdir -p $(BUILDDIR))

# Check for linux vs macOS and account for clang/ld path
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    CC := clang
    CCX := clang++
    LD := ld.lld
    CDIR := linux
endif
ifeq ($(UNAME_S),Darwin)
    CC := /usr/local/opt/llvm/bin/clang
    CCX := /usr/local/opt/llvm/bin/clang++
    LD := /usr/local/opt/llvm/bin/ld.lld
    CDIR := macos
endif

# Main target - fake-signed eboot
$(BUILDDIR)/$(EBOOT_TARGET): $(INTDIR)/$(ELF_TARGET)
	$(TOOLCHAIN)/bin/$(CDIR)/create-fself -in=$< -out=$(INTDIR)/$(OELF_TARGET) --eboot "$@" --paid $(PAID) -authinfo $(AUTHINFO)

# ELF binary target
$(INTDIR)/$(ELF_TARGET): $(OBJS)
	$(LD) $(OBJS) "$(FUSIONDRIVER_LIB)" -o $@ $(LDFLAGS)

# Compile stdafx.cpp (precompiled header)
$(INTDIR)/stdafx.o: stdafx.cpp stdafx.h
	$(CCX) $(CXXFLAGS) -o $@ $<

# Compile source files
$(INTDIR)/%.o: %.cpp stdafx.h
	$(CCX) $(CXXFLAGS) -include stdafx.h -o $@ $<

.PHONY: clean all
.DEFAULT_GOAL := all

all: $(BUILDDIR)/$(EBOOT_TARGET)

clean:
	rm -f $(BUILDDIR)/$(EBOOT_TARGET) $(INTDIR)/$(OELF_TARGET) $(INTDIR)/$(ELF_TARGET) $(OBJS)