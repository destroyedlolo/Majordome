#include "Event.h"
#include "Config.h"

#include <iostream>
#include <fstream>

#include <cstring>
#include <cassert>

Event::Event(const std::string &fch, std::string &where, std::string &name) :
	Object(fch, where, name){

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

		/*
		 * Reading header (Majordome's commands)
		 */

		do {
			std::string l;
			pos = file.tellg();

			std::getline( file, l);
			if( l.compare(0, 2, "--") ){	// End of comments
				file.seekg( pos );
				break;
			}

			this->readConfigDirective(l, name, nameused);
		} while(true);

		file.close();
	} catch(const std::ifstream::failure &e){
		if(!file.eof()){
			SelLog->Log('F', "%s : %s", fch.c_str(), strerror(errno) );
			exit(EXIT_FAILURE);
		}
	}
}


	/* ****
	 * Lua exposed functions
	 * ****/
static class Event *checkMajordomeEvent(lua_State *L){
	class Event **r = (class Event **)SelLua->testudata(L, 1, "MajordomeRendezVous");
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
		class Event *evt = config.EventsList.at( name );
		class Event **event = (class Event **)lua_newuserdata(L, sizeof(class Event *));
		assert(event);

		*event = evt;
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

#if 0
static int mevt_Launch( lua_State *L ){
	class Event *event = checkMajordomeEvent(L);

	if( event->isEnabled() ){
		event->execTasks(config, event->getNameC(), NULL, "fake");
		event->enableTrackers();
		event->disableTrackers();
#ifdef DEBUG
	} else if( debug ){
		SelLog->Log('D', "Event %s is disabled : no tasks launched", event->getNameC());
#endif
	}
	return 0;
}
#endif

static int mevt_getContainer( lua_State *L ){
	class Event *event = checkMajordomeEvent(L);
	lua_pushstring( L, event->getWhereC() );
	return 1;
}

static int mevt_getName( lua_State *L ){
	class Event *event = checkMajordomeEvent(L);
	lua_pushstring( L, event->getName().c_str() );
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
//	{"Launch", mevt_Launch},
	{"getContainer", mevt_getContainer},
	{"getName", mevt_getName},
	{"isEnabled", mevt_isEnabled},
	{"Enable", mevt_enabled},
	{"Disable", mevt_disable},
	{NULL, NULL}
};

void Event::initLuaObject( lua_State *L ){
	SelLua->objFuncs( L, "MajordomeRendezVous", MajEventM );
	SelLua->libCreateOrAddFuncs( L, "MajordomeRendezVous", MajEventLib );
}
