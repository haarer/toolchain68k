#!/bin/sh

# install MSYS2 package requirements

pacman -S --needed git msys2-runtime unzip make tar flex bison diffutils texinfo mingw-w64-x86_64-gcc mingw-w64-x86_64-libmangle-git mingw-w64-x86_64-make mingw-w64-x86_64-pkg-config mingw-w64-x86_64-tools-git mingw-w64-x86_64-winstorecompat-git
pacman -S --needed mingw-w64-x86_64-libusb 1.0.21-1 mingw-w64-x86_64-libusb-compat-git r60.072a5e4-3
