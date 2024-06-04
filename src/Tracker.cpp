#include <iostream>
#include <fstream>
#include <sstream>	// stringstream

#include <cstring>
#include <cassert>
#include <regex>

extern "C" {
    #include "lualib.h"
    #include "lauxlib.h"
};

#include "Config.h"
#include "Helpers.h"
#include "Tracker.h"

Tracker::Tracker( const std::string &fch, std::string &where, std::string &name, lua_State *L ):status(_status::WAITING), howmany(1){
	if(verbose)
		SelLog->Log('L', "\t'%s'", fch.c_str());

	this->extrName( fch, name );
	this->name = name;
	this->where = where;

	/*
	 * Reading file's content
	 */

	std::stringstream buffer;
	std::ifstream file;
	file.exceptions ( std::ios::eofbit | std::ios::failbit );
	try {
		std::ifstream file(fch);
		std::streampos pos;

		bool nameused = false;	// if so, the name can't be changed anymore

		do {
			std::string l;
			pos = file.tellg();

			std::getline( file, l);
			if( l.compare(0, 2, "--") ){	// End of comments
				file.seekg( pos );
				break;
			}

			MayBeEmptyString arg;
			if( !!(arg = striKWcmp( l, "-->> name=" ))){
				if( nameused ){
					SelLog->Log('F', "\t\tName can be changed only before listen, until or waitfor directives");
					exit(EXIT_FAILURE);
				}

				this->name = name = arg;
				if(verbose)
					SelLog->Log('C', "\t\tChanging name to '%s'", name.c_str());

			} else if( !!(arg = striKWcmp( l, "-->> listen=" ))){
				Config::TopicElements::iterator topic;
				if( (topic = config.TopicsList.find(arg)) != config.TopicsList.end()){
					if(verbose)
						SelLog->Log('C', "\t\tAdded to topic '%s'", arg.c_str());
	 				topic->second.addTracker( this->getName() );
					nameused = true;
				} else {
					SelLog->Log('F', "\t\tTopic '%s' is not (yet ?) defined", arg.c_str());
					exit(EXIT_FAILURE);
				}
			} else if( !!(arg = striKWcmp( l, "-->> howmany=" ))){
				if((this->howmany = strtoul(arg.c_str(), NULL, 0))<1)
					this->howmany = 1;
				if(verbose)
					SelLog->Log('C', "\t\tHow many: '%d'", this->howmany);
			} else if( !!(arg = striKWcmp( l, "-->> statustopic=" ))){
				setStatusTopic( std::regex_replace(arg, std::regex("%ClientID%"), MQTT_ClientID) );
				if(verbose)
					SelLog->Log('C', "\t\tStatus topic : '%s'", this->getStatusTopic().c_str());
			} else if( !!(arg = striKWcmp( l, "-->> start=" ))){
				Config::TimerElements::iterator timer;
				if( (timer = config.TimersList.find(arg)) != config.TimersList.end()){
					if(verbose)
						SelLog->Log('C', "\t\tStart timer '%s'", arg.c_str());
	 				timer->second.addStartTracker( this->getName() );
					nameused = true;
				} else {
					SelLog->Log('F', "\t\tTimer '%s' is not (yet ?) defined", arg.c_str());
					exit(EXIT_FAILURE);
				}
			} else if( !!(arg = striKWcmp( l, "-->> stop=" ))){
				Config::TimerElements::iterator timer;
				if( (timer = config.TimersList.find(arg)) != config.TimersList.end()){
					if(verbose)
						SelLog->Log('C', "\t\tStop timer '%s'", arg.c_str());
	 				timer->second.addStopTracker( this->getName() );
					nameused = true;
				} else {
					SelLog->Log('F', "\t\tTimer '%s' is not (yet ?) defined", arg.c_str());
					exit(EXIT_FAILURE);
				}
			} else if( !!(arg = striKWcmp( l, "-->> enableRDV=" ))){
				Config::EventElements::iterator event;
				if( (event = config.EventsList.find(arg)) != config.EventsList.end()){
					if(verbose)
						SelLog->Log('C', "\t\tEnabling rendez-vous '%s'", arg.c_str());
	 				event->second.addTrackerEN( this->getName() );
					nameused = true;
				} else {
					SelLog->Log('F', "\t\tRendez-vous '%s' is not (yet ?) defined", arg.c_str());
					exit(EXIT_FAILURE);
				}
			} else if( !!(arg = striKWcmp( l, "-->> disableRDV=" ))){
				Config::EventElements::iterator event;
				if( (event = config.EventsList.find(arg)) != config.EventsList.end()){
					if(verbose)
						SelLog->Log('C', "\t\tDisabling rendez-vous '%s'", arg.c_str());
	 				event->second.addTrackerDIS( this->getName() );
					nameused = true;
				} else {
					SelLog->Log('F', "\t\tRendez-vous '%s' is not (yet ?) defined", arg.c_str());
					exit(EXIT_FAILURE);
				}
			} else if( l == "-->> activated" ){
				if(verbose)
					SelLog->Log('C', "\t\tActivated at startup");
				this->status = _status::CHECKING;
				this->hm_counter = this->howmany;
			} else if( l == "-->> quiet" ){
				if(verbose)
					SelLog->Log('C', "\t\tBe quiet");
				this->beQuiet();
			} else if( l == "-->> disabled" ){
				if(verbose)
					SelLog->Log('C', "\t\tDisabled");
				this->disable();
			} else if( LuaExec::readConfigDirective(l) )
				nameused = true;
#if 0
else printf("Ignore '%s'\n", l.c_str());
#endif
		} while(true);

		/*
		 * Reading the remaining of the script and keep it as 
		 * an Lua's script
		 */

		buffer << file.rdbuf();
		file.close();
	} catch(const std::ifstream::failure &e){
		if(!file.eof()){
			SelLog->Log('F', "%s : %s", fch.c_str(), strerror(errno) );
			exit(EXIT_FAILURE);
		}
	}

	if( !this->LoadFunc( L, buffer, this->name.c_str() ))
		exit(EXIT_FAILURE);
}

const char *Tracker::getStatusC( void ){
	switch( this->status ){
	case _status::WAITING :
		return "WAITING";
	case _status::CHECKING :
		return "CHECKING";
	case _status::DONE :
		return "DONE";
	}

	return "?? Invalid tracker status ??";
}

void Tracker::publishstatus( void ){
	if( this->asStatusTopic() ){
		SelMQTT->mqttpublish( MQTT_client, this->getStatusTopic().c_str(), strlen(this->getStatusC()), (void *)this->getStatusC(), false);
	}
}

void Tracker::feedState( lua_State *L, const char *name, const char *topic, const char *payload, bool tracker, const char *trkstatus ){

	try {
		class Tracker &trk = config.TrackersList.at( this->getNameC() );
		class Tracker **tracker = (class Tracker **)lua_newuserdata(L, sizeof(class Tracker *));
		assert(tracker);

		*tracker = &trk;
		luaL_getmetatable(L, "MajordomeTracker");
		lua_setmetatable(L, -2);
		lua_setglobal( L, "MAJORDOME_Myself" );
	} catch( std::out_of_range &e ){	// Not found 
		SelLog->Log('F', "Can't find tracker '%s'", this->getNameC() );
		exit(EXIT_FAILURE);
	}

	LuaExec::feedState(L, name, topic, payload, tracker, trkstatus);
}

bool Tracker::exec( const char *name, const char *topic, const char *payload ){
	if( !this->isEnabled() || this->status != _status::CHECKING ){
		if(verbose)
			SelLog->Log('T', "Tracker '%s' from '%s' is disabled or inactive", this->getNameC(), this->getWhereC() );
		return false;
	}

	LuaExec::boolRetCode rc;
	bool r = this->LuaExec::execSync(name, topic, payload, true, &rc);

	if( rc == LuaExec::boolRetCode::RCfalse )
		this->hm_counter = this->howmany;
	else if( rc == LuaExec::boolRetCode::RCtrue ){
		if(!--this->hm_counter)
			this->done();
	}

	return r;
}

void Tracker::notifyChanged( void ){
	if( this->isEnabled() ){
		for( Entries::iterator tsk = this->changingTasks.begin(); tsk != this->changingTasks.end(); tsk++){
			try {
				LuaTask &task = config.findTask( *tsk );
				if( this->isQuiet() )
					task.beQuiet();
				task.exec( this->getNameC(), NULL, NULL, true, this->getStatusC() );
			} catch (...) {
				SelLog->Log('F', "Internal error : can't find task \"%s\"", (*tsk).c_str() );
				exit(EXIT_FAILURE);
			}
		}
	}
}

void Tracker::start( void ){
	if( this->isEnabled() && this->getStatus() != _status::CHECKING ){
		for( Entries::iterator tsk = this->startingTasks.begin(); tsk != this->startingTasks.end(); tsk++){
			try {
				LuaTask &task = config.findTask( *tsk );
				if( this->isQuiet() )
					task.beQuiet();
				task.exec( this->getNameC(), NULL, NULL, true, this->getStatusC() );
			} catch (...) {
				SelLog->Log('F', "Internal error : can't find task \"%s\"", (*tsk).c_str() );
				exit(EXIT_FAILURE);
			}
		}
	}
	if(verbose)
		SelLog->Log('T', "Tracker '%s' is checking", this->getNameC() );
	this->status = _status::CHECKING;
	this->hm_counter = this->howmany;
	this->publishstatus();
	this->notifyChanged();
}

void Tracker::stop( void ){
	if( this->isEnabled() && this->getStatus() != _status::WAITING ){
		for( Entries::iterator tsk = this->stoppingTasks.begin(); tsk != this->stoppingTasks.end(); tsk++){
			try {
				LuaTask &task = config.findTask( *tsk );
				if( this->isQuiet() )
					task.beQuiet();
				task.exec( this->getNameC(), NULL, NULL, true, this->getStatusC() );
			} catch (...) {
				SelLog->Log('F', "Internal error : can't find task \"%s\"", (*tsk).c_str() );
				exit(EXIT_FAILURE);
			}
		}
	}
	if(verbose)
		SelLog->Log('T', "Tracker '%s' is waiting", this->getNameC() );
	this->status = _status::WAITING;
	this->publishstatus();
	this->notifyChanged();
}

void Tracker::done( void ){
	if( this->isEnabled() && this->getStatus() == _status::CHECKING ){
		this->execTasks(config, this->getNameC(), NULL, NULL, true, this->getStatusC()); // by definition the previous status was CHECKING
	}

	if(verbose)
		SelLog->Log('T', "Tracker '%s' is done", this->getNameC() );
	this->status = _status::DONE;
	this->publishstatus();
	this->notifyChanged();
}

	/*****
	 * Lua exposed functions
	 *****/

static class Tracker *checkMajordomeTracker(lua_State *L){
	class Tracker **r = (class Tracker **)luaL_testudata(L, 1, "MajordomeTracker");
	luaL_argcheck(L, r != NULL, 1, "'MajordomeTracker' expected");
	return *r;
}

static int mtrk_find(lua_State *L){
	const char *name = luaL_checkstring(L, 1);

	try {
		class Tracker &trk = config.TrackersList.at( name );
		class Tracker **tracker = (class Tracker **)lua_newuserdata(L, sizeof(class Tracker *));
		assert(tracker);

		*tracker = &trk;
		luaL_getmetatable(L, "MajordomeTracker");
		lua_setmetatable(L, -2);

		return 1;
	} catch( std::out_of_range &e ){	// Not found 
		return 0;
	}
}

static const struct luaL_Reg MajTrackerLib [] = {
	{"find", mtrk_find},
	{NULL, NULL}
};

static int mtrk_getContainer(lua_State *L){
	class Tracker *tracker = checkMajordomeTracker(L);
	lua_pushstring( L, tracker->getWhereC() );
	return 1;
}

static int mtrk_getName(lua_State *L){
	class Tracker *tracker = checkMajordomeTracker(L);
	lua_pushstring( L, tracker->getName().c_str() );
	return 1;
}

static int mtrk_enabled( lua_State *L ){
	class Tracker *tracker = checkMajordomeTracker(L);
	tracker->enable();
	return 1;
}

static int mtrk_disable( lua_State *L ){
	class Tracker *tracker = checkMajordomeTracker(L);
	tracker->disable();
	return 1;
}

static int mtrk_isEnabled( lua_State *L ){
	class Tracker *tracker = checkMajordomeTracker(L);
	lua_pushboolean( L, tracker->isEnabled() );
	return 1;
}

static int mtrk_getCounter(lua_State *L){
	class Tracker *tracker = checkMajordomeTracker(L);
	lua_pushinteger( L, tracker->getCounter() );
	return 1;
}

static int mtrk_resetCounter(lua_State *L){
	class Tracker *tracker = checkMajordomeTracker(L);
	tracker->resetCounter();
	return 0;
}

static int mtrk_getStatus(lua_State *L){
	class Tracker *tracker = checkMajordomeTracker(L);
	lua_pushstring( L, tracker->getStatusC() );
	return 1;
}

static int mtrk_setStatus(lua_State *L){
	class Tracker *tracker = checkMajordomeTracker(L);
	const char *arg = luaL_checkstring(L, 2);

	if( !strcmp(arg, "CHECKING") || !strcmp(arg, "START") )
		tracker->start();
	else if( !strcmp(arg, "WAITING") || !strcmp(arg, "STOP") )
		tracker->stop();
	else 
		tracker->done();
	return 0;
}

static const struct luaL_Reg MajTrackerM [] = {
	{"getContainer", mtrk_getContainer},
	{"getName", mtrk_getName},
	{"isEnabled", mtrk_isEnabled},
	{"Enable", mtrk_enabled},
	{"Disable", mtrk_disable},
	{"getCounter", mtrk_getCounter},
	{"resetCounter", mtrk_resetCounter},
	{"getStatus", mtrk_getStatus},
	{"setStatus", mtrk_setStatus},
	{NULL, NULL}
};

void Tracker::initLuaObject( lua_State *L ){
	SelLua->objFuncs( L, "MajordomeTracker", MajTrackerM );
	SelLua->libCreateOrAddFuncs( L, "MajordomeTracker", MajTrackerLib );
}

