# toolchain68k
Build a toolchain for 68k cross developement using recent compilers and libraries
The build script is also supporting the avr target

The script can be used on Windows 10 with MSYS2, on Debian 8, and on Fedora 25


# Windows and MSYS
all commands are to be typed into the msys shell*
## Install MSYS2 Base System and needed packages
  * download the MSYS2 Installer from the [MSYS Home Page](http://www.msys2.org/), use the 64 Bit Version
  * execute installer and chose the installation path as follows
    * best on a SSD
    * will need approx 4 GB space
    * exclude it from the virus scanner
    * exclude it form the windows index service
    * short path without spaces
    * not on a FAT Filesystem
    * i use *C:\Tools\msys64*
  * update the base system as described on the [MSYS Home Page](http://www.msys2.org/)
    * ```pacman -Syu```
    * ok the questions, close window when ready
    * open MSYS Shell again(start menu, use 64bit version)
    * ```pacman -Su```
## clone the git repo
```
git clone https://github.com/haarer/toolchain68k.git
```
## Install the required packages for msys
```
sh install_req_pkg_msys.sh
```

# Debian 8
## Install the required packages for msys
```
sh install_req_pkg_debian.sh
```

# Fedora 25
## Install the required packages for msys
```
sh install_req_pkg_fedora.sh
```


# Building and Installing the Toolchain    
## set the wanted target architecture in the build script by commenting out the unwanted arch.
```
#TARGETARCHITECTURE=avr
TARGETARCHITECTURE=m68k-elf
```
Note that the toolchain is currently installed to /opt/m68k - i'll change that to something like /opt/crosstoolchain

## start the build script
```
sh   buildtoolchain.sh
```
## on linux, authorize sudo (needed for installing)

#Testing the Toolchain
## add the path
```export PATH=$PATH:/opt/m68k/bin```
## compile one of the examples
```
cd examples/m68k-example2
make 
```

