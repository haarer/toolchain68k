#!/bin/sh

# install MSYS2 package requirements

pacman -S --needed git msys2-runtime unzip make tar flex bison diffutils texinfo patch mingw-w64-x86_64-gcc mingw-w64-x86_64-libmangle-git mingw-w64-x86_64-make mingw-w64-x86_64-pkg-config mingw-w64-x86_64-tools-git mingw-w64-x86_64-winstorecompat-git

## all after this is only needed if avrdude shall use usb devices

# pull additonal libraries
pacman -S --needed mingw-w64-x86_64-libusb mingw-w64-x86_64-libusb-compat-git mingw-w64-x86_64-libftdi

# nomally the avrdude should be updated to use the new libusb-1.0 but ...
# 
# quick and dirty hack: patch up msys  - there is an usb.h in the include path that shadows the usb.h
# from the libusb compatibility package - we move it out of the way and symlink the needed file..
if grep -q _WDMDDK_ /mingw64/x86_64-w64-mingw32/include/usb.h ; then
	mv  /mingw64/x86_64-w64-mingw32/include/usb.h /mingw64/x86_64-w64-mingw32/include/usb_go_away.h 
fi
ln -s  /mingw64/include/libusb-compat/usb.h /mingw64/x86_64-w64-mingw32/include/
