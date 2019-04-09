#include <cassert>

extern "C" {
    #include "lualib.h"
    #include "lauxlib.h"
};

#include "Config.h"
#include "Helpers.h"
#include "LuaExec.h"

LuaExec::LuaExec(){
	assert( EStorage_init(&this->func) );	
}

bool LuaExec::LoadFunc( lua_State *L, std::stringstream &buffer, const char *name ){
	int err;
	if( !!(err = luaL_loadbuffer( L, buffer.str().c_str(), buffer.str().size(), name ))){
		switch( err ){
		case LUA_ERRMEM :
			publishLog('F', "Memory allocation error");
			return false;
		case LUA_ERRSYNTAX :
			publishLog('F', lua_tostring(L, -1));
			return false;
		default :
			publishLog('F', "luaL_loadbuffer() unknown error : %d", err);
			return false;
		}
	}

	if(lua_dump(L, ssfc_dumpwriter, this->getFunc()
#if LUA_VERSION_NUM > 501
		,1
#endif
	) != 0){
		publishLog('F', "lua_dump() : %d", err);
		return false;
	}
	lua_pop(L,1);	// remove the function from the stack

	return true;
}
