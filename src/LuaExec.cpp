#include <cstring>
#include <cassert>

extern "C" {
    #include "lualib.h"
    #include "lauxlib.h"
};

#include "Config.h"
#include "Helpers.h"
#include "LuaExec.h"

LuaExec::LuaExec(){
	assert( SelElasticStorage->init(&this->func) );	
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

	/***** 
	 * Slave threads
	 ****/

void LuaExec::feedState( lua_State *L, const char *name, const char *topic, const char *payload, bool tracker, const char *trkstatus ){

	if( !name )	// No argument provide (launched at startup)
		return;

	if( topic ){	// If launched by a message receiving
		lua_pushstring( L, name);	// Push the name
		lua_setglobal( L, "MAJORDOME_TOPIC_NAME" );
		lua_pushstring( L, topic );	// Push the topic
		lua_setglobal( L, "MAJORDOME_TOPIC" );
		lua_pushstring( L, payload);	// and its payload
		lua_setglobal( L, "MAJORDOME_PAYLOAD" );
	}

	if( tracker ){	// Launched by a tracker
		lua_pushstring( L, name );	// Push the name of the tracker
		lua_setglobal( L, "MAJORDOME_TRACKER" );

		if(trkstatus){
			lua_pushstring( L, trkstatus );	// Push the name of the tracker
			lua_setglobal( L, "MAJORDOME_TRACKER_STATUS" );
		}
	} else if( !payload ){	// Launched by a timer
		lua_pushstring( L, name );	// Push the name of the trigger
		lua_setglobal( L, "MAJORDOME_TIMER" );
	} else { // Launched by a trigger
		lua_pushstring( L, name );	// Push the name of the trigger
		lua_setglobal( L, "MAJORDOME_TRIGGER" );
	}
}

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

bool LuaExec::execAsync( const char *name, const char *topic, const char *payload, bool tracker, const char *trkstatus ){
		 /* Create the new thread */
	struct launchargs *arg = new launchargs;
	arg->task = this;

	arg->L = luaL_newstate();
	if( !arg->L ){
		SelLog->Log('E', "Unable to create a new Lua State for '%s' from '%s'", this->getNameC(), this->getWhereC() );
		this->finished();
		delete arg;
		return false;
	}

	luaL_openlibs(arg->L);
	SelLua->ApplyStartupFunc(arg->L);

	int err;
	if( (err = SelElasticStorage->loadsharedfunction( arg->L, this->getFunc() )) ){
		SelLog->Log('E', "Unable to create task '%s' from '%s' : %s", this->getNameC(), this->getWhereC(), (err == LUA_ERRSYNTAX) ? "Syntax error" : "Memory error" );
		lua_close( arg->L );
		this->finished();
		delete arg;
		return false;
	}
	this->feedState( arg->L, name, topic, payload, tracker, trkstatus );

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

bool LuaExec::execSync( const char *name, const char *topic, const char *payload, bool tracker, enum boolRetCode *rc ){
	lua_State *L = luaL_newstate();
	if( !L ){
		SelLog->Log('E', "Unable to create a new Lua State for '%s' from '%s'", this->getNameC(), this->getWhereC() );
		return false;
	}

	luaL_openlibs(L);
	SelLua->ApplyStartupFunc(L);

	int err;
	if( (err = SelElasticStorage->loadsharedfunction( L, this->getFunc() )) ){
		SelLog->Log('E', "Unable to create task '%s' from '%s' : %s", this->getNameC(), this->getWhereC(), (err == LUA_ERRSYNTAX) ? "Syntax error" : "Memory error" );
		lua_close( L );
		return false;
	}

	this->feedState( L, name, topic, payload, tracker );

	if(verbose && !this->isQuiet())
		SelLog->Log('T', "Sync running Task '%s' from '%s'", this->getNameC(), this->getWhereC() );

	if(lua_pcall( L, 0, 1, 0))
		SelLog->Log('E', "Can't execute task '%s' from '%s' : %s", this->getNameC(), this->getWhereC(), lua_tostring(L, -1));

	if(rc){
		*rc = boolRetCode::RCnil;
		if(lua_isboolean(L, -1))
			*rc = lua_toboolean(L, -1) ? boolRetCode::RCtrue : boolRetCode::RCfalse;
	}

	lua_close(L);

	return true;
}
