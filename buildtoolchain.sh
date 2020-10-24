#!/bin/bash
# Copyright (c) 2014-2020, A.Haarer, All rights reserved. LGPLv3

# build 68k cross toolchain based on gcc
# set the number of make jobs as appropriate for build machine
# set the path after installation : export PATH=/opt/crosschain/bin:$PATH

# tested on host platforms : msys2 64bit on windows 10, debian 10.6

# usage :
#    pass operating system as first parameter "linux" or "windows"

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

OS=$1

if [[ "$OS" = "windows" || "$OS" = "linux"  ]]; then
  echo ""
else
  echo "os not supported $OS ! use one of linux or windows"
  exit 1
fi

#set this to the desired target architecture
#TARGETARCHITECTURE=arm-none-eabi
#TARGETARCHITECTURE=m68k-elf
#TARGETARCHITECTURE=avr
TARGETARCHITECTURE=$2
if [[ "$TARGETARCHITECTURE" = "arm-none-eabi" || "$TARGETARCHITECTURE" = "m68k-elf"  || "$TARGETARCHITECTURE" = "avr" ]]; then
  echo ""
else
  echo "architecture not supported $TARGETARCHITECTURE ! use one of arm-none-eabi, m68k-elf or avr"
  exit 1
fi


LOGFILE="`pwd`/buildlog.txt"

#set the number of parallel makes
MAKEJOBS=16



HOSTINSTALLPATH="`pwd`/toolchain-$TARGETARCHITECTURE-current"


export CFLAGS='-O2 -pipe'
export CXXFLAGS='-O2 -pipe'
export LDFLAGS='-s'
export DEBUG_FLAGS=''


if [ "$TARGETARCHITECTURE" = "arm-none-eabi" ]; then
        MACHINEFLAGS="--with-cpu=cortex-m4 --with-cpu=cortex-m3 --with-fpu=fpv4-sp-d16 --with-float=hard --with-float=softfp 
-with-float=soft --with-mode=thumb"
	GCCFLAGS=$MACHINEFLAGS
	BINUTILSFLAGS=$MACHINEFLAGS
	LIBCFLAGS=$MACHINEFLAGS
	GDBFLAGS=$MACHINEFLAGS
fi



export PATH=$HOSTINSTALLPATH/bin:$PATH


function log_msg () {
	local logline="`date` $1"
	echo $logline >> $LOGFILE
	echo $logline
}


function prepare_source () {
    local BASEURL=$1
    local SOURCENAME=$2
    local ARCHTYPE=$3

    if [ "$ARCHTYPE" = "git" ]; then
        if [ ! -f $SOURCENAME.$ARCHTYPE ]; then
            log_msg " cloning $BASEURL"
            git clone $BASEURL
        else
            log_msg " pulling update from $BASEURL"
            cd $SOURCENAME
            git pull
        fi
    else
        if [ ! -f $SOURCENAME.$ARCHTYPE ]; then
            log_msg " downloading $SOURCENAME"
            wget $BASEURL/$SOURCENAME.$ARCHTYPE
            log_msg " downloading $SOURCENAME finished"
        else
            log_msg " downloading $SOURCENAME skipped"
        fi
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
        elif [ "$ARCHTYPE" = "git" ]; then
            echo "" #nothing to do for git
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

#function to install package
function install_package () {
    make install
}

function conf_compile_source () {
    local SOURCEPACKAGE=$1
    local DETECTFILE=$2
    local CONFIGURESTRING=$3

    log_msg "CCS sourcepackage= $SOURCEPACKAGE"
    log_msg "CCS detect file= $DETECTFILE"
    log_msg "CCS cfgstring $CONFIGURESTRING"

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


log_msg " building on OS: $OS $VER for target architecture $TARGETARCHITECTURE"
if [[ $OS = windows ]]; then
	EXECUTEABLESUFFIX=".exe"
	echo "ouch.. on windows"
else
	echo "not on windows"
	EXECUTEABLESUFFIX=""
fi

if [ ! -d cross-toolchain ]; then
	mkdir cross-toolchain
fi

cd cross-toolchain

M68KBUILD=`pwd`
echo "build path:" $M68KBUILD



#-------------------------------- BINUTILS --------------------------------------------------
# build binutils

log_msg ">>>> build binutils"
BINUTILS="binutils-2.35"

prepare_source http://ftp.gnu.org/gnu/binutils  $BINUTILS tar.bz2

if [ "$TARGETARCHITECTURE" = "avr" ]; then
	log_msg "patching binutils"
	tmpdir=`pwd`
	cd ..
	patch  -p0 -i $M68KBUILD/../avr_binutils.patch
	cd $tmpdir
fi

BINUTILSFLAGS+=" --target=$TARGETARCHITECTURE --prefix=$HOSTINSTALLPATH/" 

#LDFLAGS=-Wl,-rpath=$(ORIGIN)/usr/lib/binutils/avr/git,--enable-new-dtags

conf_compile_source binutils "$HOSTINSTALLPATH/bin/$TARGETARCHITECTURE-objcopy$EXECUTEABLESUFFIX" "$BINUTILSFLAGS"

cd $M68KBUILD

#--------------------------------- GCC ------------------------------------------------
# build gcc

log_msg ">>>> build gcc"
GCCVER="gcc-10.2.0"

prepare_source ftp://ftp.gwdg.de/pub/misc/gcc/releases/$GCCVER $GCCVER tar.xz

if [ ! -d ../gmp ]; then
    log_msg "fetching gcc prerequisites"
    cd ..
    ./contrib/download_prerequisites
    cd cross-chain-$TARGETARCHITECTURE-obj
fi

GCCFLAGS+=" --target=$TARGETARCHITECTURE --prefix=$HOSTINSTALLPATH/ --enable-languages=c,c++ --disable-bootstrap --with-newlib --disable-libmudflap --disable-libssp --disable-libgomp --disable-libstdcxx-pch --disable-threads --with-gnu-as --with-gnu-ld --disable-nls --with-headers=yes --disable-checking --without-headers"

conf_compile_source gcc "$HOSTINSTALLPATH/bin/$TARGETARCHITECTURE-gcov$EXECUTEABLESUFFIX" "$GCCFLAGS"

cd $M68KBUILD

if [ "$TARGETARCHITECTURE" = "avr" ]; then
    #--------------------------------LIBC LIBAVR -------------------------------------------------
    #build avr libc

    log_msg ">>>> build avrlib"
    LIBCVER="avr-libc-2.0.0"

    prepare_source http://download.savannah.gnu.org/releases/avr-libc $LIBCVER tar.bz2

    export PATH=$PATH:/opt/$TARGETARCHITECTURE/bin/

    LIBCFLAGS+=" --host=avr --prefix=$HOSTINSTALLPATH/"

    conf_compile_source avrlib "$HOSTINSTALLPATH/$TARGETARCHITECTURE/lib/libc.a" "$LIBCFLAGS"

    cd $M68KBUILD

else

    #--------------------------------LIBC NEWLIB -------------------------------------------------
    #build libc for other platforms

    log_msg ">>>> build newlib"
    LIBCVER="newlib-3.1.0"

    prepare_source ftp://sources.redhat.com/pub/newlib $LIBCVER tar.gz

    log_msg "patching newlib to automatically determine _LDBL_EQ_DBL"
    tmpdir=`pwd`
    cd ..
    echo `pwd`
    patch  -p1 -i $M68KBUILD/../newlib.patch
    cd $tmpdir


    export PATH=$PATH:/opt/$TARGETARCHITECTURE/bin/

    LIBCFLAGS+=" --target=$TARGETARCHITECTURE --prefix=$HOSTINSTALLPATH/ --enable-newlib-reent-small --disable-malloc-debugging --enable-newlib-multithread --disable-newlib-io-float --disable-newlib-supplied-syscalls --disable-newlib-io-c99-formats --disable-newlib-mb --disable-newlib-atexit-alloc --enable-target-optspace --disable-shared --enable-static --enable-fast-install"

    conf_compile_source newlib "$HOSTINSTALLPATH/$TARGETARCHITECTURE/lib/libc.a" "$LIBCFLAGS"

    cd $M68KBUILD

fi


#---------------------------------------------------------------------------------
#build gdb
GDBVER="gdb-10.1"

log_msg ">>>> build gdb"
prepare_source http://ftp.gnu.org/gnu/gdb $GDBVER tar.xz

GDBFLAGS+=" --target=$TARGETARCHITECTURE --prefix=$HOSTINSTALLPATH/"

conf_compile_source gdb "$HOSTINSTALLPATH/bin/$TARGETARCHITECTURE-gdb$EXECUTEABLESUFFIX" "$GDBFLAGS"

cd $M68KBUILD






#---------------------------------------------------------------------------------
#build pio package

#works only in bash
PACKAGEVER=${GCCVER/#gcc-}

if [[ $OS = MINGW* ]]; then EXECUTEABLESUFFIX=".exe" echo "on windows, copy mingw dlls"
for DLLFILE in libgmp-10.dll libiconv-2.dll libintl-8.dll libwinpthread-1.dll libexpat-1.dll
do
    cp  /mingw64/bin/$DLLFILE $HOSTINSTALLPATH/bin
done
cat >$HOSTINSTALLPATH/package.json <<EOFWINDOWSVARIANT
{
    "description": "$GCCVER $BINUTILS $LIBCVER $GDBVER",
    "name": "toolchain-$TARGETARCHITECTURE-current",
    "system": [
        "windows",
        "windows_amd64",
        "windows_x86"
    ],
    "url": "https://github.com/haarer/toolchain68k",
    "version": "$PACKAGEVER"
}
EOFWINDOWSVARIANT

elif [[ $OS = Rasp* ]]; then echo "on raspian"
cat >$HOSTINSTALLPATH/package.json <<EOFRASPIVARIANT
{
    "description": "$GCCVER $BINUTILS $LIBCVER $GDBVER",
    "name": "toolchain-$TARGETARCHITECTURE-current",
    "system": [
        "linux_armv6l",
        "linux_armv7l",
        "linux_armv8l"
    ],
    "url": "https://github.com/haarer/toolchain68k",
    "version": "$PACKAGEVER"
}
EOFRASPIVARIANT

else
echo "on linux"
cat >$HOSTINSTALLPATH/package.json <<EOFLINUXVARIANT
{
    "description": "$GCCVER $BINUTILS $LIBCVER $GDBVER",
    "name": "toolchain-$TARGETARCHITECTURE-current",
    "system": [
        "linux_x86_64"
    ],
    "url": "https://github.com/haarer/toolchain68k",
    "version": "$PACKAGEVER"
}
EOFLINUXVARIANT
fi

log_msg "packaging.."
cd $HOSTINSTALLPATH ;tar czf ../toolchain-$TARGETARCHITECTURE-$1-$GCCVER.tar.gz * ; cd ..
sha1sum toolchain-$TARGETARCHITECTURE-$1-$GCCVER.tar.gz


