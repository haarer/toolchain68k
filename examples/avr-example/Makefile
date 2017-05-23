TARGET := avr-test
CPU:= atmega2560


CC := avr-gcc
OBJCPY := avr-objcopy
SIZE := avr-size
OBJDUMP := avr-objdump

CFLAGS += -mmcu=$(CPU) -I. -DREENTRANT_SYSCALLS_PROVIDED -D_REENT_SMALL -Wall -O0 -std=gnu99 -g

LFLAGS += -mmcu=$(CPU) -g  -Wl,-Map=$(TARGET).map,--allow-multiple-definition


# determine the object files
OBJ := main.o 


# link the program
$(TARGET).elf: $(OBJ) $(AOBJ) 
	@echo "---> link project ..."
	$(CC) $(AOBJ) $(OBJ) $(LIBS) $(LFLAGS) -o $@


# create hex,srec and bin files from target
files: $(TARGET).elf
	@echo "---> convert to Intel HEX..."
	$(OBJCPY) -O ihex $(TARGET).elf $(TARGET).hex
	@echo "---> convert to binary image..."
	$(OBJCPY) -O binary $(TARGET).elf $(TARGET).bin

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

