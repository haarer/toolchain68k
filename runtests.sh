#!/bin/bash
# Copyright (c) 2014-2020, A.Haarer, All rights reserved. LGPLv3

TARGETARCHITECTURE=$1

export PATH=$PATH:`pwd`/toolchain-$TARGETARCHITECTURE-current/bin:`pwd`/toolchain-$TARGETARCHITECTURE-current/bin/$TARGETARCHITECTURE/bin

echo $PATH

case $TARGETARCHITECTURE in
"arm-none-eabi")
    echo "no test for arm yet"
  ;;
"m68k-elf")
    ( cd examples/m68k-example && make files )
    ( cd examples/m68k-example2 && make files )
  ;;
"avr")
    ( cd examples/avr-example && make  )
    ( cd examples/avr-example2 && make  )
    ( cd examples/avr-cpp-example && make  )
    ( cd examples/avr-cpp-example2 && make  )
  ;;
*)
  echo "architecture not supported $TARGETARCHITECTURE ! use one of arm-none-eabi, m68k-elf or avr"
  exit 1
  ;;
esac
