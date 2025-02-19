#include "NamedFeed.h"
#include "pgSQL.h"
#include "../Config.h"
#include "../Helpers.h"

#include <fstream>

#include <cstring>
#include <cassert>

void NamedFeed::feedState( lua_State *L ){
	class NamedFeed **feed= (class NamedFeed **)lua_newuserdata(L, sizeof(class NamedFeed *));
	assert(feed);

	lua_pushstring( L, this->getNameC() );	// Push the name of the tracker
	lua_setglobal( L, "MAJORDOME_NAMEDFEED" );

	*feed = this;
	luaL_getmetatable(L, "MajordomeFeed");
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
	cmd += (t = PQescapeIdentifier(this->conn, rs.c_str(), rs.length()));
	PQfreemem(t);
	
	cmd += ", ";
	cmd += std::to_string(val);
	cmd += " )";

puts(cmd.c_str());
#if 0
	if(!this->doSQL(cmd.c_str()))
		SelLog->Log('E', "['%s'] %s", this->getNameC(), this->lastError());
#endif

	this->disconnect();
	lua_close(L);
	return r;
}
