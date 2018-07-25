# makefile created automaticaly by LFMakeMaker
# LFMakeMaker 1.4 (May  1 2018 12:31:45) (c)LFSoft 1997

gotoall: all


#The compiler (may be customized for compiler's options).
cc=g++ -Wall -O2 -DxDEBUG -lpthread -lpaho-mqtt3c  -llua -lm -I/home/laurent/Projets/Selene.v4/src/SeleneLibrary -L/home/laurent/Projets/Selene.v4 -lSelene
opts=

Helpers.o : Helpers.cpp Components.h Makefile 
	$(cc) -c -o Helpers.o Helpers.cpp $(opts) 

Majordome.o : Majordome.cpp Components.h Makefile 
	$(cc) -c -o Majordome.o Majordome.cpp $(opts) 

readUserConfig.o : readUserConfig.cpp Components.h Makefile 
	$(cc) -c -o readUserConfig.o readUserConfig.cpp $(opts) 

../Majordome : readUserConfig.o Majordome.o Helpers.o Makefile 
	 $(cc) -o ../Majordome readUserConfig.o Majordome.o Helpers.o \
  $(opts) 

all: ../Majordome 
