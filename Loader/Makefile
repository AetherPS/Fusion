CC		:=	gcc
AS		:=	gcc
OBJCOPY	:=	objcopy
ODIR	:=	build
SDIR	:=	source
RDIR	:=  resources
IDIRS	:=  -I../Shared -Iinclude -I../External/FusionShared
CFLAGS	:=	$(IDIRS) -Os -std=gnu11 -ffunction-sections -fdata-sections -fno-builtin -nostartfiles -nostdlib -Wall -masm=intel -march=btver2 -mtune=btver2 -m64 -mabi=sysv -mcmodel=small -fpie -DSOFTWARE_VERSION=$(SOFTWARE_VERSION)
SFLAGS	:=	-nostartfiles -nostdlib -march=btver2 -mtune=btver2
LFLAGS	:=	-Xlinker -T Linker -Wl,--build-id=none -Wl,--gc-sections
CFILES	:=	$(wildcard $(SDIR)/*.c)
SFILES	:=	$(wildcard $(SDIR)/*.s)
RFILES  :=  $(shell find $(RDIR) -name \*)
OBJS	:=	$(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(CFILES)) $(patsubst $(SDIR)/%.s, $(ODIR)/%.o, $(SFILES)) $(patsubst $(RDIR)/%, $(ODIR)/%.o, $(RFILES))

TARGET = Fusion-$(SOFTWARE_VERSION).bin

$(TARGET): $(ODIR) $(OBJS)
	$(CC) crt0.s $(ODIR)/*.o -o $(TARGET) $(CFLAGS) $(LFLAGS)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/%.s
	$(AS) -c -o $@ $< $(SFLAGS)

$(ODIR)/%.o: $(RDIR)/%
	echo $@ $<
	mkdir -p $(dir $@)
	ld -r -b binary -o $@ $<
	objdump -t $@

$(ODIR):
	@mkdir $@

.PHONY: clean

clean:
	@echo "Cleaning project files..."
	rm -f **Fusion-*.bin** $(ODIR)/*.o

copy:
	@echo "Copying build file..."
	objcopy -O binary $(TARGET) $(TARGET)