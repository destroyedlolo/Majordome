#!/bin/bash

#this script is used to create a Makefile

# configuration

# Optional features
# Uncomment to enable them

# PostgreSQL database access
# needs postgresql-libs installed
BUILD_PGSQL='-DPGSQL -DDBASE'

# Graphical extension : Toile
# Not yet !
# BUILD_TOILE='-DTOILE'

# Enable debugging messages
# DEBUG='-DDEBUG'

# Environment discovering

# Hardcode test Lua version
# Development purpose only or if pkg-config doesn't work
#
#	LUA_DIR=/home/laurent/Projets/lua-5.3.4/install
#	LUA="-isystem $LUA_DIR/include"
#	LUALIB="-L$LUA_DIR/lib"
#
# If used, uncomment the lines above and comment out system's Lua
# detection bellow.

VERLUA=$( lua -v 2>&1 | grep -o -E '[0-9]\.[0-9]' )
echo -n "Lua's version : $VERLUA"

if pkg-config --cflags "lua$VERLUA" > /dev/null 2>&1; then
	echo "  (Packaged)"
	LUA="\$(shell pkg-config --cflags lua$VERLUA )"
	LUALIB="\$(shell pkg-config --libs lua$VERLUA )"
elif pkg-config --cflags lua > /dev/null 2>&1; then
	echo " (unpackaged)"
	LUA="\$(shell pkg-config --cflags lua )"
	LUALIB="\$(shell pkg-config --libs lua )"
else
	echo " - No package found"
	echo "Workaround : edit this remake file to hardcode where Lua is installed."
	echo
	exit 1
fi

echo -n "Selene : "

if [ -f /usr/local/lib/libSelene.so.2 ]; then
	echo "System installation"
	SELDIR=/usr/local
	SELLIB='-lSelene'
elif [ -d ~/Projets/Selene.v7 ]; then
	echo "**DEV**DEV** V7"
	SELDIR=~/Projets/Selene.v7
	SELLIB='-l:libSelene.so.2'
else
	echo "**DEV**DEV**"
	SELDIR=~/Projets/Selene
	SELLIB='-l:libSelene.so.2'
	echo "Don't forget"
	echo "export LD_LIBRARY_PATH=$SELDIR/lib:$LD_LIBRARY_PATH"
fi

cd src

SOURCES='*.cpp'
LIBS=''

if [ -z ${BUILD_PGSQL+x} ]; then
	echo "PostgreSQL : not compiled"
else
	echo "PostgreSQL : included"
	SOURCES+=' pgsql/*.cpp'
	LIBS+=' -lpq'
fi

if [ -Z ${BUILD_TOILE+x} ]; then
	echo "Toile : not compiled"
else
	echo "Toile : included"
	SOURCES+=' Toile/*.cpp'
fi

echo
echo "----------------"
echo

LFMakeMaker -v +f=Makefile -cc="g++" --opts="-Wall -O2 ${DEBUG} ${BUILD_TOILE} ${BUILD_PGSQL} -lpthread -lpaho-mqtt3c -ldl \
${LUA} ${LUALIB} \
${LIBS} \
-I$SELDIR/include -L$SELDIR/lib $SELLIB" ${SOURCES} -t=../Majordome > Makefile
