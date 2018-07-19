# makefile created automaticaly by LFMakeMaker
# LFMakeMaker 1.4 (May  1 2018 12:31:45) (c)LFSoft 1997

gotoall: all


#The compiler (may be customized for compiler's options).
cc=gcc -Wall -O2 -DxDEBUG -lpthread -lpaho-mqtt3c  -llua -lm -I/home/laurent/Projets/Selene.v4/src/SeleneLibrary -L/home/laurent/Projets/Selene.v4 -lSelene
opts=

Helpers.o : Helpers.c Components.h Makefile 
	$(cc) -c -o Helpers.o Helpers.c $(opts) 

Majordome.o : Majordome.c Components.h sortdir.h Makefile 
	$(cc) -c -o Majordome.o Majordome.c $(opts) 

sortdir.o : sortdir.c sortdir.h Makefile 
	$(cc) -c -o sortdir.o sortdir.c $(opts) 

../Majordome : sortdir.o Majordome.o Helpers.o Makefile 
	 $(cc) -o ../Majordome sortdir.o Majordome.o Helpers.o $(opts) \
  

all: ../Majordome 
