#!/bin/bash
# Copyright (c) 2014, A.Haarer, All rights reserved. LGPLv3

# build 68k cross toolchain based on gcc
# set the number of make jobs as appropriate for build machine
# set the path after installation : export PATH=/opt/m68k/bin:$PATH

# tested on host platforms : msys2 64bit on windows 10, debian 8

# links:
#   http://www.mikrocontroller.net/articles/GCC_M68k
#   https://gcc.gnu.org/install/index.html
#   https://gcc.gnu.org/install/prerequisites.html
#   https://gcc.gnu.org/wiki/FAQ#configure
#   http://www.msys2.org/


# Tips:
# to speed up things:  chose a location without 
# - indexing (dont use windows home dir )
# - virus scanner ( add exclusion, or stop scanner)
# 
# chose a number of make jobs #number of cores + 1 to make use of all computer resources
#
# some sh versions dont like the () after functions - call the script using bash helps
#
# on linux the script asks for su privileges at start - they are kept during script runtime but used only for make install

LOGFILE="`pwd`/buildlog.txt"
#set the number of parallel makes
MAKEJOBS=3
TARGETARCHITECTURE=avr
#TARGETARCHITECTURE=m68k-elf
HOSTINSTALLPATH="/opt/crosschain"


export PATH=$HOSTINSTALLPATH/bin:$PATH

function determine_os () {
	if [ -f /etc/lsb-release ]; then
	    . /etc/lsb-release 
	    OS=$DISTRIB_ID
	    VER=$DISTRIB_RELEASE
	elif [ -f /usr/bin/lsb_release ]; then
	    OS=`/usr/bin/lsb_release -is`
	    VER=`/usr/bin/lsb_release -rs`
	elif [ -f /etc/debian_version ]; then
	    OS=Debian  # XXX or Ubuntu??
	    VER=$(cat /etc/debian_version)
	elif [ -f /etc/redhat-release ]; then
	    # TODO add code for Red Hat and CentOS here
	    ...
	else
	    OS=$(uname -s)
	    VER=$(uname -r)
	fi
}

function log_msg () {
	local logline="`date` $1"
	echo $logline >> $LOGFILE
	echo $logline
}

#todo, consider this
#fullfile="stuff.tar.gz"
#fileExt=${fullfile#*.}
#fileName=${fullfile%*.$fileExt}

function prepare_source () {
    local BASEURL=$1
    local SOURCENAME=$2
    local ARCHTYPE=$3

    if [ ! -f $SOURCENAME.$ARCHTYPE ]; then
        log_msg " downloading $SOURCENAME"
        wget $BASEURL/$SOURCENAME.$ARCHTYPE
        log_msg " downloading $SOURCENAME finished"
    else
        log_msg " downloading $SOURCENAME skipped"
    fi

    if [ ! -d $SOURCENAME ]; then
        log_msg " unpacking $SOURCENAME"
        if [ "$ARCHTYPE" == "tar.bz2" ]; then
            tar -xjf $SOURCENAME.$ARCHTYPE 
        elif [ "$ARCHTYPE" = "tar.gz" ]; then
            tar -xzf $SOURCENAME.$ARCHTYPE 
        elif [ "$ARCHTYPE" = "tar.xz" ]; then
            tar -xJf $SOURCENAME.$ARCHTYPE 
        elif [ "$ARCHTYPE" = "zip" ]; then
            unzip $SOURCENAME.$ARCHTYPE 
        else
            log_msg " !!!!! unknown archive format"
            exit 1
        fi
        log_msg " unpacking $SOURCENAME finished"
    else
        log_msg " unpacking $SOURCENAME skipped"
    fi
    cd $SOURCENAME

    if [ ! -d cross-chain-$TARGETARCHITECTURE-obj ]; then
        mkdir cross-chain-$TARGETARCHITECTURE-obj
    fi	
    cd cross-chain-$TARGETARCHITECTURE-obj

}

#function to install - requests privileges from user if os requires
# issues: 
# *  is the detection safe ?
#   
function install_package () {
    if [ $OS = "Debian" ] || [ $OS = "Fedora" ]; then
        sudo sh -c "export PATH=$PATH:$HOSTINSTALLPATH/bin; make install"
    else
        make install
    fi
}

function conf_compile_source () {
    local SOURCEPACKAGE=$1
    local DETECTFILE=$2
    local CONFIGURESTRING=$3

    if [ ! -f config.status ]; then
        log_msg "configuring $SOURCEPACKAGE"
        ../configure $CONFIGURESTRING || exit 1
        log_msg "configuring $SOURCEPACKAGE finished"
    else
        log_msg "configuring $SOURCEPACKAGE skipped"
    fi

    if [ ! -f $DETECTFILE ]; then

        log_msg "building $SOURCEPACKAGE"
        make -j $MAKEJOBS
        log_msg "building $SOURCEPACKAGE finished"

        log_msg "install $SOURCEPACKAGE"
        install_package
        log_msg "install $SOURCEPACKAGE finished"
    else
        log_msg "compiling and install $SOURCEPACKAGE skipped"
    fi

}

# -------------------------------------------------------------------------------------------
log_msg " start of buildscript"

determine_os
log_msg " building on OS: $OS $VER for target architecture $TARGETARCHITECTURE"
if [ "$OS" = "MINGW64_NT-10.0" ]; then
	EXECUTEABLESUFFIX=".exe"
	echo "ouuch.. on windows"
else
	echo "eeek.. not on windows, doing sudo keepalive tricks"
	EXECUTEABLESUFFIX=""
	sudo -v
	while true; do sudo -n true; sleep 60; kill -0 "$$" || exit; done 2>/dev/null &
fi

if [ ! -d cross-toolchain ]; then
	mkdir cross-toolchain
fi

cd cross-toolchain

M68KBUILD=`pwd`
echo "build path:" $M68KBUILD



#----------------------------------------------------------------------------------

log_msg ">>>> build binutils"
BINUTILS="binutils-2.25"

prepare_source http://ftp.gnu.org/gnu/binutils  $BINUTILS tar.bz2

conf_compile_source binutils "$HOSTINSTALLPATH/bin/$TARGETARCHITECTURE-objcopy$EXECUTEABLESUFFIX" "--target=$TARGETARCHITECTURE --prefix=$HOSTINSTALLPATH/" 

cd $M68KBUILD

#---------------------------------------------------------------------------------
# build gcc

log_msg ">>>> build gcc"
GCCVER="gcc-7.1.0"

prepare_source ftp://ftp.gwdg.de/pub/misc/gcc/releases/$GCCVER $GCCVER tar.bz2

if [ ! -d ../gmp ]; then
    log_msg "fetching gcc prerequisites"
    cd ..
    ./contrib/download_prerequisites
    cd m68k-obj
fi

conf_compile_source gcc "$HOSTINSTALLPATH/bin/$TARGETARCHITECTURE-gcov$EXECUTEABLESUFFIX" "--target=$TARGETARCHITECTURE --prefix=$HOSTINSTALLPATH/ --enable-languages=c,c++ --disable-bootstrap --with-newlib --disable-libmudflap --disable-libssp --disable-libgomp --disable-libstdcxx-pch --disable-threads --with-gnu-as --with-gnu-ld --disable-nls --with-headers=yes --disable-checking --without-headers"

cd $M68KBUILD

if [ "$TARGETARCHITECTURE" = "avr" ]; then
    #---------------------------------------------------------------------------------
    #build avr libc

    log_msg ">>>> build avrlib"
    LIBCVER="avr-libc-2.0.0"

    prepare_source http://download.savannah.gnu.org/releases/avr-libc $LIBCVER tar.bz2

    export PATH=$PATH:/opt/$TARGETARCHITECTURE/bin/

    conf_compile_source avrlib "$HOSTINSTALLPATH/$TARGETARCHITECTURE/lib/libc.a" "--host=avr --prefix=$HOSTINSTALLPATH/"

    cd $M68KBUILD

else

    #---------------------------------------------------------------------------------
    #build libc for other platforms

    log_msg ">>>> build newlib"
    LIBCVER="newlib-2.5.0"

    prepare_source ftp://sources.redhat.com/pub/newlib $LIBCVER tar.gz

    export PATH=$PATH:/opt/$TARGETARCHITECTURE/bin/

    conf_compile_source newlib "$HOSTINSTALLPATH/$TARGETARCHITECTURE/lib/mfidoa/softfp/libc.a" " --target=$TARGETARCHITECTURE --prefix=$HOSTINSTALLPATH/ --enable-newlib-reent-small --disable-malloc-debugging --enable-newlib-multithread --disable-newlib-io-float --disable-newlib-supplied-syscalls --disable-newlib-io-c99-formats --disable-newlib-mb --disable-newlib-atexit-alloc --enable-target-optspace --disable-shared --enable-static --enable-fast-install"

    cd $M68KBUILD
 
fi 

#erst mal ohne gdb

#---------------------------------------------------------------------------------
#build gdb
#sudo apt-get install ncurses-dev
GDBVER="gdb-7.12"

log_msg ">>>> build gdb"
prepare_source http://ftp.gnu.org/gnu/gdb $GDBVER tar.xz

conf_compile_source gdb "$HOSTINSTALLPATH/bin/$TARGETARCHITECTURE-gdb$EXECUTEABLESUFFIX" "--target=$TARGETARCHITECTURE --prefix=$HOSTINSTALLPATH/"

cd $M68KBUILD



# musasim bauen
# https://code.google.com/p/musasim/
#apt-get install libsdl2-dev
#apt-get install libargtable2-dev
#apt-get install libfontconfig-dev
#apt-get install libelf-dev

# dafür braucht man auch libsdl2 ttf die bei debian nicht im repo ist - suxx
#wget https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-2.0.12.tar.gz
#tar xvzf SDL2_ttf-2.0.12.tar.gz
#cd SDL2_ttf-2.0.12/
#./configure
#make
#sudo make install


#git clone https://code.google.com/p/musasim/
#cd muasim
#./bootstrap.sh
#cd musasim
## vorher in verzeichnis ui das makefile fixen, es fehlt `pkg-config --cflags SDL2_ttf` bei den cflags
#make 


if [ "$TARGETARCHITECTURE" = "avr" ]; then


    #---------------------------------------------------------------------------------
    #build avrdude.
    #   On windows builds, avrdude needs the libusb or libftdi for certain programmers
    #   but not for flashing an arduino using the usb cable using the avrdude wiring configuration.
    #   see make file of avr example

    log_msg ">>>> build avrdude"
    AVRDUDEVER="avrdude-6.3"

    prepare_source http://download.savannah.gnu.org/releases/avrdude $AVRDUDEVER tar.gz

    export PATH=$PATH:/opt/$TARGETARCHITECTURE/bin/

    conf_compile_source avrdude "$HOSTINSTALLPATH/$TARGETARCHITECTURE/bin/avrdude" " --prefix=$HOSTINSTALLPATH/"

    cd $M68KBUILD

fi
