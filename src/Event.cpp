#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>

extern "C" {
    #include "lualib.h"
    #include "lauxlib.h"
};

#include "Helpers.h"
#include "Event.h"
#include "Config.h"

Event::Event( const std::string &fch, std::string &where, std::string &name ){
	this->extrName( fch, name );
	this->name = name;
	this->where = where;

	/*
	 * Reading file's content
	 */
	if(verbose)
		publishLog('L', "\t'%s'", fch.c_str());

	std::ifstream file;
	file.exceptions ( std::ios::eofbit | std::ios::failbit ); // No need to check failbit
	try {
		std::string l;

		file.open(fch);
		while( std::getline( file, l) ){
			MayBeEmptyString arg;

			if( !!(arg = striKWcmp( l, "name=" )) ){
				this->name = name = arg;
				if(verbose)
					publishLog('C', "\t\tChanging name to '%s'", name.c_str());
			} else if( l == "disabled" ){
				if(verbose)
					publishLog('C', "\t\tDisabled");
				this->disable();
			}
#if 0
else publishLog('D', "Ignore '%s'", l.c_str());
#endif
		}
	} catch(const std::ifstream::failure &e){
		if(!file.eof()){
			publishLog('F', "%s : %s", fch.c_str(), strerror(errno) );
			exit(EXIT_FAILURE);
		}
	} catch(const std::invalid_argument &e){
		publishLog('F', "%s : invalid argument", fch.c_str() );
		exit(EXIT_FAILURE);
	}

	file.close();
}

void Event::execTasks( Config &cfg, const char *trig_name, const char *topic, const char *payload, bool tracker, const char *trkstatus ){
#ifdef DEBUG
	if(debug && !this->isQuiet())
		publishLog('D', "execTasks() : %d to run", this->list.size() );
#endif

	for( Entries::iterator tsk = this->begin(); tsk != this->end(); tsk++){
		try {
			LuaTask &task = cfg.findTask( *tsk );
			task.exec( trig_name, topic, payload, tracker, trkstatus );
		} catch (...) {
			publishLog('F', "Internal error : can't find task \"%s\"", (*tsk).c_str() );
			exit(EXIT_FAILURE);
		}
	}
}

void Event::execTasks( Config &cfg, const char *timer_name ){
#ifdef DEBUG
	if(debug && !this->isQuiet())
		publishLog('D', "execTasks() : %d to run", this->list.size() );
#endif

	for( Entries::iterator tsk = this->begin(); tsk != this->end(); tsk++){
		try {
			LuaTask &task = cfg.findTask( *tsk );
			task.exec( timer_name );
		} catch (...) {
			publishLog('F', "Internal error : can't find task \"%s\"", (*tsk).c_str() );
			exit(EXIT_FAILURE);
		}
	}
}

	/*****
	 * Lua exposed functions
	 *****/
static class Event *checkMajordomeEvent(lua_State *L){
	class Event **r = (class Event **)luaL_testudata(L, 1, "MajordomeRendezVous");
	luaL_argcheck(L, r != NULL, 1, "'MajordomeRendezVous' expected");
	return *r;
}

static int mevt_find(lua_State *L){
/* Find an event
 * 1 : event to find
 * 2 : true if it has to fail if not found
 */
	const char *name = luaL_checkstring(L, 1);
	bool tofail =  lua_toboolean(L, 2);

	try {
		class Event &evt = config.EventsList.at( name );
		class Event **event = (class Event **)lua_newuserdata(L, sizeof(class Event *));
		assert(event);

		*event = &evt;
		luaL_getmetatable(L, "MajordomeRendezVous");
		lua_setmetatable(L, -2);

		return 1;
	} catch( std::out_of_range &e ){	// Not found
		if( tofail )
			return luaL_error( L, "Can't find \"%s\" RendezVous", name );
		return 0;
	}
}

static const struct luaL_Reg MajEventLib [] = {
	{"find", mevt_find},
	{NULL, NULL}
};

static int mevt_Launch( lua_State *L ){
	class Event *event = checkMajordomeEvent(L);

	if( event->isEnabled() )
		event->execTasks(config, event->getNameC(), NULL, "fake");
#ifdef DEBUG
	else if( debug )
		publishLog('D', "Event %s is disabled : no tasks launched", event->getNameC() );
#endif	
	return 0;
}

static int mevt_getContainer( lua_State *L ){
	class Event *event = checkMajordomeEvent(L);
	lua_pushstring( L, event->getWhereC() );
	return 1;
}

static int mevt_enabled( lua_State *L ){
	class Event *event = checkMajordomeEvent(L);
	event->enable();
	return 0;
}

static int mevt_disable( lua_State *L ){
	class Event *event = checkMajordomeEvent(L);
	event->disable();
	return 0;
}

static int mevt_isEnabled( lua_State *L ){
	class Event *event = checkMajordomeEvent(L);
	lua_pushboolean( L, event->isEnabled() );
	return 1;
}

static const struct luaL_Reg MajEventM [] = {
	{"Launch", mevt_Launch},
	{"getContainer", mevt_getContainer},
	{"isEnabled", mevt_isEnabled},
	{"Enable", mevt_enabled},
	{"Disable", mevt_disable},
	{NULL, NULL}
};

int Event::initLuaObject( lua_State *L ){
	libSel_objFuncs( L, "MajordomeRendezVous", MajEventM );
	libSel_libFuncs( L, "MajordomeRendezVous", MajEventLib );

	return 1;
}

