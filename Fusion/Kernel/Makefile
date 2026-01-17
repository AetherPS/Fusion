CC        	:=    g++
OBJCOPY   	:=    objcopy
ODIR    	:=    build
SDIR    	:=    source
RI			:=	  $(shell find include -type d -not -path ./include -printf '-I%p ')
IDIRS    	:=    -I../../Shared -Iinclude -I../../External/FusionShared $(RI)
CFLAGS    	:=    $(IDIRS) -O2 -s -w -std=gnu++11 -ffunction-sections -fdata-sections -fno-builtin -fno-exceptions -fno-asynchronous-unwind-tables -nostdlib -w -masm=intel -march=btver2 -m64 -mabi=sysv -mcmodel=small -mstackrealign -D_KERNEL
LFLAGS    	:=    -Xlinker -T Linker -Wl,--build-id=none -mstackrealign -Wl,--gc-sections -nostdlib
CFILES    	:=    $(shell find $(SDIR) -name \*.cpp)
OBJS	    :=	  $(patsubst $(SDIR)/%.cpp, $(ODIR)/%.o, $(CFILES))
TARGET		=	  $(shell basename "$(CURDIR)").elf

ifeq ($(DEBUG), 1)
    CFLAGS += -DDEBUG
endif

$(TARGET): increment_version $(ODIR) $(OBJS)
	$(CC) crt0.s $(OBJS) -o $(TARGET) $(CFLAGS) $(LFLAGS)

$(ODIR)/%.o: $(SDIR)/%.cpp
	mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR):
	@mkdir $@

increment_version:
	@echo "Incrementing version..."
	@python3 ../../increment_version.py

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
	@echo "Compressing $(TARGET) with zlib..."
	@python3 ../../Scripts/create_zlib_blob.py $(TARGET) $(TARGET).compressed
	@echo "Copying compressed $(TARGET) to ../../Fusion/Loader/resources/"
	@mkdir -p ../../Fusion/Loader/resources/
	@cp -u $(TARGET).compressed ../../Fusion/Loader/resources/
	@echo "Done!"

clean:
	rm -f $(TARGET) $(TARGET).sym $(ODIR)/*.o

.PHONY: increment_version copy clean