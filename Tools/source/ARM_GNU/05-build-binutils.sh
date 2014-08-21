#!/bin/bash

# 
# YAGARTO toolchain                                                       
#                                                                            
# Copyright (C) 2006-2011 by Michael Fischer                                      
# Michael.Fischer@yagarto.de                                                 
#
# Copyright (C) 2011 by Broadcom Inc.
#                                                                            
# This program is free software; you can redistribute it and/or modify       
# it under the terms of the GNU General Public License as published by       
# the Free Software Foundation; either version 2 of the License, or          
# (at your option) any later version.                                        
#                                                                            
# This program is distributed in the hope that it will be useful,            
# but WITHOUT ANY WARRANTY; without even the implied warranty of             
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              
# GNU General Public License for more details.                               
#                                                                            
# You should have received a copy of the GNU General Public License          
# along with this program; if not, write to the Free Software                
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA 
# 

set -e

#---------------------------------------------------------------------------------
# Setup the environment
# 
# This is done by a separate script, because if you have close your shell
# you can set the environment again with this script only.
#---------------------------------------------------------------------------------

if [ "$OSTYPE" == "msys" ]
then
. _env-only.sh
else
source ./_env-only.sh
fi

#---------------------------------------------------------------------------------
# build and install binutils
#---------------------------------------------------------------------------------

echo "Extracting $BINUTILS_SRC"
tar -xjvf download/binutils-$BINUTILS_ZIP_VER.tar.bz2 || { echo "Error extracting binutils-"$BINUTILS_ZIP_VER; exit; }


echo
echo "Patching $BINUTILS_SRC"
patch -p1 -d $BINUTILS_SRC < patches/$BINUTILS_SRC.patch || { echo "Error patching "$BINUTILS_SRC; exit; }
echo

echo "Start of build:" > 05-temp.txt
date >> 05-temp.txt 

mkdir -p binutils-build
cd binutils-build

#use $HOSTTYPE to check for x86_64  vs i686
if [ "$OSTYPE" == "msys" ]; then
export CFLAGS=-D__USE_MINGW_ACCESS
if [ "$HOSTTYPE" == "x86_64" ]; then
export BINDIR=$prefix/bin/Win64
else
export BINDIR=$prefix/bin/Win32
fi
elif [ "$OSTYPE" == "linux-gnu" -o "$OSTYPE" == "linux" ]; then
if [ "$HOSTTYPE" == "x86_64" -o "$HOSTTYPE" == "x86_64-linux" ]; then
export BINDIR=$prefix/bin/Linux64
else
export BINDIR=$prefix/bin/Linux32
fi
elif [[ "$OSTYPE" == *darwin* ]]; then
export BINDIR=$prefix/bin/OSX
else
echo $OSTYPE
fi

echo BINDIR $BINDIR
../$BINUTILS_SRC/configure \
	--bindir=$BINDIR \
	--infodir=$(pwd)/junk \
	--mandir=$(pwd)/junk \
	--libdir=$(pwd)/junk \
	--datarootdir=$(pwd)/junk \
	--target=$target --prefix=$prefix \
	--enable-interwork --enable-multilib \
	--enable-lto \
	--disable-nls --disable-shared --disable-threads \
	--with-gcc --with-gnu-as --with-gnu-ld --disable-werror \
	|| { echo "Error configuring binutils"; exit 1; }
$MAKE configure-host
$MAKE LDFLAGS="-all-static" || { echo "Error building binutils"; exit 1; }
$MAKE install || { echo "Error installing binutils"; exit 1; }

cd ..

echo "End of build:" >> 05-temp.txt
date >> 05-temp.txt 
mv 05-temp.txt 05-ready.txt

