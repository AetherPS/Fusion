CC		:=	gcc
OBJCOPY	:=	objcopy
ODIR	:=	build
SDIR	:=	source
RDIR	:=  resources
IDIRS	:=  -I../../Shared -Iinclude -I../../External/FusionShared
CFLAGS	:=	$(IDIRS) -Os -std=gnu11 -ffunction-sections -fdata-sections -fno-builtin -nostartfiles -nostdlib -Wall -masm=intel -march=btver2 -mtune=btver2 -m64 -mabi=sysv -mcmodel=small -fpie -DSOFTWARE_VERSION=$(SOFTWARE_VERSION) -D_LOADER
LFLAGS	:=	-Xlinker -T Linker -Wl,--build-id=none -Wl,--gc-sections
CFILES	:=	$(wildcard $(SDIR)/*.c)
RFILES  :=  $(shell find $(RDIR) -name \* -type f 2>/dev/null)
OBJS	:=	$(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(CFILES)) $(patsubst $(RDIR)/%, $(ODIR)/%.o, $(RFILES))

ifneq ($(wildcard $(RDIR)/eboot.bin.compressed), )
	CFLAGS += -DHAS_DAEMON
endif

TARGET = Fusion-$(SOFTWARE_VERSION).bin
ELF_TARGET = Fusion-$(SOFTWARE_VERSION).elf

$(ELF_TARGET): $(ODIR) $(OBJS)
	$(CC) crt0.s $(ODIR)/*.o -o $(ELF_TARGET) $(CFLAGS) $(LFLAGS)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(RDIR)/%
	@mkdir -p $(dir $@)
	@ld -r -b binary -o "$@" "$<"

$(ODIR):
	@mkdir $@

.PHONY: clean copy

clean:
	@echo "Cleaning project files..."
	rm -f Fusion-*.bin Fusion-*.elf $(ODIR)/*.o

copy: $(ELF_TARGET)
	@echo "Converting to binary..."
	@objcopy -O binary $(ELF_TARGET) $(TARGET)
	@echo "Copying to build directory..."
	@mkdir -p ../../Build/
	@cp -u $(TARGET) ../../Build/
	@echo "Done!"