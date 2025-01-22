#include "Config.h"
#include "Helpers.h"
#include "LuaTask.h"

#include <iostream>
#include <fstream>

#include <cstring>
#include <cassert>

LuaTask::LuaTask( const std::string &fch, std::string &where, std::string &name, lua_State *L ) : Object(fch, where, name), LuaExec(fch, where, name), Handler(fch, where, name), once(false), running_access(PTHREAD_MUTEX_INITIALIZER), running(false), runatstartup(false){
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

			this->readConfigDirective(l, nameused);
		} while(true);


		/*
		 * Reading the remaining of the script and keep it as 
		 * an Lua's script
		 */

		buffer << file.rdbuf();
		file.close();
	} catch(const std::ifstream::failure &e){
		if(!file.eof()){
			SelLog->Log('F', "%s : %s", fch.c_str(), strerror(errno) );
			exit(EXIT_FAILURE);
		}
	}

	if( !this->LoadFunc( L, buffer, this->name.c_str() ))
		exit(EXIT_FAILURE);
}

void LuaTask::readConfigDirective( std::string &l, bool &nameused ){
	MayBeEmptyString arg;

	if( l == "-->> RunAtStartup" ){
		if(verbose)
			SelLog->Log('C', "\t\tRun at startup");
		this->setRunAtStartup( true );
	} else 
		LuaExec::readConfigDirective(l, nameused);
}

	/* ***
	 * Execution
	 * ***/

bool LuaTask::canRun( void ){
	if(!this->LuaExec::canRun())
		return false;

	if( !this->once )	// Nothing else can prevent it to run
		return true;

	pthread_mutex_lock( &this->running_access );
	if( this->running ){
		pthread_mutex_unlock( &this->running_access );
		if(verbose)
			SelLog->Log('T', "Task '%s' from '%s' is already running", this->getNameC(), this->getWhereC() );		
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

void LuaTask::feedState(lua_State *L){
	try {
		class LuaTask *tsk = config.TasksList.at( this->getNameC() );
		class LuaTask **task = (class LuaTask **)lua_newuserdata(L, sizeof(class Tracker *));
		assert(task);

		*task = tsk;
		luaL_getmetatable(L, "MajordomeTask");
		lua_setmetatable(L, -2);
		lua_setglobal( L, "MAJORDOME_Myself" );
	} catch( std::out_of_range &e ){	// Not found 
		SelLog->Log('F', "Can't find task '%s'", this->getNameC() );
		exit(EXIT_FAILURE);
	}
}


	/* ****
	 * Lua exposed functions
	 * ****/

static class LuaTask *checkMajordomeTask(lua_State *L){
	class LuaTask **r = (class LuaTask **)SelLua->testudata(L, 1, "MajordomeTask");
	luaL_argcheck(L, r != NULL, 1, "'MajordomeTask' expected");
	return *r;
}

static int mtsk_find(lua_State *L){
/* Find a task
 * 1 : task to find
 * 2 : true if it has to fail if not found
 */
	const char *name = luaL_checkstring(L, 1);
	bool tofail =  lua_toboolean(L, 2);

	try {
		class LuaTask *tsk = config.TasksList.at( name );
		class LuaTask **task = (class LuaTask **)lua_newuserdata(L, sizeof(class LuaTask *));
		assert(task);

		*task = tsk;
		luaL_getmetatable(L, "MajordomeTask");
		lua_setmetatable(L, -2);

		return 1;
	} catch( std::out_of_range &e ){	// Not found 
		if( tofail )
			return luaL_error( L, "Can't find \"%s\" task", name );
		return 0;
	}
}

static const struct luaL_Reg MajTaskLib [] = {
	{"find", mtsk_find},
	{NULL, NULL}
};

static int mtsk_launch(lua_State *L){
	class LuaTask *task = checkMajordomeTask(L);

	if( !task->canRun() )
		return 0;

	if(!task->feedbyNeeded(L))	// Feed with local needs
		return 0;

	task->execSync(L);
	task->finished();

	return 0;
}

static int mtsk_getContainer(lua_State *L){
	class LuaTask *task = checkMajordomeTask(L);
	lua_pushstring( L, task->getWhereC() );
	return 1;
}

static int mtsk_getName(lua_State *L){
	class LuaTask *task = checkMajordomeTask(L);
	lua_pushstring( L, task->getName().c_str() );
	return 1;
}

static int mtsk_enabled( lua_State *L ){
	class LuaTask *task = checkMajordomeTask(L);
	task->enable();
	return 0;
}

static int mtsk_disable( lua_State *L ){
	class LuaTask *task = checkMajordomeTask(L);
	task->disable();
	return 0;
}

static int mtsk_isEnabled( lua_State *L ){
	class LuaTask *task = checkMajordomeTask(L);
	lua_pushboolean( L, task->isEnabled() );
	return 1;
}

static const struct luaL_Reg MajTaskM [] = {
	{"Launch", mtsk_launch},
	{"getContainer", mtsk_getContainer},
	{"getName", mtsk_getName},
	{"isEnabled", mtsk_isEnabled},
	{"Enable", mtsk_enabled},
	{"Disable", mtsk_disable},
	{NULL, NULL}
};

void LuaTask::initLuaInterface( lua_State *L ){
	SelLua->objFuncs( L, "MajordomeTask", MajTaskM );
	SelLua->libCreateOrAddFuncs( L, "MajordomeTask", MajTaskLib );
}
