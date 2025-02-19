#include "NamedFeed.h"
#include "pgSQL.h"
#include "../Config.h"
#include "../Helpers.h"

#include <fstream>

#include <cstring>
#include <cassert>

void NamedFeed::feedState( lua_State *L ){
	class Feed **feed= (class Feed **)lua_newuserdata(L, sizeof(class Feed *));
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
	std::string rs;
	lua_Number val = NAN;

	bool r = this->LuaExec::execSync(L, &rc, &rs, &val);
	if( rc != LuaExec::boolRetCode::RCfalse ){
		if(isnan(val)){
			lua_getglobal(L, "MAJORDOME_PAYLOAD");
			if(lua_isnumber(L, -1))
				val = lua_tonumber(L, -1);
			else
				SelLog->Log('E', "['%s'] can't find MAJORDOME_PAYLOAD variable", this->getNameC());
		}
		
		if(val != NAN){
			if(debug && !this->isQuiet())
				SelLog->Log('T', "['%s'/'%s'] accepting %.0f", this->getNameC(),rs.c_str(), val);
		} else
			SelLog->Log('E', "['%s'] can't find MAJORDOME_PAYLOAD variable", this->getNameC());
	} else
		SelLog->Log('D', "['%s'] Data rejected", this->getNameC());

	lua_close(L);

	return r;
}
