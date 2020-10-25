#!/bin/bash
# Copyright (c) 2014-2020, A.Haarer, All rights reserved. LGPLv3


function test_build_result () {
  local TESTFILE=$1
  local NOCOLOR='\033[0m'
  local RED='\033[0;31m'
 if [ ! -f $TESTFILE ]; then
   echo -e "${RED}Build Error${NOCOLOR} : missing file $TESTFILE"
   exit 1
 fi 
}

TARGETARCHITECTURE=$1

export PATH=$PATH:`pwd`/toolchain-$TARGETARCHITECTURE-current/bin:`pwd`/toolchain-$TARGETARCHITECTURE-current/bin/$TARGETARCHITECTURE/bin

echo $PATH

case $TARGETARCHITECTURE in
"arm-none-eabi")
     ( cd examples/arm-none-eabi-example && \
       wget -nc https://www.st.com/content/ccc/resource/technical/software/firmware/48/ab/e5/17/0d/79/43/74/stsw-stm32054.zip/files/stsw-stm32054.zip/_jcr_content/translations/en.stsw-stm32054.zip &&\
       [ -d "STM32F10x_StdPeriph_Lib_V3.5.0/Libraries" ] || unzip en.stsw-stm32054.zip &&\
       make clean && make arm-test.bin )
    test_build_result "examples/arm-none-eabi-example/arm-test.bin" 
  ;;
"m68k-elf")
    ( cd examples/m68k-example && make clean && make files )
    test_build_result "examples/m68k-example/m68k-test.bin" 
    ( cd examples/m68k-example2 && make clean && make files )
    test_build_result "examples/m68k-example2/m68k-test.bin" 
  ;;
"avr")
    ( cd examples/avr-example && make clean && make avr-test.hex )
    test_build_result "examples/avr-example/avr-test.hex" 
    ( cd examples/avr-example2 && make clean && make avr-test.hex )
    test_build_result "examples/avr-example2/avr-test.hex"
    ( cd examples/avr-cpp-example && make clean && make avr-test.hex )
    test_build_result "examples/avr-cpp-example/avr-test.hex"
    ( cd examples/avr-cpp-example2 && make clean && make avr-c++test.hex )
    test_build_result "examples/avr-cpp-example2/avr-c++test.hex" 
  ;;
*)
  echo "architecture not supported $TARGETARCHITECTURE ! use one of arm-none-eabi, m68k-elf or avr"
  exit 1
  ;;
esac
