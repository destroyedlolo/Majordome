# makefile created automaticaly by LFMakeMaker
# LFMakeMaker 1.6 (Nov 18 2023 14:49:11) (c)LFSoft 1997

gotoall: all


#The compiler (may be customized for compiler's options).
cc=g++ -Wall -O2 -DDEBUG -lpthread -lpaho-mqtt3c $(shell pkg-config --cflags lua5.4 ) $(shell pkg-config --libs lua5.4 ) -I/usr/local/lib/src/SeleneLibrary -L/usr/local/lib -lSelene
opts=

Majordome.o : Majordome.cpp Makefile 
	$(cc) -c -o Majordome.o Majordome.cpp $(opts) 

../Majordome : Majordome.o Makefile 
	 $(cc) -o ../Majordome Majordome.o $(opts) 

all: ../Majordome 
