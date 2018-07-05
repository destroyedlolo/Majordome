#!/bin/bash

#this script is used to create a Makefile

cd src

# LFMakeMaker -v +f=Makefile -cc='gcc -Wall -O2 -DxDEBUG -lpthread -lpaho-mqtt3c `pkg-config --cflags lua` `pkg-config --libs lua` `pkg-config --cflags json-c` `pkg-config --libs json-c`' *.c -t=../Majordome > Makefile
LFMakeMaker -v +f=Makefile -cc='gcc -Wall -O2 -DxDEBUG -lpthread -lpaho-mqtt3c `pkg-config --cflags lua` `pkg-config --libs lua`' *.c -t=../Majordome > Makefile
