# c++ message handling project, assuming Arduino Mega on port COM3


TARGET := avr-c++test
CPU:= atmega2560


CC := avr-gcc
CXX := avr-g++
OBJCPY := avr-objcopy
SIZE := avr-size
OBJDUMP := avr-objdump

CFLAGS += -mmcu=$(CPU) -I. -I/opt/crosschain/avr/include -DF_CPU=16000000L -DREENTRANT_SYSCALLS_PROVIDED -D_REENT_SMALL -Wall -Os -std=gnu99 -g
CXXFLAGS += -mmcu=$(CPU) -I. -I/opt/crosschain/avr/include -DF_CPU=16000000L -DREENTRANT_SYSCALLS_PROVIDED -D_REENT_SMALL -Wall -Os -std=c++17 -g



LFLAGS += -mmcu=$(CPU) -g  -Wl,-Map=$(TARGET).map,--allow-multiple-definition


# determine the object files
OBJ := main.o uart.o


# link the program
$(TARGET).elf: $(OBJ) $(AOBJ) 
	@echo "---> link project ..."
	$(CC) $(AOBJ) $(OBJ) $(LIBS) $(LFLAGS) -o $@


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
	$(SIZE) -Ax $(TARGET).elf

# print target info
info: $(TARGET).elf
	$(OBJDUMP) --headers $<

flash: $(TARGET).hex
	/opt/crosschain/bin/avrdude -p$(CPU) -v -cwiring -PCOM3 -b115200 -C/opt/crosschain/etc/avrdude.conf -D -Uflash:w:$(TARGET).hex:i

