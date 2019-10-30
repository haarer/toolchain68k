# toolchain68k
Main Goal:
Build a toolchain for 68k, avr and cortex-m3 and cortex-m4  cross developement using recent compilers and libraries.

This supports development for various m68k boards, Arduinos and STM32 Boards (STM32F411 Nucleo, 32F411EDiscovery, Blue Pill )

The used toolchain versions are
 * gcc 9.2.0
 * binutils 2.32 (with avr size patch for avr target)
 * gdb 8.3
 * newlib 3.1.0   (with a patch to determine if long double is the same size as double, needed for m68k-elf )
 * avr-libc 2.0.0
 * avrdude 6.3
 * texane/stlink (pulled from github)

The build script can build for the following target architectures
 * m68k-elf
 * avr
 * arm-none-eabi

The script works on Windows 10 with MSYS2 and on Debian 9. It may work on other platforms but i dont run any tests.

The toolchains are either installed into a directory or packaged into archives suitable for the platform ide.
The archives can be used with eclipse or with make files as well - just unpack them.

The releases section of this git repo contains pre built toolchains. This can be used as package repository for platformio and there is a platform definition referencing the toolchain packages for platformio use. See https://github.com/haarer/platform-atmelavr .

There are example projects for m68k and avr.

# Prepare Build Environment for Windows and MSYS
all commands are to be typed into the msys shell
## Installation of  MSYS2 Base System and needed packages
  * download the MSYS2 Installer from the [MSYS Home Page](http://www.msys2.org/), use the 64 Bit Version
  * execute installer and chose the installation path according to the following criteria
    * best performance will be on a SSD
    * ite will need approx 4 GB space
    * exclude it from the virus scanner
    * exclude it form the windows index service
    * use a short path without spaces
    * do not put it on a FAT Filesystem
    * i use *C:\Tools\msys64*
  * update the base system as described on the [MSYS Home Page](http://www.msys2.org/)
    * ```pacman -Syu```
    * ok the questions, close window when ready
    * open MSYS Shell again(start menu, use 64bit version)
    * ```pacman -Su```
  * install git
    * ```pacman -S  git```

Now clone the git repository.
```
git clone https://github.com/haarer/toolchain68k.git
```
Install the required packages for msys
```
cd toolchain68k
sh install_req_pkg_msys.sh
```

# Prepare Build Environment for Debian 8
First install git.
```
sudo apt-get install git
```
Now clone the git repository.
```
git clone https://github.com/haarer/toolchain68k.git
```
Install the required packages for msys.
```
cd toolchain68k
bash install_req_pkg_debian.sh
```

# Prepare Build Environment for Fedora
First install git.
```
sudo yum install git
```
Now clone the git repository.
```
git clone https://github.com/haarer/toolchain68k.git
```
Install the required packages for fedora.
```
sh install_req_pkg_fedora.sh
```

# Building and Installing of the Toolchain    
Set the wanted target architecture in the build script by commenting out the unwanted architecture. It is also possible to add Target specific build options, see the arm-none-eabi Target how to do this.
```
#TARGETARCHITECTURE=avr
TARGETARCHITECTURE=m68k-elf
```
Select the method of deployment - either as package or install.

```
DEPLOY=package
#DEPLOY=install
```
Start the build script.
```
bash   buildtoolchain.sh
```

The script obtains the sources, unpacks them and builds the tools of the toolchain.

The progress of the build is written to a logfile: ```buildlog.txt```

Note that the toolchain used to be /opt/crosschain, now the default is to create platformio toolchain packages. 

# Testing the Toolchain
Add the path to the binaries 

This depends on $HOSTINSTALLPATH in the script)

```
export PATH=$PATH:/opt/crosschain/bin
```

### compile one of the 68k toolchain examples
see the linker scripts called for the make file.
One example uses toolchain provided startup files and linkerscript (m68k-example2), the other one uses custom linker scripts and own startup files (m68k-example)

```
cd examples/m68k-example
make 
```


### compile the simple avr toolchain example
This example assumes an arduino mega connected to usb (see makefile and change com port accordingly) and is a simple led blink example.
```
cd examples/avr-example-example
make 
make flash
```

### compile the simple arm-none-eabi toolchain example
This example assumes blue pill STM32F103C8 board  connected via a STLINK-2 adapter to usb (see makefile ) and is a simple led blink example.
It requires the [ST Microelectronics Std_Peripheral Lib][1] unpacked into the example directory
```
cd examples/arm-none-eabi
make 
make flash
```

  [1]: https://www.st.com/content/ccc/resource/technical/software/firmware/48/ab/e5/17/0d/79/43/74/stsw-stm32054.zip/files/stsw-stm32054.zip/_jcr_content/translations/en.stsw-stm32054.zip
