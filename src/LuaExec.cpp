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

	/***** 
	 * Slave threads
	 ****/

void LuaExec::feedState( lua_State *L, const char *name, const char *topic, const char *payload, bool tracker, const char *trkstatus ){

	if( !name )	// No argument provide (launched at startup)
		return;

	if( topic ){	// If launched by a message receiving
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
		publishLog('E', "Can't execute task '%s' from '%s' : %s", arg->task->getNameC(), arg->task->getWhereC(), lua_tostring(arg->L, -1));
	lua_close(arg->L);
	arg->task->finished();
	free(arg);
	return NULL;
}

bool LuaExec::execAsync( const char *name, const char *topic, const char *payload, bool tracker, const char *trkstatus ){
		 /* Create the new thread */
	struct launchargs *arg = new launchargs;
	arg->task = this;

	arg->L = luaL_newstate();
	if( !arg->L ){
		publishLog('E', "Unable to create a new Lua State for '%s' from '%s'", this->getNameC(), this->getWhereC() );
		this->finished();
		free( arg );
		return false;
	}

	luaL_openlibs(arg->L);
	libSel_ApplyStartupFunc( luainitfunc, arg->L );

	int err;
	if( (err = loadsharedfunction( arg->L, this->getFunc() )) ){
		publishLog('E', "Unable to create task '%s' from '%s' : %s", this->getNameC(), this->getWhereC(), (err == LUA_ERRSYNTAX) ? "Syntax error" : "Memory error" );
		lua_close( arg->L );
		this->finished();
		free( arg );
		return false;
	}
	this->feedState( arg->L, name, topic, payload, tracker, trkstatus );

	if(verbose && !this->isQuiet())
		publishLog('T', "Async running Task '%s' from '%s'", this->getNameC(), this->getWhereC() );

	pthread_t tid;	// No need to be kept
	if(pthread_create( &tid, &thread_attr, launchfunc,  arg) < 0){
		publishLog('E', "Unable to create task '%s' from '%s' : %s", this->getNameC(), this->getWhereC(), strerror(errno));
		this->finished();
		lua_close( arg->L );
		free( arg );
		return false;
	}

	return true;
}

bool LuaExec::execSync( const char *name, const char *topic, const char *payload, bool tracker ){
	lua_State *L = luaL_newstate();
	if( !L ){
		publishLog('E', "Unable to create a new Lua State for '%s' from '%s'", this->getNameC(), this->getWhereC() );
		return false;
	}

	luaL_openlibs(L);
	libSel_ApplyStartupFunc( luainitfunc, L );

	int err;
	if( (err = loadsharedfunction( L, this->getFunc() )) ){
		publishLog('E', "Unable to create task '%s' from '%s' : %s", this->getNameC(), this->getWhereC(), (err == LUA_ERRSYNTAX) ? "Syntax error" : "Memory error" );
		lua_close( L );
		return false;
	}

	this->feedState( L, name, topic, payload, tracker );

	if(verbose && !this->isQuiet())
		publishLog('T', "Async running Task '%s' from '%s'", this->getNameC(), this->getWhereC() );

	if(lua_pcall( L, 0, 0, 0))
		publishLog('E', "Can't execute task '%s' from '%s' : %s", this->getNameC(), this->getWhereC(), lua_tostring(L, -1));
	lua_close(L);

	return true;
}
