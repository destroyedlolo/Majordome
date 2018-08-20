#include <iostream>
#include <fstream>
#include <sstream>	// stringstream

#include <cstring>
#include <cassert>

extern "C" {
    #include "lualib.h"
    #include "lauxlib.h"
};

#include "Config.h"
#include "Helpers.h"
#include "LuaTask.h"

LuaTask::LuaTask( Config &cfg, const std::string &fch, std::string &where, std::string &name, lua_State *L ) : once( false ){
	if(verbose)
		publishLog('L', "\t'%s'", fch.c_str());

	assert( EStorage_init(&this->func) );

	this->extrName( fch, name );
	this->name = name;
	this->where = where;

	/*
	 * Reading file's content
	 */

	std::stringstream buffer;
	std::ifstream file;
	file.exceptions ( std::ios::eofbit | std::ios::failbit );
	try {
		std::ifstream file(fch);
		std::streampos pos;

		bool nameused = false;	// if so, the name can't be changed anymore

		/*
		 * Reading header (Majordome's commands)
		 */

		do {
			std::string l;
			pos = file.tellg();

			std::getline( file, l);
			if( l.compare(0, 2, "--") ){	// End of comments
				file.seekg( pos );
				break;
			}

			MayBeEmptyString arg;
			if( !!(arg = striKWcmp( l, "-->> name=" ))){
				if( nameused ){
					publishLog('F', "\t\tName can be changed only before listen directives");
					exit(EXIT_FAILURE);
				}

				this->name = name = arg;
				if(verbose)
					publishLog('C', "\t\tChanging name to '%s'", name.c_str());
			} else if( !!(arg = striKWcmp( l, "-->> listen=" ))){
				Config::TopicElements::iterator topic;
				if( (topic = cfg.TopicsList.find(arg)) != cfg.TopicsList.end()){
					publishLog('C', "\t\tAdded to topic '%s'", arg.c_str());
	 				topic->second.addTasks( this->getName() );
					nameused = true;
				} else {
					publishLog('F', "\t\tTopic '%s' is not (yet ?) defined", arg.c_str());
					exit(EXIT_FAILURE);
				}
			} else if( l == "-->> once" ){
				if(verbose)
					publishLog('C', "\t\tOnly one instance is allowed to run (once)");
				this->setOnce( true );
			} else if( l == "-->> disabled" ){
				if(verbose)
					publishLog('C', "\t\tDisabled");
				this->disable();
			}
#if 0
else printf("Ignore '%s'\n", l.c_str());
#endif
		} while(true);


		/*
		 * Reading the remaining of the script and keep it as 
		 * an Lua's script
		 */

		buffer << file.rdbuf();
		file.close();
	} catch(const std::ifstream::failure &e){
		if(!file.eof()){
			publishLog('F', "%s : %s", fch.c_str(), strerror(errno) );
			exit(EXIT_FAILURE);
		}
	}

	int err;
	if( !!(err = luaL_loadbuffer( L, buffer.str().c_str(), buffer.str().size(), this->name.c_str() ))){
		switch( err ){
		case LUA_ERRMEM :
			publishLog('F', "Memory allocation error");
			exit(EXIT_FAILURE);
		case LUA_ERRSYNTAX :
			publishLog('F', lua_tostring(L, -1));
			exit(EXIT_FAILURE);
		default :
			publishLog('F', "luaL_loadbuffer() unknown error : %d", err);
			exit(EXIT_FAILURE);
		}
	}

	if(lua_dump(L, ssfc_dumpwriter, &this->func
#if LUA_VERSION_NUM > 501
		,1
#endif
	) != 0){
		publishLog('F', "lua_dump() : %d", err);
		exit(EXIT_FAILURE);
	}
	lua_pop(L,1);	// remove the function from the stack
}


	/***** 
	 * Slave threads
	 ****/

struct launchargs {
	lua_State *L;	// New thread Lua state
	LuaTask *task;	// task definition
};

static void *launchfunc(void *a){
	struct launchargs *arg = (struct launchargs *)a;	// To avoid further casting

	if(lua_pcall( arg->L, 0, 0, 0))
		publishLog('E', "Unable to create task '%s' from '%s' : %s", arg->task->getNameC(), arg->task->getWhereC(), lua_tostring(arg->L, -1));
	lua_close(arg->L);
	free(arg);
	return NULL;
}

bool LuaTask::exec( const char *name, const char *topic, const char *payload ){
		/* Check if the task can be launched */
	if(!this->isEnabled()){
		if(verbose)
			publishLog('I', "Task '%s' from '%s' is disabled", this->getNameC(), this->getWhereC() );
		return false;
	}

		 /* Create the new thread */
	struct launchargs *arg = new launchargs;
	arg->task = this;
#if 0	// Let the default handler working
	if( !arg ){
		publishLog('E', "Unable to create a new thread arguments for '%s' from '%s'", this->getNameC(), this->getWhereC() );
		return false;
	}
#endif

	arg->L = luaL_newstate();
	if( !arg->L ){
		publishLog('E', "Unable to create a new Lua State for '%s' from '%s'", this->getNameC(), this->getWhereC() );
		free( arg );
		return false;
	}

	luaL_openlibs(arg->L);
	libSel_ApplyStartupFunc( luainitfunc, arg->L );

	int err;
	if( (err = loadsharedfunction( arg->L, &(this->func) )) ){
		publishLog('E', "Unable to create task '%s' from '%s' : %s", this->getNameC(), this->getWhereC(), (err == LUA_ERRSYNTAX) ? "Syntax error" : "Memory error" );
		lua_close( arg->L );
		free( arg );
		return false;
	}

	lua_pushstring( arg->L, name );	// Push the name of the trigger
	lua_setglobal( arg->L, "MAJORDOME_TRIGGER" );
	lua_pushstring( arg->L, topic );	// Push the topic
	lua_setglobal( arg->L, "MAJORDOME_TOPIC" );
	lua_pushstring( arg->L, payload);	// and its payload
	lua_setglobal( arg->L, "MAJORDOME_PAYLOAD" );

	if(verbose)
		publishLog('I', "running Task '%s' from '%s'", this->getNameC(), this->getWhereC() );

	pthread_t tid;	// No need to be kept
	if(pthread_create( &tid, &thread_attr, launchfunc,  arg) < 0){
		publishLog('E', "Unable to create task '%s' from '%s' : %s", this->getNameC(), this->getWhereC(), strerror(errno));
		lua_close( arg->L );
		free( arg );
	}

	return true;
}