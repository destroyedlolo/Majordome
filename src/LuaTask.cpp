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

LuaTask::LuaTask( Config &cfg, const std::string &fch, std::string &where, std::string &name, lua_State *L ) : once( false ), running_access(PTHREAD_MUTEX_INITIALIZER), running(false){
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
			} else if( !!(arg = striKWcmp( l, "-->> waitfor=" ))){
				Config::TimerElements::iterator timer;
				if( (timer = cfg.Timerslist.find(arg)) != cfg.Timerslist.end()){
					publishLog('C', "\t\tAdded to timer '%s'", arg.c_str());
	 				timer->second.addTasks( this->getName() );
					nameused = true;
				} else {
					publishLog('F', "\t\ttimer '%s' is not (yet ?) defined", arg.c_str());
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
	arg->task->finished();
	free(arg);
	return NULL;
}

bool LuaTask::exec( const char *name, const char *topic, const char *payload ){
		/* Check if the task can be launched */
	if( !this->isEnabled() ){
		if(verbose)
			publishLog('I', "Task '%s' from '%s' is disabled", this->getNameC(), this->getWhereC() );
		return false;
	}

	if( !this->canRun() ){
		if(verbose)
			publishLog('I', "Task '%s' from '%s' is already running", this->getNameC(), this->getWhereC() );
		return false;
	}

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
	if( (err = loadsharedfunction( arg->L, &(this->func) )) ){
		publishLog('E', "Unable to create task '%s' from '%s' : %s", this->getNameC(), this->getWhereC(), (err == LUA_ERRSYNTAX) ? "Syntax error" : "Memory error" );
		lua_close( arg->L );
		this->finished();
		free( arg );
		return false;
	}

	if( payload ){	// Launched by a trigger
		lua_pushstring( arg->L, name );	// Push the name of the trigger
		lua_setglobal( arg->L, "MAJORDOME_TRIGGER" );
		lua_pushstring( arg->L, topic );	// Push the topic
		lua_setglobal( arg->L, "MAJORDOME_TOPIC" );
		lua_pushstring( arg->L, payload);	// and its payload
		lua_setglobal( arg->L, "MAJORDOME_PAYLOAD" );
	} else {	// Launched by a timer
		lua_pushstring( arg->L, name );	// Push the name of the trigger
		lua_setglobal( arg->L, "MAJORDOME_TIMER" );
	}

	if(verbose)
		publishLog('I', "running Task '%s' from '%s'", this->getNameC(), this->getWhereC() );

	pthread_t tid;	// No need to be kept
	if(pthread_create( &tid, &thread_attr, launchfunc,  arg) < 0){
		publishLog('E', "Unable to create task '%s' from '%s' : %s", this->getNameC(), this->getWhereC(), strerror(errno));
		this->finished();
		lua_close( arg->L );
		free( arg );
	}

	return true;
}

bool LuaTask::canRun( void ){
	if( !this->once )
		return true;

	pthread_mutex_lock( &this->running_access );
	if( this->running ){
		pthread_mutex_unlock( &this->running_access );
		return false;
	}
	this->running = true;
	pthread_mutex_unlock( &this->running_access );

	return true;
}

void LuaTask::finished( void ){
	if( this->once ){
		pthread_mutex_lock( &this->running_access );
		this->running = false;
		pthread_mutex_unlock( &this->running_access );
	}
}

	/*****
	 * Lua exposed functions
	 *****/

static class LuaTask *checkMajordomeTask(lua_State *L){
	class LuaTask **r = (class LuaTask **)luaL_testudata(L, 1, "MajordomeTask");
	luaL_argcheck(L, r != NULL, 1, "'MajordomeTask' expected");
	return *r;
}

static int mtsk_find(lua_State *L){
	const char *name = luaL_checkstring(L, 1);

	try {
		LuaTask &tsk = config.TasksList.at( name );
		class LuaTask **task = (class LuaTask **)lua_newuserdata(L, sizeof(class LuaTask *));
		assert(task);

		*task = &tsk;
		luaL_getmetatable(L, "MajordomeTask");
		lua_setmetatable(L, -2);

		return 1;
	} catch( std::out_of_range &e ){	// Not found 
		return 0;
	}
}

static int mtsk_getContainer(lua_State *L){
	class LuaTask *task = checkMajordomeTask(L);

	lua_pushstring( L, task->getWhereC() );
	return 1;
}

static const struct luaL_Reg MajTaskLib [] = {
	{"find", mtsk_find},
	{NULL, NULL}
};

static const struct luaL_Reg MajTaskM [] = {
	{"getContainer", mtsk_getContainer},
	{NULL, NULL}
};

int LuaTask::initLuaObject( lua_State *L ){
	libSel_objFuncs( L, "MajordomeTask", MajTaskM );
	libSel_libFuncs( L, "MajordomeTask", MajTaskLib );

	return 1;
}
