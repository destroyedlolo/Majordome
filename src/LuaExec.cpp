#include "Config.h"
#include "Helpers.h"
#include "LuaExec.h"

#include <cstring>
#include <cassert>
#include <cmath>

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

	if(!!(arg = striKWcmp( l, "-->> need_task=" ))){
			/* No way to test if the task exists or not (as it could be
			 * defined afterward. Will be part of sanity checks
			 */
		if(verbose)
			SelLog->Log('C', "\t\tAdded needed task '%s'", arg.c_str());
		this->addNeededTask( arg );
		return;
	}

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
			SelLog->Log('E', "[%s] Needed task '%s' doesn't exist", this->getNameC(), i.c_str() );
			return false;
		}
	}

	return true;
}

	/* Executing */

struct launchargs {
	lua_State *L;	// New thread Lua state
	LuaExec *task;	// task definition
};

static void *launchfunc(void *a){
	struct launchargs *arg = (struct launchargs *)a;	// To avoid further casting

	if(lua_pcall( arg->L, 0, 0, 0))
		SelLog->Log('E', "Can't execute task '%s' from '%s' : %s", arg->task->getNameC(), arg->task->getWhereC(), lua_tostring(arg->L, -1));
	lua_close(arg->L);
	arg->task->finished();
	delete arg;
	return NULL;
}

bool LuaExec::execAsync( lua_State *L ){
	struct launchargs *arg = new launchargs; // Create the new thread
	arg->task = this;
	arg->L = L;

	int err;
	if( (err = SelElasticStorage->loadsharedfunction( arg->L, this->getFunc() )) ){
		SelLog->Log('E', "Unable to create task '%s' from '%s' : %s", this->getNameC(), this->getWhereC(), (err == LUA_ERRSYNTAX) ? "Syntax error" : "Memory error" );
		this->finished();
		lua_close( arg->L );
		delete arg;
		return false;
	}

	if(verbose && !this->isQuiet())
		SelLog->Log('T', "Async running Task '%s' from '%s'", this->getNameC(), this->getWhereC() );

	pthread_t tid;	// No need to be kept
	if(pthread_create( &tid, &thread_attr, launchfunc,  arg) < 0){
		SelLog->Log('E', "Unable to create task '%s' from '%s' : %s", this->getNameC(), this->getWhereC(), strerror(errno));
		this->finished();
		lua_close( arg->L );
		delete arg;
		return false;
	}

	return true;
}

bool LuaExec::execSync(lua_State *L, enum boolRetCode *rc, std::string *rs, lua_Number *retn){
	int err;
	if( (err = SelElasticStorage->loadsharedfunction( L, this->getFunc() )) ){
		SelLog->Log('E', "Unable to create task '%s' from '%s' : %s", this->getNameC(), this->getWhereC(), (err == LUA_ERRSYNTAX) ? "Syntax error" : "Memory error" );
		lua_close( L );
		return false;
	}

	if(verbose && !this->isQuiet())
		SelLog->Log('T', "Sync running Task '%s' from '%s'", this->getNameC(), this->getWhereC() );

	if(lua_pcall( L, 0, 2, 0))
		SelLog->Log('E', "Can't execute task '%s' from '%s' : %s", this->getNameC(), this->getWhereC(), lua_tostring(L, -1));

		/* -1 : numeric value if provided
		 * -2 : string value or RC
		 */
	if(rc){
		*rc = boolRetCode::RCnil;
		if(lua_isboolean(L, -2))
			*rc = lua_toboolean(L, -2) ? boolRetCode::RCtrue : boolRetCode::RCfalse;
	}

	if(rs){
		*rs = "";
		if(lua_isstring(L, -2))
			*rs = lua_tostring(L, -2);
	}

	if(retn){
		*retn = NAN;
		if(lua_isnumber(L, -1))
			*retn = lua_tonumber(L, -1);
	}

	return true;
}
