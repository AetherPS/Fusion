CC        	:=    g++
AS        	:=    g++
OBJCOPY   	:=    objcopy
ODIR    	:=    build
SDIR    	:=    source
RI			:=	  $(shell find include -type d -not -path ./include -printf '-I%p ')
IDIRS    	:=    -I../../Shared -Iinclude -I../../External/FusionShared $(RI)
CFLAGS    	:=    $(IDIRS) -O3 -s -w -std=gnu++11 -ffunction-sections -fdata-sections -fno-builtin -fno-exceptions -fno-asynchronous-unwind-tables -nostdlib -w -masm=intel -march=btver2 -m64 -mabi=sysv -mcmodel=small -mstackrealign -fPIE -DSOFTWARE_VERSION=$(SOFTWARE_VERSION) 
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
	mkdir -p $(dir $@)
	$(AS) -c -o $@ $< $(SFLAGS)

$(ODIR):
	@mkdir $@

rebuild:
	$(MAKE) --no-print-directory clean
	$(MAKE) --no-print-directory all

clean:
	rm -f $(TARGET) $(ODIR)/*.o
