#cortex m3


TARGET := arm-test
CPU:= cortex-m3

STMLIBDIR :=../STM32F10x_StdPeriph_Lib_V3.5.0/Libraries

CXX := arm-none-eabi-g++
CC := arm-none-eabi-gcc
OBJCPY := arm-none-eabi-objcopy
SIZE := arm-none-eabi-size
OBJDUMP := arm-none-eabi-objdump
GDB := arm-none-eabi-gdb

INCLUDES += -I.
INCLUDES += -I$(STMLIBDIR)/CMSIS/Include
INCLUDES += -I$(STMLIBDIR)/CMSIS/ST/STM32F10x/Include
INCLUDES += -I$(STMLIBDIR)/STM32F10x_StdPeriph_Driver/inc
INCLUDES += -I$(STMLIBDIR)/CMSIS/CM3/CoreSupport
INCLUDES += -I$(STMLIBDIR)/CMSIS/CM3/DeviceSupport/ST/STM32F10x

CDEFS :=-DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -DUSE_FULL_ASSERT -DDEBUG 

CXXFLAGS += -mcpu=$(CPU) -mthumb $(INCLUDES) $(CDEFS)  -Wall -Werror -O2 -std=c++17 -g -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics
CFLAGS += -mcpu=$(CPU) -mthumb $(INCLUDES) $(CDEFS) -Wall -Werror  -O2 -std=c99 -g


LFLAGS += -Os -Wl,--gc-sections -mcpu=$(CPU) -Tmem.ld -Tlibs.ld -Tsections.ld "-Wl,-Map,$(TARGET).map" "-Lldscripts" -mthumb -Wl,--cref -Wl,--check-sections -Wl,--gc-sections -Wl,--unresolved-symbols=report-all -Wl,--warn-common -Wl,--warn-section-align -Wl,--warn-unresolved-symbols 
LIBS += -lm -lgcc 

# determine the object files
OBJ := main.o stm32f10x_it.o system_stm32f10x.o $(STMLIBDIR)/STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.o $(STMLIBDIR)/STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.o $(STMLIBDIR)/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/startup_stm32f10x_md.o


# link the program
$(TARGET).elf: $(OBJ) $(AOBJ) 
	@echo "---> link project ..."
	$(CC) $(AOBJ) $(OBJ) $(LIBS) $(LFLAGS) -o $@


# build rule for assembler files
%.o: %.s
	$(CC) -c $(AFLAGS) -Wa,-adhlns=$<.lst $< -o $@

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

%.bin: %.elf
	arm-none-eabi-objcopy -O binary $< $@

flash: $(TARGET).bin
	 st-flash write $< 0x8000000

debugserver:
	st-util

gdbgui:
	/c/tools/python27/python.exe -m gdbgui --gdb $(GDB) --gdb-args="-se='$(TARGET).elf' --eval-command='target extended-remote localhost:4242'"

unpack:
	( cd .. && [ -d "STM32F10x_StdPeriph_Lib_V3.5.0/Libraries" ] || unzip ../binary_assets/en.stsw-stm32054.zip )