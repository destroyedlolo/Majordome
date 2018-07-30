# makefile created automaticaly by LFMakeMaker
# LFMakeMaker 1.4 (May  1 2018 12:31:45) (c)LFSoft 1997

gotoall: all


#The compiler (may be customized for compiler's options).
cc=g++ -Wall -O2 -DDEBUG -lpthread -lpaho-mqtt3c  -llua -lm -I/home/laurent/Projets/Selene.v4/src/SeleneLibrary -L/home/laurent/Projets/Selene.v4 -lSelene
opts=

Config.o : Config.cpp Components.h MayBeEmptyString.h Config.h \
  SortDir.h MQTTTopic.h Object.h SubConfigDir.h Makefile 
	$(cc) -c -o Config.o Config.cpp $(opts) 

Helpers.o : Helpers.cpp Components.h MayBeEmptyString.h Makefile 
	$(cc) -c -o Helpers.o Helpers.cpp $(opts) 

Majordome.o : Majordome.cpp Components.h MayBeEmptyString.h Config.h \
  SortDir.h MQTTTopic.h Object.h Makefile 
	$(cc) -c -o Majordome.o Majordome.cpp $(opts) 

MQTTTopic.o : MQTTTopic.cpp Components.h MayBeEmptyString.h \
  MQTTTopic.h Object.h Makefile 
	$(cc) -c -o MQTTTopic.o MQTTTopic.cpp $(opts) 

SortDir.o : SortDir.cpp SortDir.h Makefile 
	$(cc) -c -o SortDir.o SortDir.cpp $(opts) 

SubConfigDir.o : SubConfigDir.cpp Components.h MayBeEmptyString.h \
  SubConfigDir.h Config.h SortDir.h MQTTTopic.h Object.h Makefile 
	$(cc) -c -o SubConfigDir.o SubConfigDir.cpp $(opts) 

../Majordome : SubConfigDir.o SortDir.o MQTTTopic.o Majordome.o \
  Helpers.o Config.o Makefile 
	 $(cc) -o ../Majordome SubConfigDir.o SortDir.o MQTTTopic.o \
  Majordome.o Helpers.o Config.o $(opts) 

all: ../Majordome 
