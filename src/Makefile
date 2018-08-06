# makefile created automaticaly by LFMakeMaker
# LFMakeMaker 1.4 (Apr  3 2018 23:14:49) (c)LFSoft 1997

gotoall: all


#The compiler (may be customized for compiler's options).
cc=g++ -Wall -O2 -DDEBUG -lpthread -lpaho-mqtt3c  -llua -lm -I/home/laurent/Projets/Selene/src/SeleneLibrary -L/home/laurent/Projets/Selene -lSelene
opts=

Config.o : Config.cpp Components.h MayBeEmptyString.h Config.h \
  SortDir.h MQTTTopic.h Object.h LuaTask.h SubConfigDir.h Makefile 
	$(cc) -c -o Config.o Config.cpp $(opts) 

Helpers.o : Helpers.cpp Components.h MayBeEmptyString.h Makefile 
	$(cc) -c -o Helpers.o Helpers.cpp $(opts) 

LuaTask.o : LuaTask.cpp Components.h MayBeEmptyString.h LuaTask.h \
  Object.h Makefile 
	$(cc) -c -o LuaTask.o LuaTask.cpp $(opts) 

Majordome.o : Majordome.cpp Components.h MayBeEmptyString.h Config.h \
  SortDir.h MQTTTopic.h Object.h LuaTask.h Makefile 
	$(cc) -c -o Majordome.o Majordome.cpp $(opts) 

MQTTTopic.o : MQTTTopic.cpp Components.h MayBeEmptyString.h \
  MQTTTopic.h Object.h Makefile 
	$(cc) -c -o MQTTTopic.o MQTTTopic.cpp $(opts) 

Object.o : Object.cpp Object.h Makefile 
	$(cc) -c -o Object.o Object.cpp $(opts) 

SortDir.o : SortDir.cpp SortDir.h Makefile 
	$(cc) -c -o SortDir.o SortDir.cpp $(opts) 

SubConfigDir.o : SubConfigDir.cpp Components.h MayBeEmptyString.h \
  SubConfigDir.h Config.h SortDir.h MQTTTopic.h Object.h LuaTask.h \
  Makefile 
	$(cc) -c -o SubConfigDir.o SubConfigDir.cpp $(opts) 

../Majordome : SubConfigDir.o SortDir.o Object.o MQTTTopic.o \
  Majordome.o LuaTask.o Helpers.o Config.o Makefile 
	 $(cc) -o ../Majordome SubConfigDir.o SortDir.o Object.o \
  MQTTTopic.o Majordome.o LuaTask.o Helpers.o Config.o $(opts) 

all: ../Majordome 
