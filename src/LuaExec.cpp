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

void LuaExec::feedState( lua_State *L, const char *name, const char *topic, const char *payload, bool tracker ){
	if( tracker ){	// Launched by a tracker
		lua_pushstring( L, name );	// Push the name of the tracker
		lua_setglobal( L, "MAJORDOME_TRACKER" );
		if( topic ){ // Launched when the tracker is stopped
			lua_pushstring( L, topic );	// Push the status
			lua_setglobal( L, "MAJORDOME_TRACKER_STATUS" );
		}
	} else if( payload ){	// Launched by a trigger
		lua_pushstring( L, name );	// Push the name of the trigger
		lua_setglobal( L, "MAJORDOME_TRIGGER" );
		if( topic ){	// Otherwise, it means it has been launched by a Lua script
			lua_pushstring( L, topic );	// Push the topic
			lua_setglobal( L, "MAJORDOME_TOPIC" );
			lua_pushstring( L, payload);	// and its payload
			lua_setglobal( L, "MAJORDOME_PAYLOAD" );
		}
	} else {	// Launched by a timer
		lua_pushstring( L, name );	// Push the name of the trigger
		lua_setglobal( L, "MAJORDOME_TIMER" );
	}
}

struct launchargs {
	lua_State *L;	// New thread Lua state
	LuaExec *task;	// task definition
};

static void *launchfunc(void *a){
	struct launchargs *arg = (struct launchargs *)a;	// To avoid further casting

	if(lua_pcall( arg->L, 0, 0, 0))
		publishLog('E', "Unable to create task '%s' from '%s' : %s", arg->task->getNameC(), arg->task->getWhereC(), lua_tostring(arg->L, -1));
	lua_close(arg->L);
	arg->task->finished();
	free(arg);
	return NULL;
}

bool LuaExec::execASync( const char *name, const char *topic, const char *payload, bool tracker ){
		 /* Create the new thread */
	struct launchargs *arg = new launchargs;
	arg->task = this;
#if 0	// Let the default handler working
	if( !arg ){
		publishLog('E', "Unable to create a new thread arguments for '%s' from '%s'", this->getNameC(), this->getWhereC() );
		this->finished();
		return false;
	}
#endif

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

	this->feedState( arg->L, name, topic, payload, tracker );

	if(verbose)
		publishLog('I', "running Task '%s' from '%s'", this->getNameC(), this->getWhereC() );

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
