# demonstrating hardware access by a template based mechanism
# assuming Arduino Mega on port COM3


TARGET := avr-test
CPU:= atmega2560


CXX := avr-gcc
OBJCPY := avr-objcopy
SIZE := avr-size
OBJDUMP := avr-objdump

CXXFLAGS += -mmcu=$(CPU) -I. -I/opt/crosschain/avr/include -DF_CPU=16000000L -DREENTRANT_SYSCALLS_PROVIDED -D_REENT_SMALL -Wall -Os -std=c++17 -g

LFLAGS += -mmcu=$(CPU) -g  -Wl,-Map=$(TARGET).map,--allow-multiple-definition


# determine the object files
OBJ := main.o


# link the program
$(TARGET).elf: $(OBJ) $(AOBJ) 
	@echo "---> link project ..."
	$(CXX) $(AOBJ) $(OBJ) $(LIBS) $(LFLAGS) -o $@


# create hex,srec and bin files from target
$(TARGET).hex: $(TARGET).elf
	@echo "---> convert to Intel HEX..."
	$(OBJCPY) -O ihex $(TARGET).elf $(TARGET).hex


# clean project
clean:
	rm -f $(OBJ) $(AOBJ) *.hex *.bin *.elf *.map *~ *.lst

# calculate sizes for program
size: $(TARGET).elf
	$(SIZE) $(OBJ) $(AOBJ) $(TARGET).elf
	@echo ""
	$(SIZE) --format avr --mcu $(CPU) $(TARGET).elf

# print target info and generate lss
info: $(TARGET).elf
	$(OBJDUMP) --headers $<
	$(OBJDUMP) -h -S -z $< > $@

flash: $(TARGET).hex
	/opt/crosschain/bin/avrdude -p$(CPU) -v -cwiring -PCOM8 -b115200 -C/opt/crosschain/etc/avrdude.conf -D -Uflash:w:$(TARGET).hex:i

