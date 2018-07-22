#!/bin/bash

#this script is used to create a Makefile

SELLIBDIR=~/Projets/Selene.v4
SELLIBHDIR=$SELLIBDIR/src/SeleneLibrary

cd src

LFMakeMaker -v +f=Makefile -cc="gcc -Wall -O2 -DxDEBUG -lpthread -lpaho-mqtt3c `pkg-config --cflags lua` `pkg-config --libs lua` \
-I$SELLIBHDIR -L$SELLIBDIR -lSelene" *.c -t=../Majordome > Makefile