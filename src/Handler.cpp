#include "Helpers.h"
#include "Handler.h"

bool Handler::exec(void){
	if(!this->canRun())
		return false;

	/* ***
	 * Right now, the tasks is marked as running 
	 * ***/

	lua_State *L = LuaExec::createLuaState();
	if(!L){	// Can't create the state
		this->finished();
		return false;
	}

	threadEnvironment(L);	// Feed environment with generals
	if(!this->feedbyNeeded(L)){
		this->finished();
		lua_close( L );
		return false;
	}

	this->feedState(L);

	return this->LuaExec::execAsync(L);
}

bool Handler::exec(lua_State *L, enum boolRetCode *rc, std::string *rs, lua_Number *retn){
	if(!this->canRun())
		return false;

	/* ***
	 * Right now, the tasks is marked as running 
	 * ***/

	this->feedState(L);

	return this->LuaExec::execSync(L, rc,rs,retn);
}
