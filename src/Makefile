# makefile created automaticaly by LFMakeMaker
# LFMakeMaker 1.4 (May  1 2018 12:31:45) (c)LFSoft 1997

gotoall: all


#The compiler (may be customized for compiler's options).
cc=gcc -Wall -O2 -DxDEBUG -lpthread -lpaho-mqtt3c `pkg-config --cflags lua` `pkg-config --libs lua`
opts=

Helpers.o : Helpers.c Helpers.h Makefile 
	$(cc) -c -o Helpers.o Helpers.c $(opts) 

Majordome.o : Majordome.c Helpers.h Makefile 
	$(cc) -c -o Majordome.o Majordome.c $(opts) 

../Majordome : Majordome.o Helpers.o Makefile 
	 $(cc) -o ../Majordome Majordome.o Helpers.o $(opts) 

all: ../Majordome 
