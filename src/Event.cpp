#include "Event.h"
#include "Config.h"
#include "Helpers.h"

#include <iostream>
#include <fstream>

#include <cstring>
#include <cassert>

Event::Event(const std::string &fch, std::string &where) : Object(fch, where){
	this->loadConfigurationFile(fch, where);
}

void Event::readConfigDirective(std::string &l){
		/* No specific directive */
	this->Object::readConfigDirective(l);
}

void Event::execHandlers(lua_State *L){
	if(this->isEnabled()){
#ifdef DEBUG
		if(debug && !this->isQuiet())
			SelLog->Log('D', "[%s] Handlers called", this->getNameC());
#endif

		for(auto &i : *this)
			i->exec(L);

#ifdef DEBUG
	} else if(debug && !this->isQuiet()){
		SelLog->Log('D', "Event %s is disabled : no tasks launched", this->getNameC());
	}
#endif
}

void Event::enableTrackers( void ){
#ifdef DEBUG
	if(debug && !this->isQuiet())
		SelLog->Log('D', "[%s] %d traker(s) to enable", this->getNameC(), this->trackersToEnable.size());
#endif

	for(auto &i : this->trackersToEnable)
		i->enable();
}

void Event::disableTrackers( void ){
#ifdef DEBUG
	if(debug && !this->isQuiet())
		SelLog->Log('D', "[%s] %d traker(s) to disable", this->getNameC(), this->trackersToDisable.size());
#endif

	for(auto &i : this->trackersToDisable)
		i->disable();
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

static int mevt_Launch( lua_State *L ){
	class Event *event = checkMajordomeEvent(L);

	if(event->isEnabled()){
		event->execHandlers(L);
		event->enableTrackers();
		event->disableTrackers();
	}
		
	return 0;
}

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
	{"Launch", mevt_Launch},
	{"getContainer", mevt_getContainer},
	{"getName", mevt_getName},
	{"isEnabled", mevt_isEnabled},
	{"Enable", mevt_enabled},
	{"Disable", mevt_disable},
	{NULL, NULL}
};

void Event::initLuaInterface( lua_State *L ){
	SelLua->objFuncs( L, "MajordomeRendezVous", MajEventM );
	SelLua->libCreateOrAddFuncs( L, "MajordomeRendezVous", MajEventLib );
}
