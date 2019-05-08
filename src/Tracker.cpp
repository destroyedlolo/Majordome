#include <iostream>
#include <fstream>
#include <sstream>	// stringstream

#include <cstring>
#include <cassert>

extern "C" {
    #include "lualib.h"
    #include "lauxlib.h"
};

#include "Config.h"
#include "Helpers.h"
#include "Tracker.h"

Tracker::Tracker( Config &cfg, const std::string &fch, std::string &where, std::string &name, lua_State *L ):status(_status::WAITING){
	if(verbose)
		publishLog('L', "\t'%s'", fch.c_str());

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
					publishLog('F', "\t\tName can be changed only before listen, until or waitfor directives");
					exit(EXIT_FAILURE);
				}

				this->name = name = arg;
				if(verbose)
					publishLog('C', "\t\tChanging name to '%s'", name.c_str());

			} else if( !!(arg = striKWcmp( l, "-->> listen=" ))){
				Config::TopicElements::iterator topic;
				if( (topic = cfg.TopicsList.find(arg)) != cfg.TopicsList.end()){
					if(verbose)
						publishLog('C', "\t\tAdded to topic '%s'", arg.c_str());
	 				topic->second.addTracker( this->getName() );
					nameused = true;
				} else {
					publishLog('F', "\t\tTopic '%s' is not (yet ?) defined", arg.c_str());
					exit(EXIT_FAILURE);
				}
			} else if( !!(arg = striKWcmp( l, "-->> start=" ))){
				Config::TimerElements::iterator timer;
				if( (timer = cfg.TimersList.find(arg)) != cfg.TimersList.end()){
					if(verbose)
						publishLog('C', "\t\tStart timer '%s'", arg.c_str());
	 				timer->second.addStartTracker( this->getName() );
					nameused = true;
				} else {
					publishLog('F', "\t\tTimer '%s' is not (yet ?) defined", arg.c_str());
					exit(EXIT_FAILURE);
				}
			} else if( !!(arg = striKWcmp( l, "-->> stop=" ))){
				Config::TimerElements::iterator timer;
				if( (timer = cfg.TimersList.find(arg)) != cfg.TimersList.end()){
					if(verbose)
						publishLog('C', "\t\tStop timer '%s'", arg.c_str());
	 				timer->second.addStopTracker( this->getName() );
					nameused = true;
				} else {
					publishLog('F', "\t\tTimer '%s' is not (yet ?) defined", arg.c_str());
					exit(EXIT_FAILURE);
				}
			} else if( !!(arg = striKWcmp( l, "-->> enableRDV=" ))){
				Config::EventElements::iterator event;
				if( (event = cfg.EventsList.find(arg)) != cfg.EventsList.end()){
					if(verbose)
						publishLog('C', "\t\tEnabling rendez-vous '%s'", arg.c_str());
	 				event->second.addTrackerEN( this->getName() );
					nameused = true;
				} else {
					publishLog('F', "\t\tRendez-vous '%s' is not (yet ?) defined", arg.c_str());
					exit(EXIT_FAILURE);
				}
			} else if( !!(arg = striKWcmp( l, "-->> disableRDV=" ))){
				Config::EventElements::iterator event;
				if( (event = cfg.EventsList.find(arg)) != cfg.EventsList.end()){
					if(verbose)
						publishLog('C', "\t\tDisabling rendez-vous '%s'", arg.c_str());
	 				event->second.addTrackerDIS( this->getName() );
					nameused = true;
				} else {
					publishLog('F', "\t\tRendez-vous '%s' is not (yet ?) defined", arg.c_str());
					exit(EXIT_FAILURE);
				}
			} else if( l == "-->> activated" ){
				if(verbose)
					publishLog('C', "\t\tActivated at startup");
				this->status = _status::CHECKING;
			} else if( l == "-->> quiet" ){
				if(verbose)
					publishLog('C', "\t\tBe quiet");
				this->beQuiet();
			} else if( l == "-->> disabled" ){
				if(verbose)
					publishLog('C', "\t\tDisabled");
				this->disable();
			}
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
			publishLog('F', "%s : %s", fch.c_str(), strerror(errno) );
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
		publishLog('F', "Can't find tracker '%s'", this->getNameC() );
		exit(EXIT_FAILURE);
	}

	LuaExec::feedState(L, name, topic, payload, tracker, trkstatus);
}

bool Tracker::exec( const char *name, const char *topic, const char *payload ){
	if( !this->isEnabled() || this->status != _status::CHECKING ){
		if(verbose)
			publishLog('I', "Tracker '%s' from '%s' is disabled or inactive", this->getNameC(), this->getWhereC() );
		return false;
	}

	return this->LuaExec::execSync(name, topic, payload, true);
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
				publishLog('F', "Internal error : can't find task \"%s\"", (*tsk).c_str() );
				exit(EXIT_FAILURE);
			}
		}
	}
	publishLog('I', "Tracker '%s' is checking", this->getNameC() );
	this->status = _status::CHECKING; 
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
				publishLog('F', "Internal error : can't find task \"%s\"", (*tsk).c_str() );
				exit(EXIT_FAILURE);
			}
		}
	}
	publishLog('I', "Tracker '%s' is waiting", this->getNameC() );
	this->status = _status::WAITING;
}

void Tracker::done( void ){
	if( this->isEnabled() && this->getStatus() == _status::CHECKING )
		this->execTasks(config, this->getNameC(), NULL, NULL, true, this->getStatusC()); // by definition the previous status was CHECKING
	publishLog('I', "Tracker '%s' is done", this->getNameC() );
	this->status = _status::DONE;
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
	{"isEnabled", mtrk_isEnabled},
	{"Enable", mtrk_enabled},
	{"Disable", mtrk_disable},
	{"getStatus", mtrk_getStatus},
	{"setStatus", mtrk_setStatus},
	{NULL, NULL}
};

int Tracker::initLuaObject( lua_State *L ){
	libSel_objFuncs( L, "MajordomeTracker", MajTrackerM );
	libSel_libFuncs( L, "MajordomeTracker", MajTrackerLib );

	return 1;
}

