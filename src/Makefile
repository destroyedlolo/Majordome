# makefile created automaticaly by LFMakeMaker
# LFMakeMaker 1.4 (May  1 2018 12:31:45) (c)LFSoft 1997

gotoall: all


#The compiler (may be customized for compiler's options).
cc=g++ -Wall -O2 -DDEBUG -lpthread -lpaho-mqtt3c  -llua -lm -I/usr/local/lib/src/SeleneLibrary -L/usr/local/lib -lSelene
opts=

Config.o : Config.cpp Helpers.h MayBeEmptyString.h Config.h SortDir.h \
  MQTTTopic.h Event.h Object.h StringVector.h LuaTask.h LuaExec.h \
  Timer.h Tracker.h SubConfigDir.h Makefile 
	$(cc) -c -o Config.o Config.cpp $(opts) 

Event.o : Event.cpp Helpers.h MayBeEmptyString.h Event.h Object.h \
  StringVector.h LuaTask.h LuaExec.h Config.h SortDir.h MQTTTopic.h \
  Timer.h Tracker.h Makefile 
	$(cc) -c -o Event.o Event.cpp $(opts) 

Helpers.o : Helpers.cpp Helpers.h MayBeEmptyString.h Makefile 
	$(cc) -c -o Helpers.o Helpers.cpp $(opts) 

LuaExec.o : LuaExec.cpp Config.h SortDir.h MQTTTopic.h \
  MayBeEmptyString.h Event.h Object.h StringVector.h LuaTask.h \
  LuaExec.h Timer.h Tracker.h Helpers.h Makefile 
	$(cc) -c -o LuaExec.o LuaExec.cpp $(opts) 

LuaTask.o : LuaTask.cpp Config.h SortDir.h MQTTTopic.h \
  MayBeEmptyString.h Event.h Object.h StringVector.h LuaTask.h \
  LuaExec.h Timer.h Tracker.h Helpers.h Makefile 
	$(cc) -c -o LuaTask.o LuaTask.cpp $(opts) 

Majordome.o : Majordome.cpp Helpers.h MayBeEmptyString.h Config.h \
  SortDir.h MQTTTopic.h Event.h Object.h StringVector.h LuaTask.h \
  LuaExec.h Timer.h Tracker.h Version.h Makefile 
	$(cc) -c -o Majordome.o Majordome.cpp $(opts) 

MQTTTopic.o : MQTTTopic.cpp Helpers.h MayBeEmptyString.h MQTTTopic.h \
  Event.h Object.h StringVector.h LuaTask.h LuaExec.h Config.h \
  SortDir.h Timer.h Tracker.h Makefile 
	$(cc) -c -o MQTTTopic.o MQTTTopic.cpp $(opts) 

Object.o : Object.cpp Object.h Makefile 
	$(cc) -c -o Object.o Object.cpp $(opts) 

SortDir.o : SortDir.cpp SortDir.h Makefile 
	$(cc) -c -o SortDir.o SortDir.cpp $(opts) 

SubConfigDir.o : SubConfigDir.cpp Helpers.h MayBeEmptyString.h \
  SubConfigDir.h Config.h SortDir.h MQTTTopic.h Event.h Object.h \
  StringVector.h LuaTask.h LuaExec.h Timer.h Tracker.h Makefile 
	$(cc) -c -o SubConfigDir.o SubConfigDir.cpp $(opts) 

Timer.o : Timer.cpp Helpers.h MayBeEmptyString.h Timer.h \
  StringVector.h Event.h Object.h LuaTask.h LuaExec.h Config.h \
  SortDir.h MQTTTopic.h Tracker.h Makefile 
	$(cc) -c -o Timer.o Timer.cpp $(opts) 

Tracker.o : Tracker.cpp Config.h SortDir.h MQTTTopic.h \
  MayBeEmptyString.h Event.h Object.h StringVector.h LuaTask.h \
  LuaExec.h Timer.h Tracker.h Helpers.h Makefile 
	$(cc) -c -o Tracker.o Tracker.cpp $(opts) 

../Majordome : Tracker.o Timer.o SubConfigDir.o SortDir.o Object.o \
  MQTTTopic.o Majordome.o LuaTask.o LuaExec.o Helpers.o Event.o \
  Config.o Makefile 
	 $(cc) -o ../Majordome Tracker.o Timer.o SubConfigDir.o \
  SortDir.o Object.o MQTTTopic.o Majordome.o LuaTask.o LuaExec.o \
  Helpers.o Event.o Config.o $(opts) 

all: ../Majordome 
