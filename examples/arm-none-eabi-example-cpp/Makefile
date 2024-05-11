#cortex m3


TARGET := arm-test
CPU:= cortex-m3

#STMLIBDIR :=../STM32F10x_StdPeriph_Lib_V3.5.0/Libraries

CXX := arm-none-eabi-g++
CC := arm-none-eabi-gcc
OBJCPY := arm-none-eabi-objcopy
SIZE := arm-none-eabi-size
OBJDUMP := arm-none-eabi-objdump
GDB := arm-none-eabi-gdb

INCLUDES += -I.
#INCLUDES += -I$(STMLIBDIR)/CMSIS/Include
#INCLUDES += -I$(STMLIBDIR)/CMSIS/ST/STM32F10x/Include
#INCLUDES += -I$(STMLIBDIR)/STM32F10x_StdPeriph_Driver/inc
#INCLUDES += -I$(STMLIBDIR)/CMSIS/CM3/CoreSupport
#INCLUDES += -I$(STMLIBDIR)/CMSIS/CM3/DeviceSupport/ST/STM32F10x

CDEFS :=-DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -DUSE_FULL_ASSERT -DDEBUG 

CXXFLAGS += -mcpu=$(CPU) -mthumb $(INCLUDES) $(CDEFS)  -Wall -Werror -Os -std=c++17 -g -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics
CFLAGS += -mcpu=$(CPU) -mthumb $(INCLUDES) $(CDEFS) -Wall -Werror  -Os -std=c99 -g

#LFLAGS += -O0 -Wl,--gc-sections -mcpu=$(CPU) -Ttt.ld "-Wl,-Map,$(TARGET).map" "-Lldscripts" -mthumb -Wl,--cref -Wl,--check-sections -Wl,--gc-sections -Wl,--unresolved-symbols=report-all -Wl,--warn-common -Wl,--warn-section-align -Wl,--warn-unresolved-symbols 

LFLAGS += -Os -mcpu=$(CPU) -Tmem.ld -Tlibs.ld -Tsections.ld  "-Wl,-Map,build/$(TARGET).map" "-Lldscripts" -mthumb -Wl,--cref -Wl,--check-sections -Wl,--gc-sections -Wl,--unresolved-symbols=report-all -Wl,--warn-common -Wl,--warn-section-align -Wl,--warn-unresolved-symbols 
LIBS += -lm -lgcc -lstdc++

# determine the object files
OBJ := build/main.o \
	build/uart.o \
	build/glue.o \
	build/system.o \
	build/startup.o 


# link the program
build/$(TARGET).elf: $(OBJ) $(AOBJ) 
	@echo "---> link project ..."
	$(CC) $(AOBJ) $(OBJ) $(LIBS) $(LFLAGS) -o $@

# build rule for assembler files
build/%.o: %.s
	$(CC) -c $(AFLAGS) -Wa,-adhlns=$@.lst $< -o $@

# build rule for cpp
build/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -Wa,-adhlns=$@.lst $< -o $@

# build rule for c
build/%.o: %.c
	$(CC) -c $(CFLAGS) -Wa,-adhlns=$@.lst $< -o $@


# clean project
clean:
	rm -f $(OBJ) $(AOBJ) *.hex *.bin build/*.elf build/*.map *~ build/*.lst

# calculate sizes for program
size: build/$(TARGET).elf
	$(SIZE) build/$(OBJ) $(AOBJ) $<
	@echo ""
	$(SIZE) --format avr --mcu $(CPU) $<

# print target info and generate lss
info: build/$(TARGET).elf
	$(OBJDUMP) --headers $<
	$(OBJDUMP) -h -S -z $< > $@

%.bin: %.elf
	arm-none-eabi-objcopy -O binary $< $@
	arm-none-eabi-objdump -d $< >$(TARGET).dis

flash: build/$(TARGET).bin
	st-flash write $< 0x8000000

debugserver:
	st-util

gdbgui:
	python -m gdbgui --gdb $(GDB) --gdb-args="-se='$(TARGET).elf' --eval-command='target extended-remote localhost:4242'"

unpack:
	( cd .. && [ -d "STM32F10x_StdPeriph_Lib_V3.5.0/Libraries" ] || unzip ../binary_assets/en.stsw-stm32054.zip )
	( cd .. && [ -d "stlink-1.8.0-win32" ] || unzip ../binary_assets/stlink-1.8.0-win32.zip )
