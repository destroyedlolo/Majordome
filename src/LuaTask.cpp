#include <iostream>
#include <fstream>

#include <cstring>
#include <cassert>

extern "C" {
    #include "lualib.h"
    #include "lauxlib.h"
};

#include "Config.h"
#include "Helpers.h"
#include "LuaTask.h"
#include "Tracker.h"

LuaTask::LuaTask( Config &cfg, const std::string &fch, std::string &where, std::string &name, lua_State *L ) : once( false ), running_access(PTHREAD_MUTEX_INITIALIZER), running(false){
	if(verbose)
		publishLog('L', "\t'%s'", fch.c_str());

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
					publishLog('F', "\t\tName can be changed only before listen, until or waitfor directives");
					exit(EXIT_FAILURE);
				}

				this->name = name = arg;
				if(verbose)
					publishLog('C', "\t\tChanging name to '%s'", name.c_str());
			} else if( !!(arg = striKWcmp( l, "-->> listen=" ))){
				Config::TopicElements::iterator topic;
				if( (topic = cfg.TopicsList.find(arg)) != cfg.TopicsList.end()){
					if(verbose)
						publishLog('C', "\t\tAdded to topic '%s'", arg.c_str());
	 				topic->second.addTask( this->getName() );
					nameused = true;
				} else {
					publishLog('F', "\t\tTopic '%s' is not (yet ?) defined", arg.c_str());
					exit(EXIT_FAILURE);
				}
			} else if( !!(arg = striKWcmp( l, "-->> when=" ))){
				Config::TimerElements::iterator timer;
				if( (timer = cfg.TimersList.find(arg)) != cfg.TimersList.end()){
					if(verbose)
						publishLog('C', "\t\tAdded to timer '%s'", arg.c_str());
	 				timer->second.addTask( this->getName() );
					nameused = true;
				} else {
					publishLog('F', "\t\ttimer '%s' is not (yet ?) defined", arg.c_str());
					exit(EXIT_FAILURE);
				}
			} else if( !!(arg = striKWcmp( l, "-->> waitfor=" ))){
				Config::EventElements::iterator event;
				if( (event = cfg.EventsList.find(arg)) != cfg.EventsList.end()){
					if(verbose)
						publishLog('C', "\t\tAdded to event '%s'", arg.c_str());
	 				event->second.addTask( this->getName() );
					nameused = true;
				} else {
					publishLog('F', "\t\tEvent '%s' is not (yet ?) defined", arg.c_str());
					exit(EXIT_FAILURE);
				}
			} else if( !!(arg = striKWcmp( l, "-->> whenDone=" ))){
				Config::TrackerElements::iterator tracker;
				if( (tracker = cfg.TrackersList.find(arg)) != cfg.TrackersList.end()){
					if(verbose)
						publishLog('C', "\t\tAdded to tracker '%s' as Done task", arg.c_str());
	 				tracker->second.addDone( this->getName() );
					nameused = true;
				} else {
					publishLog('F', "\t\tracker '%s' is not (yet ?) defined", arg.c_str());
					exit(EXIT_FAILURE);
				}
			} else if( !!(arg = striKWcmp( l, "-->> whenStarted=" ))){
				Config::TrackerElements::iterator tracker;
				if( (tracker = cfg.TrackersList.find(arg)) != cfg.TrackersList.end()){
					if(verbose)
						publishLog('C', "\t\tAdded to tracker '%s' as Started task", arg.c_str());
	 				tracker->second.addStarted( this->getName() );
					nameused = true;
				} else {
					publishLog('F', "\t\tracker '%s' is not (yet ?) defined", arg.c_str());
					exit(EXIT_FAILURE);
				}
			} else if( !!(arg = striKWcmp( l, "-->> whenStopped=" ))){
				Config::TrackerElements::iterator tracker;
				if( (tracker = cfg.TrackersList.find(arg)) != cfg.TrackersList.end()){
					if(verbose)
						publishLog('C', "\t\tAdded to tracker '%s' as Stopped task", arg.c_str());
	 				tracker->second.addStopped( this->getName() );
					nameused = true;
				} else {
					publishLog('F', "\t\tracker '%s' is not (yet ?) defined", arg.c_str());
					exit(EXIT_FAILURE);
				}
			} else if( l == "-->> once" ){
				if(verbose)
					publishLog('C', "\t\tOnly one instance is allowed to run (once)");
				this->setOnce( true );
			} else if( l == "-->> quiet" ){
				if(verbose)
					publishLog('C', "\t\tBe quiet");
				this->beQuiet();
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

	if( !this->LoadFunc( L, buffer, this->name.c_str() ))
		exit(EXIT_FAILURE);
}

	/*****
	 * Execute the code
	 *****/

bool LuaTask::exec( const char *name, const char *topic, const char *payload, bool tracker, const char *trkstatus ){
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

	bool ret = this->LuaExec::execAsync(name, topic, payload, tracker, trkstatus);
	if(!ret)
		this->finished();

	return ret;
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
/* Find a task
 * 1 : task to find
 * 2 : true if it has to fail if not found
 */
	const char *name = luaL_checkstring(L, 1);
	bool tofail =  lua_toboolean(L, 2);

	try {
		class LuaTask &tsk = config.TasksList.at( name );
		class LuaTask **task = (class LuaTask **)lua_newuserdata(L, sizeof(class LuaTask *));
		assert(task);

		*task = &tsk;
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

	if( !task->isEnabled() ){
		if(verbose)
			publishLog('I', "Task '%s' from '%s' is disabled", task->getNameC(), task->getWhereC() );
		return 0;
	}

	if( !task->canRun() ){
		if(verbose)
			publishLog('I', "Task '%s' from '%s' is already running", task->getNameC(), task->getWhereC() );
		return 0;
	}

	int err;
	if( (err = loadsharedfunction( L, task->getFunc() )) ){
		publishLog('E', "Unable to create task '%s' from '%s' : %s", task->getNameC(), task->getWhereC(), (err == LUA_ERRSYNTAX) ? "Syntax error" : "Memory error" );
		task->finished();
		return 0;
	}

	if(verbose)
		publishLog('I', "running Task '%s' from '%s'", task->getNameC(), task->getWhereC() );

	if(lua_pcall( L, 0, 0, 0))
		publishLog('E', "Unable to create task '%s' from '%s' : %s", task->getNameC(), task->getWhereC(), lua_tostring(L, -1));

	task->finished();
	return 0;
}

static int mtsk_getContainer(lua_State *L){
	class LuaTask *task = checkMajordomeTask(L);
	lua_pushstring( L, task->getWhereC() );
	return 1;
}

static int mtsk_enabled( lua_State *L ){
	class LuaTask *task = checkMajordomeTask(L);
	task->enable();
	return 1;
}

static int mtsk_disable( lua_State *L ){
	class LuaTask *task = checkMajordomeTask(L);
	task->disable();
	return 1;
}

static int mtsk_isEnabled( lua_State *L ){
	class LuaTask *task = checkMajordomeTask(L);
	lua_pushboolean( L, task->isEnabled() );
	return 1;
}

static const struct luaL_Reg MajTaskM [] = {
	{"Launch", mtsk_launch},
	{"getContainer", mtsk_getContainer},
	{"isEnabled", mtsk_isEnabled},
	{"Enable", mtsk_enabled},
	{"Disable", mtsk_disable},
	{NULL, NULL}
};

int LuaTask::initLuaObject( lua_State *L ){
	libSel_objFuncs( L, "MajordomeTask", MajTaskM );
	libSel_libFuncs( L, "MajordomeTask", MajTaskLib );

	return 1;
}
