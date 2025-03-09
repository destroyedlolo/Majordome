#include "pgSQL.h"
#include "../Config.h"
#include "../Helpers.h"

#include <fstream>

#include <cstring>
#include <cassert>

pgSQL::pgSQL(const std::string &fch, std::string &where, std::string &name) : Object(fch, where, name) {

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

	this->db = this;
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

static int mmm_doSQL( lua_State *L ){
/* PGRES_COMMAND_OK : The command completed successfully, but no data was returned.
 * PGRES_TUPLES_OK : The query executed successfully and returned tuples (rows).
 */
	class pgSQL *pgsql= checkMajordomepgSQL(L);
	const char *req = luaL_checkstring(L, 2);

	if(!pgsql->asConnectionString()){
		SelLog->Log('E', "[%s] no connection string", pgsql->getNameC());
		lua_pushstring(L, "no connection string");
		lua_error(L);
		return 0;
	}

	if(debug && !pgsql->isQuiet())
		SelLog->Log('T', "['%s'] executing : %s", pgsql->getNameC(), req);

	if(!pgsql->connect())
		return false;

	int ret = 0;	// number of returns
	PGresult *res = PQexec(pgsql->getConnection(), req);
	switch(PQresultStatus(res)){
	case PGRES_COMMAND_OK :
		if(debug && !pgsql->isQuiet())
			SelLog->Log('T', "['%s'] No return", pgsql->getNameC());
		break;
	case PGRES_TUPLES_OK :
		if(debug && !pgsql->isQuiet())
			SelLog->Log('T', "['%s'] %d returned", pgsql->getNameC(), PQntuples(res));

		lua_createtable(L,PQntuples(res),0);
		for(auto i=0; i<PQntuples(res); ++i){
			lua_pushnumber(L, i+1);							// Row index
			lua_createtable(L, PQnfields(res), 0);								// fields
			for(auto j=0; j<PQnfields(res); ++j){
				lua_pushnumber(L, j+1);						// field index
				lua_pushstring(L, PQgetvalue(res, i, j));	// Push the value
				lua_rawset(L, -3);							// store in the sub table
			}
			lua_rawset(L, -3);							// store in the sub table
		}
		ret = 1;
		break;
	default :
		SelLog->Log('E', "['%s'] %s", pgsql->getNameC(), pgsql->lastError());
		break;
	}
	PQclear(res);

	pgsql->disconnect();
	return ret;
}

static int mmm_escapeliteral( lua_State *L ){
	class pgSQL *pgsql= checkMajordomepgSQL(L);
	const char *s = luaL_checkstring(L, 2);

	if(!pgsql->connect())
		return false;

	char *t = PQescapeLiteral(pgsql->getConnection(), s, strlen(s));
	lua_pushstring(L, t);

	PQfreemem(t);
	pgsql->disconnect();

	return 1;
}

static int mmm_escapeidentifier( lua_State *L ){
	class pgSQL *pgsql= checkMajordomepgSQL(L);
	const char *s = luaL_checkstring(L, 2);

	if(!pgsql->connect())
		return false;

	char *t = PQescapeIdentifier(pgsql->getConnection(), s, strlen(s));
	lua_pushstring(L, t);

	PQfreemem(t);
	pgsql->disconnect();

	return 1;
}

static const struct luaL_Reg MajpgSQLM [] = {
	{"getContainer", mmm_getContainer},
 	{"getName", mmm_getName},
	{"isEnabled", mmm_isEnabled},
	{"Enable", mmm_enabled},
	{"Disable", mmm_disable},
	{"doSQL", mmm_doSQL},
	{"EscapeLiteral", mmm_escapeliteral},
	{"EscapeString", mmm_escapeliteral},
	{"EscapeIdentifier", mmm_escapeidentifier},
	{NULL, NULL}
};

void pgSQL::initLuaInterface( lua_State *L ){
	SelLua->objFuncs( L, "MajordomepgSQL", MajpgSQLM );
	SelLua->libCreateOrAddFuncs( L, "MajordomepgSQL", MajpgSQLLib );
}
