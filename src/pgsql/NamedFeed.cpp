#include "NamedFeed.h"
#include "pgSQL.h"
#include "../Config.h"
#include "../Helpers.h"

#include <fstream>

#include <cstring>
#include <cassert>

NamedFeed::NamedFeed(const std::string &fch, std::string &where, std::string &name, lua_State *L) : Object(fch, where, name), Handler(fch, where, name), Feed(fch, where, name, L){
}

void NamedFeed::feedState( lua_State *L ){
	class NamedFeed **feed= (class NamedFeed **)lua_newuserdata(L, sizeof(class NamedFeed *));
	assert(feed);

	lua_pushstring( L, this->getNameC() );	// Push the name of the tracker
	lua_setglobal( L, "MAJORDOME_NAMEDFEED" );

	*feed = this;
	luaL_getmetatable(L, "MajordomeNamedFeed");
	lua_setmetatable(L, -2);
	lua_setglobal( L, "MAJORDOME_Myself" );
}

bool NamedFeed::execAsync(lua_State *L){
	LuaExec::boolRetCode rc;
	std::string rs("orphaned data");
	lua_Number val;

	bool r = this->LuaExec::execSync(L, &rs, &rc, &val);
	switch(rc){
	case LuaExec::boolRetCode::RCfalse :	// data rejected
		SelLog->Log('D', "['%s'] Data rejected", this->getNameC());
		lua_close(L);
		return r;
	
	case LuaExec::boolRetCode::RCforced :	// value provided
		break;

	default:	// Take the payload as data
		lua_getglobal(L, "MAJORDOME_PAYLOAD");
		if(lua_isnumber(L, -1))
			val = lua_tonumber(L, -1);
		else {
			SelLog->Log('E', "['%s'] can't find MAJORDOME_PAYLOAD as number", this->getNameC());
			lua_close(L);
			return r;
		}
		break;
	}

	if(debug && !this->isQuiet())
		SelLog->Log('T', "['%s'/'%s'] accepting %.0f", this->getNameC(),rs.c_str(), val);

		/* Build SQL request */
	if(!this->connect()){
		lua_close(L);
		return false;
	}

	char *t;
	std::string cmd("INSERT INTO ");
	cmd += (t = PQescapeIdentifier(this->conn, this->getTableName(), strlen(this->getTableName())));
	PQfreemem(t);

	cmd += " VALUES ( now(), ",
	cmd += (t = PQescapeLiteral(this->conn, rs.c_str(), rs.length()));
	PQfreemem(t);
	
	cmd += ", ";
	cmd += std::to_string(val);
	cmd += " )";

	if(!this->doSQL(cmd.c_str()))
		SelLog->Log('E', "['%s'] %s", this->getNameC(), this->lastError());

	this->disconnect();
	lua_close(L);
	return r;
}

	/*****
	 * Lua exposed functions
	 *****/

static class NamedFeed *checkMajordomeNamedFeed(lua_State *L){
	class NamedFeed **r = (class NamedFeed **)SelLua->testudata(L, 1, "MajordomeNamedFeed");
	luaL_argcheck(L, r != NULL, 1, "'MajordomeNamedFeed' expected");
	return *r;
}

static int mmm_find(lua_State *L){
	const char *name = luaL_checkstring(L, 1);

	try {
		class NamedFeed *f = config.NamedFeedsList.at( name );
		class NamedFeed **namedfeed = (class NamedFeed **)lua_newuserdata(L, sizeof(class NamedFeed *));
		assert(namedfeed);

		*namedfeed = f;
		luaL_getmetatable(L, "MajordomeNamedFeed");
		lua_setmetatable(L, -2);

		return 1;
	} catch( std::out_of_range &e ){	// Not found 
		return 0;
	}
}

static const struct luaL_Reg MajNamedFeedLib [] = {
	{"find", mmm_find},
	{NULL, NULL}
};

static int mmm_getContainer(lua_State *L){
	class NamedFeed *namedfeed= checkMajordomeNamedFeed(L);
	lua_pushstring( L, namedfeed->getWhereC() );
	return 1;
}

static int mmm_getName(lua_State *L){
	class NamedFeed *namedfeed= checkMajordomeNamedFeed(L);
	lua_pushstring( L, namedfeed->getName().c_str() );
	return 1;
}

static int mmm_isEnabled( lua_State *L ){
	class NamedFeed *namedfeed= checkMajordomeNamedFeed(L);
	lua_pushboolean( L, namedfeed->isEnabled() );
	return 1;
}

static int mmm_enabled( lua_State *L ){
	class NamedFeed *namedfeed= checkMajordomeNamedFeed(L);
	namedfeed->enable();
	return 0;
}

static int mmm_disable( lua_State *L ){
	class NamedFeed *namedfeed= checkMajordomeNamedFeed(L);
	namedfeed->disable();
	return 0;
}

static const struct luaL_Reg MajNamedFeedM [] = {
	{"getContainer", mmm_getContainer},
 	{"getName", mmm_getName},
	{"isEnabled", mmm_isEnabled},
	{"Enable", mmm_enabled},
	{"Disable", mmm_disable},
	{NULL, NULL}
};

void NamedFeed::initLuaInterface( lua_State *L ){
	SelLua->objFuncs( L, "MajordomeNamedFeed", MajNamedFeedM );
	SelLua->libCreateOrAddFuncs( L, "MajordomeNamedFeed", MajNamedFeedLib );
}
