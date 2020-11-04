#!/bin/bash
# Copyright (c) 2014-2020, A.Haarer, All rights reserved. LGPLv3

#clone pio platform repo , patch example project inis to use the toolchain directory and build examples


function test_build_result () {
  local TESTFILE=$1
  local NOCOLOR='\033[0m'
  local GREEN='\033[0;32m'
  local RED='\033[0;31m'
 if [ ! -f $TESTFILE ]; then
   echo -e "${RED}Build Error${NOCOLOR} : missing file $TESTFILE"
   exit 1
 else
   echo -e "${GREEN}Build Successful${NOCOLOR} : $TESTFILE"
 fi 
}

TARGETARCHITECTURE=$1
PIOTESTS=platformio_tests
TESTLOG=$PIOTESTS/testlog.txt

[[ ! -d $PIOTESTS ]] && mkdir $PIOTESTS

#tbd:
#on arm the zephyr framework needs to be patched, not to require the toolchain "" but "toolchain-arm-none-eabi-current"
# line 84 ~/.platformio/packages/framework-zephyr/scripts/platformio/platformio-build.py

#the ~/.platformio/platforms/ststm32/platform.py needs to be patched not to require those versions (remove lines 43  and 50)
#            self.packages['toolchain-gccarmnoneeabi']['version'] = "~1.60301.0"
#            self.packages['toolchain-gccarmnoneeabi']['version'] = "~1.80201.0"
#
#sed -i "/self.packages\['toolchain-gccarmnoneeabi'\]\['version'\]/d" ~/.platformio/platforms/ststm32/platform.py
case $TARGETARCHITECTURE in
"arm-none-eabi")
#in order to patch the platform and the zephyr framework to use the new toolchain
    pio platform install https://github.com/haarer/platform-ststm32.git  --with-package framework-zephyr --skip-default-package
    sed -i "/self.packages\['toolchain-gccarmnoneeabi'\]\['version'\]/d" ~/.platformio/platforms/ststm32/platform.py
    sed -i "s/toolchain-gccarmnoneeabi/toolchain-arm-none-eabi-current/g" ~/.platformio/packages/framework-zephyr/scripts/platformio/platformio-build.py
    ( cd $PIOTESTS && [[ ! -d platform-ststm32 ]] && git clone https://github.com/haarer/platform-ststm32.git )
    for ex in `ls $PIOTESTS/platform-ststm32/examples` 
    do
      sed -i -e "/^platform/a\\platform_packages = haarer/toolchain-arm-none-eabi-current @ file://../../../../toolchain-arm-none-eabi-current" $PIOTESTS/platform-ststm32/examples/$ex/platformio.ini
      echo "TESTING $ex" >>$TESTLOG
      pio run -d $PIOTESTS/platform-ststm32/examples/$ex &>>$TESTLOG
    done
  ;;
"m68k-elf")
    ( cd $PIOTESTS && [[ ! -d platform-m68k ]] && git clone https://github.com/haarer/platform-m68k.git )
    for ex in `ls $PIOTESTS/platform-m68k/examples` 
    do
      sed -i -e "/^platform/a\\platform_packages = haarer/toolchain-m68k-elf-current @ file://../../../../toolchain-m68k-elf-current" $PIOTESTS/platform-m68k/examples/$ex/platformio.ini
      echo "TESTING $ex" >>$TESTLOG
      pio run -d $PIOTESTS/platform-m68k/examples/$ex &>>$TESTLOG
    done
  ;;
"avr")
    ( cd $PIOTESTS && [[ ! -d platform-atmelavr ]] && git clone https://github.com/haarer/platform-atmelavr.git )
    for ex in `ls $PIOTESTS/platform-atmelavr/examples` 
    do
      sed -i -e "/^platform/a\\platform_packages = haarer/toolchain-avr-current @ file://../../../../toolchain-avr-current" $PIOTESTS/platform-atmelavr/examples/$ex/platformio.ini
      echo "TESTING $ex" >>$TESTLOG
      pio run -d $PIOTESTS/platform-atmelavr/examples/$ex &>>$TESTLOG
    done
  ;;
*)
  echo "architecture not supported $TARGETARCHITECTURE ! use one of arm-none-eabi, m68k-elf or avr"
  exit 1
  ;;
esac

echo "------- SUMMARY -------"
egrep -e '\[SUCC|Process|\[FAIL|TESTING' $TESTLOG