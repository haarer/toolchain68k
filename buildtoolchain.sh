#!/bin/bash
# Copyright (c) 2014-2024, A.Haarer, All rights reserved. LGPLv3

# build m68k-elf or arm-none-eabi or avr cross toolchain based on gcc
# set the number of make jobs as appropriate for build machine
# set the path after installation : export PATH=/opt/crosschain/bin:$PATH

# tested on host platforms : msys2 64bit on windows 10, ubuntu 23.10 (mantic)

# usage :
#    pass operating system as first parameter "linux" or "windows"

# links:
#   http://www.mikrocontroller.net/articles/GCC_M68k
#   https://gcc.gnu.org/install/index.html
#   https://gcc.gnu.org/install/prerequisites.html
#   https://gcc.gnu.org/wiki/FAQ#configure
#   http://www.msys2.org/


# Tips on windows:
# please use the MSYS shell, not UCRT
#
# to speed up things:  chose a location without
# - indexing (dont use windows home dir )
# - virus scanner ( add exclusion, or stop scanner)
#
# chose a number of make jobs #number of cores + 1 to make use of all computer resources
#
# some sh versions dont like the () after functions - call the script using bash helps
#

#first parameter: operating system to build for
OS=$1

#second parameter : desired target architecture
TARGETARCHITECTURE=$2

#third parameter : desired action ()
ACTION=$3

# check parameters
if [[ "$OS" = "windows" || "$OS" = "linux" || "$OS" = "raspian" ]]; then
  echo ""
else
  echo "os not supported $OS ! use one of linux , windows or raspian"
  exit 1
fi

if [[ "$TARGETARCHITECTURE" = "arm-none-eabi" || "$TARGETARCHITECTURE" = "m68k-elf"  || "$TARGETARCHITECTURE" = "avr" ]]; then
  echo ""
else
  echo "architecture not supported $TARGETARCHITECTURE ! use one of arm-none-eabi, m68k-elf or avr"
  exit 1
fi



#define package versions
BINUTILS="binutils-2.42"
GCCVER="gcc-13.2.0"
AVRLIBVER="avr-libc-2.1.0"
NEWLIBVER="newlib-4.4.0.20231231"
GDBVER="gdb-14.2"

MPFRVER="mpfr-4.2.1"
GMPVER="gmp-6.3.0"
MPCVER="mpc-1.3.1"
ISLVER="isl-0.26"

#set the number of parallel makes
MAKEJOBS=16

# -------------------------------------- derived globals -----------------------------------
ROOTDIR=`pwd`
LOGFILE="$ROOTDIR/buildlog.txt"
HOSTINSTALLPATH="$ROOTDIR/toolchain-$TARGETARCHITECTURE-current"
PREREQPATH="$ROOTDIR/toolchain-prerequisites"

export PATH=$HOSTINSTALLPATH/bin:$PATH

# -------------------------------------- functions -----------------------------------------

NOCOLOR='\033[0m'
RED='\033[0;31m'
GREEN='\033[0;32m'

function log_msg () {
	local logline="`date` $1"
	echo -e "${GREEN}$logline${NOCOLOR}" >> $LOGFILE
	echo -e "${GREEN}$logline${NOCOLOR}"
}

function log_err () {
	local logline="`date` $1"
	echo -e "${RED}$logline${NOCOLOR}" >> $LOGFILE
	echo -e "${RED}$logline${NOCOLOR}"
}

function prepare_source () {
    local BASEURL=$1
    local SOURCENAME=$2
    local ARCHTYPE=$3

    pushd $ROOTDIR/cross-toolchain > /dev/null

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
        if [ ! -f $ROOTDIR/src-archives/$SOURCENAME.$ARCHTYPE ]; then
            log_msg " downloading $SOURCENAME"
            pushd $ROOTDIR/src-archives > /dev/null
            wget $BASEURL/$SOURCENAME.$ARCHTYPE
            popd > /dev/null
            log_msg " downloading $SOURCENAME finished"
        else
            log_msg " downloading $SOURCENAME skipped"
        fi
    fi

    if [ ! -d $SOURCENAME ]; then
        log_msg " unpacking $SOURCENAME"
        if [ "$ARCHTYPE" == "tar.bz2" ]; then
            tar -xjf ../src-archives/$SOURCENAME.$ARCHTYPE
        elif [ "$ARCHTYPE" = "tar.gz" ]; then
            tar -xzf ../src-archives/$SOURCENAME.$ARCHTYPE
        elif [ "$ARCHTYPE" = "tar.xz" ]; then
            tar -xJf ../src-archives/$SOURCENAME.$ARCHTYPE
        elif [ "$ARCHTYPE" = "zip" ]; then
            unzip ../src-archives/$SOURCENAME.$ARCHTYPE
        elif [ "$ARCHTYPE" = "git" ]; then
            echo "" #nothing to do for git
        else
            log_err " !!!!! unknown archive format"
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

    popd > /dev/null

}

function conf_compile_source () {
    local SOURCEPACKAGE=$1
    local DETECTFILE=$2
    local CONFIGURESTRING=$3

    [ ! -d $ROOTDIR/cross-toolchain/$SOURCEPACKAGE/cross-chain-$TARGETARCHITECTURE-obj ] && mkdir $ROOTDIR/cross-toolchain/$SOURCEPACKAGE/cross-chain-$TARGETARCHITECTURE-obj

    pushd $ROOTDIR/cross-toolchain/$SOURCEPACKAGE/cross-chain-$TARGETARCHITECTURE-obj > /dev/null

    log_msg "COMPILE sourcepackage= $SOURCEPACKAGE"
    log_msg "COMPILE detect file= $DETECTFILE"

    if [ ! -f config.status ]; then
        log_msg "configuring $SOURCEPACKAGE"
        ../configure $CONFIGURESTRING 2>&1 | tee -a $ROOTDIR/$SOURCEPACKAGE-$TARGETARCHITECTURE-conf.log || exit 1
        log_msg "configuring $SOURCEPACKAGE finished"
    else
        log_msg "configuring $SOURCEPACKAGE skipped"
    fi
    
    if [ ! -f $DETECTFILE ]; then

        log_msg "building $SOURCEPACKAGE"
        make -j $MAKEJOBS 2>&1 | tee -a $ROOTDIR/$SOURCEPACKAGE-$TARGETARCHITECTURE-build.log || exit 1
        if [ $? -eq 0 ]; then
            log_msg "building $SOURCEPACKAGE finished"
        else
            log_err "building $SOURCEPACKAGE failed"
        fi

        log_msg "install $SOURCEPACKAGE"
        install_package
        log_msg "install $SOURCEPACKAGE finished"
    else
        log_msg "compiling and install $SOURCEPACKAGE skipped"
    fi
    popd > /dev/null
}


#function to install a single package
function install_package () {
    make install 2>&1 | tee -a $ROOTDIR/$SOURCEPACKAGE-$TARGETARCHITECTURE-install.log
    if [ $? -eq 0 ]; then
        log_msg "install finished"
    else
        log_err "install failed"
    fi
}

#remove all intermediate products of a single package
function purge_pkg () {
    local PACKAGE=$1
    [ -d $ROOTDIR/cross-toolchain/$PACKAGE ] && rm -rf $ROOTDIR/cross-toolchain/$PACKAGE
    [ -d $ROOTDIR/toolchain-prerequisites/$PACKAGE ] && rm -rf $ROOTDIR/toolchain-prerequisites/$PACKAGE
}

function download_all_pkg () {
    [ ! -d  $ROOTDIR/cross-toolchain ] &&	mkdir  $ROOTDIR/cross-toolchain
    [ ! -d  $ROOTDIR/src-archives ] &&	mkdir  $ROOTDIR/src-archives

    prepare_source http://ftp.gnu.org/gnu/binutils  $BINUTILS tar.bz2

    if [ "$TARGETARCHITECTURE" = "avr" ]; then
    # consider to use the github releases directly, e.g. https://github.com/avrdudes/avr-libc/archive/refs/tags/avr-libc-2_1_0-release.tar.gz
        prepare_source http://download.savannah.gnu.org/releases/avr-libc $AVRLIBVER tar.bz2

        pushd $ROOTDIR/cross-toolchain/$BINUTILS > /dev/null
        if ! patch -R -p0 -f -s --dry-run <../../avr_binutils.patch >/dev/null ; then
          log_msg "patching binutils"
          patch -p0  <../../avr_binutils.patch            
        else
          log_msg "already patched binutils"
        fi
        popd > /dev/null
    else
        prepare_source ftp://sourceware.org/pub/newlib $NEWLIBVER tar.gz
     
        pushd $ROOTDIR/cross-toolchain/$NEWLIBVER > /dev/null
        if ! patch -R -p0 -f -s --dry-run <../../newlib.patch >/dev/null ; then
          log_msg "patching newlib to use ssize_t on libgloss read and write functions"
          patch  -p0  <../../newlib.patch
        else
          log_msg "already patched newlib"
        fi
        popd > /dev/null

    fi

    prepare_source ftp://ftp.gwdg.de/pub/misc/gcc/releases/$GCCVER $GCCVER tar.xz

    prepare_source http://ftp.gnu.org/gnu/gdb $GDBVER tar.xz

    prepare_source https://gmplib.org/download/gmp $GMPVER tar.xz 

    prepare_source https://www.mpfr.org/mpfr-current $MPFRVER tar.xz

    prepare_source https://www.multiprecision.org/downloads $MPCVER tar.gz

    prepare_source https://libisl.sourceforge.io $ISLVER tar.bz2

}


#---------------------------------------------------------------------------------
#build pio package
function make_pio_package () {

    #works only in bash
    PACKAGEVER=${GCCVER/#gcc-}

    if [[ $OS = windows* ]]; then
        EXECUTEABLESUFFIX=".exe"
        echo "on windows, copy msys2 dlls"
        for DLLFILE in msys-gcc_s-seh-1.dll msys-2.0.dll msys-stdc++-6.dll
        do
          cp  /usr/bin/$DLLFILE $HOSTINSTALLPATH/bin
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

    elif [[ $OS = raspian* ]]; then echo "on raspian"
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
    cd $HOSTINSTALLPATH ;tar czf ../toolchain-$TARGETARCHITECTURE-$OS-$GCCVER.tar.gz * ; cd ..
    sha1sum toolchain-$TARGETARCHITECTURE-$OS-$GCCVER.tar.gz

}

function build_gdb () {
#---------------------------------------------------------------------------------
#build gdb

log_msg ">>>> build gdb"



GDBFLAGS+=" --target=$TARGETARCHITECTURE \
            --prefix=$HOSTINSTALLPATH/ \
            --with-gmp=$PREREQPATH/$GMPVER \
            --with-mpfr=$PREREQPATH/$MPFRVER \
            --with-expat
 "


conf_compile_source $GDBVER "$HOSTINSTALLPATH/bin/$TARGETARCHITECTURE-gdb$EXECUTEABLESUFFIX" "$GDBFLAGS"
}


if [ "$ACTION" = "purge" ]; then
    rm -rf $HOSTINSTALLPATH
    rm $ROOTDIR/*.log
    purge_pkg $BINUTILS
    purge_pkg $GCCVER
    purge_pkg $AVRLIBVER
    purge_pkg $NEWLIBVER
    purge_pkg $GDBVER
    purge_pkg $GMPVER
    purge_pkg $ISLVER
    purge_pkg $MPCVER
    purge_pkg $MPFRVER
    exit 0
fi

if [ "$ACTION" = "download" ]; then
    download_all_pkg
    exit 0
fi

if [ "$ACTION" = "build_gdb" ]; then
    build_gdb
    exit 0
fi


if [ "$ACTION" = "package" ]; then
    make_pio_package
    exit 0
fi

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

[ ! -d  $ROOTDIR/cross-toolchain ] &&	mkdir  $ROOTDIR/cross-toolchain
cd $ROOTDIR/cross-toolchain


# dl all sources and patch them if necessary
download_all_pkg


log_msg ">>>> build gmp"
GMPFLAGS+=" --prefix=$PREREQPATH/$GMPVER"
conf_compile_source $GMPVER "$PREREQPATH/$GMPVER/lib/libgmp.a" "$GMPFLAGS"

log_msg ">>>> build isl"
ISLFLAGS+=" --prefix=$PREREQPATH/$ISLVER --with-gmp-prefix=$PREREQPATH/$GMPVER"
conf_compile_source $ISLVER "$PREREQPATH/$ISLVER/lib/libisl.a" "$ISLFLAGS"

log_msg ">>>> build mpfr"
MPFRFLAGS+=" --prefix=$PREREQPATH/$MPFRVER --with-gmp=$PREREQPATH/$GMPVER"
conf_compile_source $MPFRVER "$PREREQPATH/$MPFRVER/lib/libmpfr.a" "$MPFRFLAGS"

log_msg ">>>> build mpc"
MPCFLAGS+=" --prefix=$PREREQPATH/$MPCVER \
            --with-mpfr=$PREREQPATH/$MPFRVER \
            --with-gmp=$PREREQPATH/$GMPVER\
            "
conf_compile_source $MPCVER "$PREREQPATH/$MPCVER/lib/libmpc.a" "$MPCFLAGS"


#-------------------------------- BINUTILS --------------------------------------------------
# build binutils

log_msg ">>>> build binutils"

BINUTILSFLAGS+=" --target=$TARGETARCHITECTURE --prefix=$HOSTINSTALLPATH/" 

#LDFLAGS=-Wl,-rpath=$(ORIGIN)/usr/lib/binutils/avr/git,--enable-new-dtags

conf_compile_source $BINUTILS "$HOSTINSTALLPATH/bin/$TARGETARCHITECTURE-objcopy$EXECUTEABLESUFFIX" "$BINUTILSFLAGS"


#--------------------------------- GCC ------------------------------------------------
# build gcc

log_msg ">>>> build gcc"


#pushd $ROOTDIR/cross-toolchain/$GCCVER > /dev/null
#if [ ! -d gmp ]; then
#    log_msg "fetching gcc prerequisites"
#    ./contrib/download_prerequisites
#fi
#popd > /dev/null


GCCFLAGS+=" --target=$TARGETARCHITECTURE  \
            --prefix=$HOSTINSTALLPATH/    \
            --libexecdir=$HOSTINSTALLPATH/lib \
            --enable-languages=c,c++      \
            --enable-lto                  \
            --with-newlib                 \
            --with-gmp=$PREREQPATH/$GMPVER\
            --with-mpfr=$PREREQPATH/$MPFRVER\
            --with-isl=$PREREQPATH/$ISLVER\
            --with-mpc=$PREREQPATH/$MPCVER\
            --disable-shared              \
            --disable-decimal-float       \
            --disable-libmudflap          \
            --disable-libssp              \
            --disable-libgomp             \
            --disable-libquadmath         \
            --disable-libstdcxx-pch       \
            --disable-threads             \
            --disable-tls                 \
            --disable-nls                 \
            --with-gnu-as                 \
            --with-gnu-ld                 \
            --disable-checking            \
            --with-sysroot=$HOSTINSTALLPATH/$TARGETARCHITECTURE \
            --without-headers"

SOURCEPACKAGE=$GCCVER
CONFIGURESTRING=$GCCFLAGS


[ ! -d $ROOTDIR/cross-toolchain/$SOURCEPACKAGE/cross-chain-$TARGETARCHITECTURE-obj ] && mkdir $ROOTDIR/cross-toolchain/$SOURCEPACKAGE/cross-chain-$TARGETARCHITECTURE-obj

pushd $ROOTDIR/cross-toolchain/$SOURCEPACKAGE/cross-chain-$TARGETARCHITECTURE-obj > /dev/null

log_msg "CCS sourcepackage= $SOURCEPACKAGE=$GCCVER"
log_msg "CCS detect file= $DETECTFILE"
log_msg "CCS cfgstring $CONFIGURESTRING"

if [ ! -f config.status ]; then
    log_msg "configuring $SOURCEPACKAGE"
    ../configure $CONFIGURESTRING 2>&1 | tee -a $ROOTDIR/$SOURCEPACKAGE-$TARGETARCHITECTURE-conf.log || exit 1
    log_msg "configuring $SOURCEPACKAGE finished"
else
    log_msg "configuring $SOURCEPACKAGE skipped"
fi

if [ ! -f gcc/include/limits.h ]; then
    log_msg "building $SOURCEPACKAGE"
    make -j $MAKEJOBS all-gcc 2>&1 | tee -a $ROOTDIR/$SOURCEPACKAGE-$TARGETARCHITECTURE-build.log || exit 1
    if [ $? -eq 0 ]; then
        log_msg "building $SOURCEPACKAGE finished"
    else
        log_err "building $SOURCEPACKAGE failed"
    fi
else
    log_msg "building $SOURCEPACKAGE skipped"
fi

# to do : extract number from gccver
if [ ! -f $HOSTINSTALLPATH/lib/gcc/$TARGETARCHITECTURE/13.2.0/include/limits.h ]; then
    log_msg "install $SOURCEPACKAGE"
    make install-gcc 2>&1 | tee -a $ROOTDIR/$SOURCEPACKAGE-$TARGETARCHITECTURE-install.log || exit 1
    log_msg "install $SOURCEPACKAGE finished"
else
    log_msg "install $SOURCEPACKAGE skipped"
fi
popd > /dev/null


if [ "$TARGETARCHITECTURE" = "avr" ]; then
    LIBCVER=$AVRLIBVER
    #--------------------------------LIBC LIBAVR -------------------------------------------------
    #build avr libc

    log_msg ">>>> build avrlib"


    LIBCFLAGS+=" --host=avr --prefix=$HOSTINSTALLPATH/ "

    # configure script does not autodetect msys
    if [ "$OS" = "windows" ]; then
        LIBCFLAGS+=" --build=x86_64"
    fi

    conf_compile_source $AVRLIBVER "$HOSTINSTALLPATH/$TARGETARCHITECTURE/lib/libc.a" "$LIBCFLAGS"

else
    LIBCVER=$NEWLIBVER
    #--------------------------------LIBC NEWLIB -------------------------------------------------
    #build libc for other platforms

    log_msg ">>>> build newlib"

    LIBCFLAGS+=" --target=$TARGETARCHITECTURE \
                 --prefix=$HOSTINSTALLPATH/ \
                 --enable-newlib-reent-small \
                 --disable-malloc-debugging \
                 --enable-newlib-multithread \
                 --disable-newlib-io-float \
                 --disable-newlib-supplied-syscalls \
                 --disable-newlib-io-c99-formats \
                 --disable-newlib-mb \
                 --disable-newlib-atexit-alloc \
                 --disable-newlib-fvwrite-in-streamio  \
                 --disable-newlib-fseek-optimization   \
                 --disable-newlib-wide-orient          \
                 --disable-newlib-unbuf-stream-opt     \
                 --enable-target-optspace \
                 --enable-newlib-nano-malloc           \
                 --disable-shared \
                 --enable-lite-exit                    \
                 --enable-newlib-global-atexit         \
       			 --enable-newlib-global-stdio-streams \
                 --disable-nls                         \
                 --enable-newlib-nano-formatted-io     \
       			 --enable-newlib-retargetable-locking \
                 --enable-static \
                 --enable-newlib-nano-malloc \
                 --enable-fast-install"

    BASE_CPPFLAGS="-pipe"
    BASE_LDFLAGS=
    BASE_CFLAGS_FOR_TARGET="-pipe -ffunction-sections -fdata-sections"
    BASE_CXXFLAGS_FOR_TARGET="-pipe -ffunction-sections -fdata-sections -fno-exceptions"
    export CPPFLAGS="${BASE_CPPFLAGS-} ${CPPFLAGS-}"
    export LDFLAGS="${BASE_LDFLAGS-} ${LDFLAGS-}"
    export CFLAGS_FOR_TARGET="-g -Os ${BASE_CFLAGS_FOR_TARGET-} ${CFLAGS_FOR_TARGET-}"


    conf_compile_source $NEWLIBVER "$HOSTINSTALLPATH/$TARGETARCHITECTURE/lib/libc.a" "$LIBCFLAGS"

fi


#--------------------------------- GCC ------------------------------------------------
# build gcc

log_msg ">>>> build gcc stage 2"



GCCFLAGS+=" --target=$TARGETARCHITECTURE  \
            --prefix=$HOSTINSTALLPATH/    \
            --libexecdir=$HOSTINSTALLPATH/lib \
            --enable-languages=c,c++      \
            --enable-lto                  \
			--disable-libstdcxx-verbose \
            --with-gmp=$PREREQPATH/$GMPVER\
            --with-mpfr=$PREREQPATH/$MPFRVER\
            --with-isl=$PREREQPATH/$ISLVER\
            --with-mpc=$PREREQPATH/$MPCVER\
            --with-newlib                 \
            --disable-shared              \
            --disable-decimal-float       \
            --disable-libmudflap          \
            --disable-libssp              \
            --disable-libgomp             \
            --disable-libquadmath         \
            --disable-libstdcxx-pch       \
            --disable-threads             \
            --disable-tls                 \
            --disable-nls                 \
            --with-gnu-as                 \
            --with-gnu-ld                 \
            --disable-checking            \
			--with-headers=yes            \
            --with-sysroot=$HOSTINSTALLPATH/$TARGETARCHITECTURE "

SOURCEPACKAGE=$GCCVER
CONFIGURESTRING=$GCCFLAGS


[ ! -d $ROOTDIR/cross-toolchain/$SOURCEPACKAGE/cross-chain-$TARGETARCHITECTURE-obj2 ] && mkdir $ROOTDIR/cross-toolchain/$SOURCEPACKAGE/cross-chain-$TARGETARCHITECTURE-obj2

pushd $ROOTDIR/cross-toolchain/$SOURCEPACKAGE/cross-chain-$TARGETARCHITECTURE-obj2 > /dev/null

log_msg "CCS sourcepackage= $SOURCEPACKAGE=$GCCVER"
log_msg "CCS cfgstring $CONFIGURESTRING"

# must always be reconfigured (with headers now)
log_msg "configuring $SOURCEPACKAGE"
../configure $CONFIGURESTRING 2>&1 | tee -a $ROOTDIR/$SOURCEPACKAGE-$TARGETARCHITECTURE-config.2.log || { log_err "configuring $SOURCEPACKAGE failed"; exit 1;}
log_msg "configuring $SOURCEPACKAGE finished"


log_msg "building $SOURCEPACKAGE"
make -j $MAKEJOBS  2>&1 | tee -a $ROOTDIR/$SOURCEPACKAGE-$TARGETARCHITECTURE-build.2.log || { log_err "building $SOURCEPACKAGE failed"; exit 1;}
log_msg "building $SOURCEPACKAGE finished"

log_msg "install $SOURCEPACKAGE"
make install  2>&1 | tee -a $ROOTDIR/$SOURCEPACKAGE-$TARGETARCHITECTURE-install.2.log || { log_err "install $SOURCEPACKAGE failed";  exit 1;}
log_msg "install $SOURCEPACKAGE finished"

popd > /dev/null


build_gdb

make_pio_package



