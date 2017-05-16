TARGET := m68k-test

CC := m68k-elf-gcc
OBJCPY := m68k-elf-objcopy
SIZE := m68k-elf-size
OBJDUMP := m68k-elf-objdump

CFLAGS += -m68332 -I. -DREENTRANT_SYSCALLS_PROVIDED -D_REENT_SMALL -Wall -O0 -std=gnu99 -g
#LFLAGS += -m68332 -g -nostartfiles -Wl,--script=ram.ld,-Map=$(TARGET).map,--allow-multiple-definition
LFLAGS += -m68332 -g  -Wl,--script=bcc.ld,-Map=$(TARGET).map,--allow-multiple-definition

# determine the object files
OBJ := main.o 


# link the program
$(TARGET).elf: $(OBJ) $(AOBJ) ram.ld
	@echo "---> link project to RAM ..."
	$(CC) $(AOBJ) $(OBJ) $(LIBS) $(LFLAGS) -o $@


# create hex,srec and bin files from target
files: $(TARGET).elf
	@echo "---> convert to Intel HEX..."
	$(OBJCPY) -O ihex $(TARGET).elf $(TARGET).hex
	@echo "---> convert to Motorola S-Record..."
	$(OBJCPY) -O srec $(TARGET).elf $(TARGET).srec
	@echo "---> convert to binary image..."
	$(OBJCPY) -O binary $(TARGET).elf $(TARGET).bin

# clean project
clean:
	rm -f $(OBJ) $(AOBJ) *.hex *.srec *.bin *.elf *.map *~ *.lst

# calculate sizes for program
size: $(TARGET).elf
	$(SIZE) $(OBJ) $(AOBJ) $(TARGET).elf
	@echo ""
	$(SIZE) -Ax $(TARGET).elf

# print target info
info: $(TARGET).elf
	$(OBJDUMP) --headers $<

