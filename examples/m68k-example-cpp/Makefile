TARGET := m68k-test

CC := m68k-elf-gcc
CXX := m68k-elf-g++
OBJCPY := m68k-elf-objcopy
SIZE := m68k-elf-size
OBJDUMP := m68k-elf-objdump
#INC += -I../../toolchain-m68k-elf-current/m68k-elf/include 
#INC += -I../../toolchain-m68k-elf-current/m68k-elf/include/c++/13.2.0 
#INC += -I../../toolchain-m68k-elf-current/m68k-elf/include/c++/13.2.0/m68k-elf 
#INC += -I../../toolchain-m68k-elf-current/lib/gcc/m68k-elf/13.2.0/include 

CFLAGS += -m68332 -I. $(INC) -D_REENT_SMALL -Wall -Os -g
CXXFLAGS += -m68332 -I. $(INC) -D_REENT_SMALL -Wall -Os -g -std=c++23

#CFLAGS += -m68332 -I. -DREENTRANT_SYSCALLS_PROVIDED -D_REENT_SMALL -Wall -O2 -g
#CXXFLAGS += -m68332 -I. -DREENTRANT_SYSCALLS_PROVIDED -D_REENT_SMALL -Wall -O2 -g -std=c++23
LFLAGS_RAM += -m68332 -g -nostartfiles  -Wl,--script=ram.ld,-Map=$(TARGET).map,--allow-multiple-definition 
LFLAGS_ROM += -m68332 -g -nostartfiles  -Wl,--script=rom.ld,-Map=$(TARGET).map,--cref,--allow-multiple-definition

LIBS += -lstdc++ 

# determine the object files
OBJ := main.o \
	appinit.o \
	glue.o \
	uart.o

AOBJ := crt0.o \
	vector.o

# link the program
$(TARGET).elf: $(OBJ) $(AOBJ) ram.ld
	@echo "---> link project to RAM ..."
	$(CC) $(AOBJ) $(OBJ) $(LIBS) $(LFLAGS_RAM) -o $@

# link the program to Flash
flash: $(AOBJ) $(OBJ) rom.ld
	@echo "---> link project to Flash..."
	$(CC) $(AOBJ) $(OBJ) $(LIBS) $(LFLAGS_ROM) -o $(TARGET).elf

# build rule for assembler files
%.o: %.S
	$(CC) -c $(AFLAGS) -Wa,-adhlns=$<.lst $< -o $@

# build rule for cpp
%.o: %.cpp
	$(CXX) -c $(CXXFLAGS)  $< -o $@

# build rule for c
%.o: %.c
	$(CC) -c $(CFLAGS)  $< -o $@



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

