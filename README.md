# toolchain68k
Build a toolchain for 68k cross developement

The script is for MSYS2

Instructions: *all commands are to be typed into the msys shell*

1. Install MSYS2 Base System and needed packages
  * download the MSYS2 Installer from the [MSYS Home Page](http://www.msys2.org/), use the 64 Bit Version
  * execute installer and chose the installation path
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
    * ```pacman -S git msys2-runtime tar flex bison diffutils texinfo mingw-w64-x86_64-gcc mingw-w64-x86_64-libmangle-git mingw-w64-x86_64-make mingw-w64-x86_64-pkg-config mingw-w64-x86_64-tools-git mingw-w64-x86_64-winstorecompat-git```
1. clone the git repo (use the msys shell)
  * ```git clone https://github.com/haarer/toolchain68k.git```
1. start the build script
  * ```sh   buildtoolchain.sh```
1. add the path
  * ```export PATH=$PATH:/opt/m68k/bin```
1. compile one of the examples
  * ```cd m68k-example2 ;  make ```

