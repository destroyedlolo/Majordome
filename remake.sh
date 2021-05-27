#!/bin/bash

#this script is used to create a Makefile

echo
echo "Prerequisites"
echo "-------------"
echo

echo -n "Selene ..."

if [ -d /usr/local/lib/Selene ]; then
	echo "System installation"
	SELLIBDIR=/usr/local/lib
else
	echo "**DEV**DEV**"
	SELLIBDIR=~/Projets/Selene
fi

# system Lua
VERLUA=$( lua -v 2>&1 | grep -o -E '[0-9]\.[0-9]' )

echo -n "Lua's version :" $VERLUA " ... "

if pkg-config --cflags lua$VERLUA > /dev/null 2>&1; then
	echo "Found Lua$VERLUA package"
	LUA="\$(shell pkg-config --cflags lua$VERLUA )"
	LUALIB="\$(shell pkg-config --libs lua$VERLUA )"
elif pkg-config --cflags lua > /dev/null 2>&1; then
	echo "Found Lua package"
	LUA="\$(shell pkg-config --cflags lua )"
	LUALIB="\$(shell pkg-config --libs lua )"
else
	echo "Lua not found"
	exit 1
fi

echo
echo "Source configuration"
echo "--------------------"
echo

SELLIBHDIR=$SELLIBDIR/src/SeleneLibrary

cd src

LFMakeMaker -v +f=Makefile -cc="g++ -Wall -O2 -DDEBUG -lpthread -lpaho-mqtt3c \
$LUA $LUALIB \
-I$SELLIBHDIR -L$SELLIBDIR -lSelene" *.cpp -t=../Majordome > Makefile
