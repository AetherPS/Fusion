CC        	:=    g++
AS        	:=    g++
OBJCOPY   	:=    objcopy
ODIR    	:=    build
SDIR    	:=    source
IDIRS    	:=    -I../External/FusionShared -Iinclude
CFLAGS    	:=    $(IDIRS) -O3 -s -w -std=gnu++11 -ffunction-sections -fdata-sections -fno-builtin -fno-exceptions -fno-asynchronous-unwind-tables -nostdlib -w -masm=intel -march=btver2 -m64 -mabi=sysv -mcmodel=small -mstackrealign -fPIE -DVERSION_$(VERSION) -DKERNELDRIVER -D_KERNEL
LFLAGS    	:=    -Xlinker -T Linker -Wl,--build-id=none -mstackrealign -pie -Wl,--gc-sections
SFLAGS  	:=    -nostdlib -masm=intel -march=btver2 -m64 -mabi=sysv
CFILES    	:=    $(shell find $(SDIR) -name \*.cpp)
SFILES    	:=    $(shell find $(SDIR) -name \*.s)
OBJS	    :=	  $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(CFILES)) $(patsubst $(SDIR)/%.s, $(ODIR)/%.o, $(SFILES))

TARGET = $(shell basename "$(CURDIR)").elf

$(TARGET): $(ODIR) $(OBJS)
	$(CC) crt0.s $(OBJS) -o $(TARGET) $(CFLAGS) $(LFLAGS)

$(ODIR)/%.o: $(SDIR)/%.cpp
	mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/%.s
	$(AS) -c -o $@ $< $(SFLAGS)

$(ODIR):
	@mkdir $@

rebuild:
	$(MAKE) --no-print-directory clean
	$(MAKE) --no-print-directory all

clean:
	rm -f $(TARGET) $(ODIR)/*.o
