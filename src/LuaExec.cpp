#include "Config.h"
#include "Helpers.h"
#include "LuaExec.h"

#include <cstring>
#include <cassert>

/*
extern "C" {
    #include "lualib.h"
    #include "lauxlib.h"
};
*/

LuaExec::LuaExec(const std::string &fch, std::string &where, std::string &name) : Object(fch, where, name) {
	assert( SelElasticStorage->init(&this->func) );	
}

lua_State *LuaExec::createLuaState(void){
	lua_State *L = luaL_newstate();
	if( !L ){
		SelLog->Log('E', "Unable to create a new Lua State for '%s' from '%s'", this->getNameC(), this->getWhereC() );
		return NULL;
	}
	luaL_openlibs(L);

	return L;
}

bool LuaExec::LoadFunc( lua_State *L, std::stringstream &buffer, const char *name ){
	int err;
	if( !!(err = luaL_loadbuffer( L, buffer.str().c_str(), buffer.str().size(), name ))){
		switch( err ){
		case LUA_ERRMEM :
			SelLog->Log('F', "Memory allocation error");
			return false;
		case LUA_ERRSYNTAX :
			SelLog->Log('F', lua_tostring(L, -1));
			return false;
		default :
			SelLog->Log('F', "luaL_loadbuffer() unknown error : %d", err);
			return false;
		}
	}

	if(lua_dump(L, SelElasticStorage->dumpwriter, this->getFunc()
#if LUA_VERSION_NUM > 501
		,1
#endif
	) != 0){
		SelLog->Log('F', "lua_dump() : %d", err);
		return false;
	}
	lua_pop(L,1);	// remove the function from the stack

	return true;
}

void LuaExec::readConfigDirective( std::string &l, bool &nameused ){
	MayBeEmptyString arg;

/* TODO */
	return Object::readConfigDirective(l, nameused);
}

bool LuaExec::canRun( void ){
	if( !this->isEnabled() ){
		if(verbose)
			SelLog->Log('T', "Task '%s' from '%s' is disabled", this->getNameC(), this->getWhereC() );
		return false;
	}

	return true;
}

bool LuaExec::feedbyNeeded( lua_State *L, bool require ){
	if(require){
		/* TODO */
	}

	for(auto &i : this->needed_task){
		try {
			class LuaTask *tsk = config.TasksList.at( i );
			class LuaTask **task = (class LuaTask **)lua_newuserdata(L, sizeof(class LuaTask *));
			assert(task);

			*task = tsk;
			luaL_getmetatable(L, "MajordomeTask");
			lua_setmetatable(L, -2);

			lua_setglobal(L, i.c_str());
		} catch( std::out_of_range &e ){	// Not found 
			return false;
		}
	}

	return true;
}

bool LuaExec::exec(lua_State *L, bool async){
	/* TODO */
	return false;
}
