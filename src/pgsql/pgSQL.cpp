#include "pgSQL.h"
#include "../Config.h"
#include "../Helpers.h"

#include <fstream>

#include <cstring>
#include <cassert>

pgSQL::pgSQL(const std::string &fch, std::string &where, std::string &name) : Object(fch, where, name){

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

void pgSQL::readConfigDirective( std::string &l, std::string &name, bool &nameused ){
	MayBeEmptyString arg;

	if( !!(arg = striKWcmp( l, "-->> pgConnectionstring=" ))){
		this->conninfo = arg;

		if(verbose)
			SelLog->Log('C', "\t\tConnexion string : '%s'", this->conninfo.c_str());
	} else 
		this->Object::readConfigDirective(l, name, nameused);
}

	/*****
	 * Lua exposed functions
	 *****/

static class pgSQL *checkMajordomepgSQL(lua_State *L){
	class pgSQL **r = (class pgSQL **)SelLua->testudata(L, 1, "MajordomepgSQL");
	luaL_argcheck(L, r != NULL, 1, "'MajordomepgSQL' expected");
	return *r;
}

static int mmm_find(lua_State *L){
	const char *name = luaL_checkstring(L, 1);

	try {
		class pgSQL *f = config.pgSQLsList.at( name );
		class pgSQL **pgsql = (class pgSQL **)lua_newuserdata(L, sizeof(class pgSQL *));
		assert(pgsql);

		*pgsql = f;
		luaL_getmetatable(L, "MajordomepgSQL");
		lua_setmetatable(L, -2);

		return 1;
	} catch( std::out_of_range &e ){	// Not found 
		return 0;
	}
}

static const struct luaL_Reg MajpgSQLLib [] = {
	{"find", mmm_find},
	{NULL, NULL}
};

static int mmm_getContainer(lua_State *L){
	class pgSQL *pgsql= checkMajordomepgSQL(L);
	lua_pushstring( L, pgsql->getWhereC() );
	return 1;
}

static int mmm_getName(lua_State *L){
	class pgSQL *pgsql= checkMajordomepgSQL(L);
	lua_pushstring( L, pgsql->getName().c_str() );
	return 1;
}

static int mmm_isEnabled( lua_State *L ){
	class pgSQL *pgsql= checkMajordomepgSQL(L);
	lua_pushboolean( L, pgsql->isEnabled() );
	return 1;
}

static int mmm_enabled( lua_State *L ){
	class pgSQL *pgsql= checkMajordomepgSQL(L);
	pgsql->enable();
	return 0;
}

static int mmm_disable( lua_State *L ){
	class pgSQL *pgsql= checkMajordomepgSQL(L);
	pgsql->disable();
	return 0;
}

static const struct luaL_Reg MajpgSQLM [] = {
	{"getContainer", mmm_getContainer},
 	{"getName", mmm_getName},
	{"isEnabled", mmm_isEnabled},
	{"Enable", mmm_enabled},
	{"Disable", mmm_disable},
	{NULL, NULL}
};

void pgSQL::initLuaInterface( lua_State *L ){
	SelLua->objFuncs( L, "MajordomepgSQL", MajpgSQLM );
	SelLua->libCreateOrAddFuncs( L, "MajordomepgSQL", MajpgSQLLib );
}
