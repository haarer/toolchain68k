# simple led blink project, assuming Arduino Mega on port COM3



PORT:= COM3
#PORT:= /dev/ttyACM0
TARGET := avr-test
CPU:= atmega2560


CC := avr-gcc
OBJCPY := avr-objcopy
SIZE := avr-size
OBJDUMP := avr-objdump

CFLAGS += -mmcu=$(CPU) -I. -I/opt/crosschain/avr/include -DF_CPU=16000000L -DREENTRANT_SYSCALLS_PROVIDED -D_REENT_SMALL -Wall -O2 -std=gnu99 -g

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
	$(SIZE) --format avr --mcu $(CPU) $(TARGET).elf

# print target info
info: $(TARGET).elf
	$(OBJDUMP) --headers $<

flash: $(TARGET).hex
	/opt/crosschain/bin/avrdude -p$(CPU) -v -cwiring -P$(PORT) -b115200 -C/opt/crosschain/etc/avrdude.conf -D -Uflash:w:$(TARGET).hex:i

