#!/bin/bash

#this script is used to create a Makefile

if [ -d /usr/local/lib/Selene ]; then
	echo "System installation"
	SELLIBDIR=/usr/local/lib
elif [ -d ~/Projets/Selene.v4 ]; then
	echo "**DEV**DEV** V4"
	SELLIBDIR=~/Projets/Selene.v4
else
	echo "**DEV**DEV**"
	SELLIBDIR=~/Projets/Selene
fi
echo "----------------"
echo

SELLIBHDIR=$SELLIBDIR/src/SeleneLibrary

cd src

LFMakeMaker -v +f=Makefile -cc="g++ -Wall -O2 -DDEBUG -lpthread -lpaho-mqtt3c `pkg-config --cflags lua` `pkg-config --libs lua` \
-I$SELLIBHDIR -L$SELLIBDIR -lSelene" *.cpp -t=../Majordome > Makefile
