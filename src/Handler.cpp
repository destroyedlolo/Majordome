#include "Helpers.h"
#include "Handler.h"

bool Handler::exec(void){
	lua_State *L = LuaExec::createLuaState();
	if(!L)	// Can't create the state
		return false;

	threadEnvironment(L);	// Feed environment with generals
	if(!this->feedbyNeeded(L)){
		this->finished();
		lua_close( L );
		return false;
	}

	return this->exec(L);
}

bool Handler::exec(lua_State *L){
	if(!this->canRun()){
		lua_close( L );
		return false;
	}

	bool ret = this->LuaExec::execAsync(L);

	return ret;
}
