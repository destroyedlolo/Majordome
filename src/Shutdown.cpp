#include "Config.h"
#include "Helpers.h"
#include "Shutdown.h"

#include <iostream>
#include <fstream>

#include <cstring>
#include <cassert>

Shutdown::Shutdown(const std::string &fch, std::string &where, std::string &name, lua_State *L) : Object(fch, where, name), LuaExec(fch, where, name){
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

void Shutdown::exec( void ){
	lua_State *L = createLuaState();
	if(L){
		threadEnvironment(L);	// Feed environment with generals
		if(this->feedbyNeeded(L))
			this->execSync(L);
		lua_close(L);
	}
}

	/*****
	 * Lua exposed functions
	 *****/

static class Shutdown *checkMajordomeShutdown(lua_State *L){
	class Shutdown **r = (class Shutdown **)SelLua->testudata(L, 1, "MajordomeShutdown");
	luaL_argcheck(L, r != NULL, 1, "'MajordomeShutdown' expected");
	return *r;
}

static int shut_find(lua_State *L){
	const char *name = luaL_checkstring(L, 1);

	try {
		class Shutdown *mm = config.ShutdownsList.at( name );
		class Shutdown **minmax = (class Shutdown **)lua_newuserdata(L, sizeof(class Shutdown *));
		assert(minmax);

		*minmax = mm;
		luaL_getmetatable(L, "MajordomeShutdown");
		lua_setmetatable(L, -2);

		return 1;
	} catch( std::out_of_range &e ){	// Not found 
		return 0;
	}
}

static const struct luaL_Reg MajShutdownLib [] = {
	{"find", shut_find},
	{NULL, NULL}
};

static int shut_getContainer(lua_State *L){
	class Shutdown *minmax= checkMajordomeShutdown(L);
	lua_pushstring( L, minmax->getWhereC() );
	return 1;
}

static int shut_getName(lua_State *L){
	class Shutdown *minmax= checkMajordomeShutdown(L);
	lua_pushstring( L, minmax->getName().c_str() );
	return 1;
}

static int shut_isEnabled( lua_State *L ){
	class Shutdown *minmax= checkMajordomeShutdown(L);
	lua_pushboolean( L, minmax->isEnabled() );
	return 1;
}

static int shut_enabled( lua_State *L ){
	class Shutdown *minmax= checkMajordomeShutdown(L);
	minmax->enable();
	return 0;
}

static int shut_disable( lua_State *L ){
	class Shutdown *minmax= checkMajordomeShutdown(L);
	minmax->disable();
	return 0;
}

static const struct luaL_Reg MajShutdownM [] = {
	{"getContainer", shut_getContainer},
 	{"getName", shut_getName},
	{"isEnabled", shut_isEnabled},
	{"Enable", shut_enabled},
	{"Disable", shut_disable},
	{NULL, NULL}
};

void Shutdown::initLuaInterface( lua_State *L ){
	SelLua->objFuncs( L, "MajordomeShutdown", MajShutdownM );
	SelLua->libCreateOrAddFuncs( L, "MajordomeShutdown", MajShutdownLib );
}
