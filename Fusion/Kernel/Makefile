CC        	:=    g++
OBJCOPY   	:=    objcopy
ODIR    	:=    build
SDIR    	:=    source
RI			:=	  $(shell find include -type d -not -path ./include -printf '-I%p ')
IDIRS    	:=    -I../../Shared -Iinclude -I../../External/FusionShared $(RI)
CFLAGS    	:=    $(IDIRS) -O3 -s -w -std=gnu++11 -ffunction-sections -fdata-sections -fno-builtin -fno-exceptions -fno-asynchronous-unwind-tables -nostdlib -w -masm=intel -march=btver2 -m64 -mabi=sysv -mcmodel=small -mstackrealign -DSOFTWARE_VERSION=$(SOFTWARE_VERSION) -D_KERNEL
LFLAGS    	:=    -Xlinker -T Linker -Wl,--build-id=none -mstackrealign -Wl,--gc-sections -nostdlib
CFILES    	:=    $(shell find $(SDIR) -name \*.cpp)
OBJS	    :=	  $(patsubst $(SDIR)/%.cpp, $(ODIR)/%.o, $(CFILES))
TARGET = $(shell basename "$(CURDIR)").elf

$(TARGET): $(ODIR) $(OBJS)
	$(CC) crt0.s $(OBJS) -o $(TARGET) $(CFLAGS) $(LFLAGS)

$(ODIR)/%.o: $(SDIR)/%.cpp
	mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR):
	@mkdir $@

copy:
	@echo "Extracting debug symbols..."
	@objcopy --only-keep-debug $(TARGET) $(TARGET).sym
	@echo "Stripping binary..."
	@strip --strip-all $(TARGET)
	@echo "Adding debug link..."
	@objcopy --add-gnu-debuglink=$(TARGET).sym $(TARGET)
	@echo "Copying files to ../../Build/"
	@mkdir -p ../../Build/
	@cp -u $(TARGET) ../../Build/
	@cp -u $(TARGET).sym ../../Build/
	@echo "Copying $(TARGET) to ../../Fusion/Loader/resources/"
	@mkdir -p ../../Fusion/Loader/resources/
	@cp -u $(TARGET) ../../Fusion/Loader/resources/
	@echo "Done!"

clean:
	rm -f $(TARGET) $(TARGET).sym $(ODIR)/*.o

.PHONY: copy clean