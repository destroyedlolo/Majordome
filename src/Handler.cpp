#include "Helpers.h"
#include "Handler.h"
#include "HandlersExecutor.h"

lua_State *Handler::prepareExec(void){
	if(!this->canRun())
		return NULL;

	/* ***
	 * Right now, the tasks is marked as running 
	 * ***/

	lua_State *L = LuaExec::createLuaState();
	if(!L){	// Can't create the state
		this->finished();
		return NULL;
	}

	threadEnvironment(L);	// Feed environment with generals
	if(!this->feedbyNeeded(L)){
		this->finished();
		lua_close( L );
		return NULL;
	}

	this->feedState(L);

	return L;
}

bool Handler::exec(HandlersExecutor *h){
	lua_State *L;

	if(!(L = this->prepareExec()))
		return false;

	if(h)
		h->feedHandlersState(L);

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
