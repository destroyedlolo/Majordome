#include "Config.h"
#include "Helpers.h"
#include "LuaExec.h"

#include <cstring>
#include <cassert>
#include <cmath>

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

void LuaExec::readConfigDirective( std::string &l, std::string &name, bool &nameused ){
	MayBeEmptyString arg;

	if(!!(arg = striKWcmp( l, "-->> need_task=" ))){
			/* No way to test if the task exists or not (as it could be
			 * defined afterward. Will be part of sanity checks
			 */
		if(verbose)
			SelLog->Log('C', "\t\tAdded needed task '%s'", arg.c_str());
		this->addNeededTask( arg );
		return;
	} else if(!!(arg = striKWcmp( l, "-->> need_rendezvous=" ))){
		EventCollection::iterator event;
		if( (event = config.EventsList.find(arg)) != config.EventsList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tAdded needed rendezvous '%s'", arg.c_str());
			this->addNeededRendezVous(arg);
			return;
		} else {
			SelLog->Log('F', "\t\tRendezvous '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(!!(arg = striKWcmp( l, "-->> need_topic=" ))){
		TopicCollection::iterator topic;
		if( (topic = config.TopicsList.find(arg)) != config.TopicsList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tAdded needed topic '%s'", arg.c_str());
			this->addNeededTopic(arg);
			return;
		} else {
			SelLog->Log('F', "\t\tTopic '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(!!(arg = striKWcmp( l, "-->> require_topic=" ))){
		TopicCollection::iterator topic;
		if( (topic = config.TopicsList.find(arg)) != config.TopicsList.end()){
			if(!topic->second->toBeStored()){
				SelLog->Log('F', "Can't required \"%s\" topic : not stored", arg.c_str());
				exit(EXIT_FAILURE);
			}
			if(verbose)
				SelLog->Log('C', "\t\tAdded required topic '%s'", arg.c_str());
			this->addRequiredTopic(arg);
			return;
		} else {
			SelLog->Log('F', "\t\tTopic '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(!!(arg = striKWcmp( l, "-->> need_timer=" ))){
		TimerCollection::iterator timer;
		if( (timer = config.TimersList.find(arg)) != config.TimersList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tAdded needed timer '%s'", arg.c_str());
			this->addNeededTimer(arg);
			return;
		} else {
			SelLog->Log('F', "\t\ttimer '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(!!(arg = striKWcmp( l, "-->> need_tracker=" ))){
		TrackerCollection::iterator trk;
		if( (trk = config.TrackersList.find(arg)) != config.TrackersList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tAdded needed TrackersList '%s'", arg.c_str());
			this->addNeededTracker(arg);
			return;
		} else {
			SelLog->Log('F', "\t\ttimer '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(!!(arg = striKWcmp( l, "-->> need_minmax=" ))){
		MinMaxCollection::iterator minmax;
		if( (minmax = config.MinMaxList.find(arg)) != config.MinMaxList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tAdded needed minmax '%s'", arg.c_str());
			this->addNeededMinMax( arg );
			return;
		} else {
			SelLog->Log('F', "\t\tminmax '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(!!(arg = striKWcmp( l, "-->> need_namedminmax=" ))){
		NamedMinMaxCollection::iterator nminmax;
		if( (nminmax = config.NamedMinMaxList.find(arg)) != config.NamedMinMaxList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tAdded needed namedminmax '%s'", arg.c_str());
			this->addNeededNamedMinMax( arg );
			return;
		} else {
			SelLog->Log('F', "\t\tnamedminmax '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(!!(arg = striKWcmp( l, "-->> need_shutdown=" ))){
		ShutdownCollection::iterator shut;
		if( (shut = config.ShutdownsList.find(arg)) != config.ShutdownsList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tAdded needed Shutdown '%s'", arg.c_str());
			this->addNeededShutdown( arg );
			return;
		} else {
			SelLog->Log('F', "\t\tShutdown '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
#ifdef DBASE
	} else if(!!(arg = striKWcmp( l, "-->> need_feed=" ))){
		FeedCollection::iterator shut;
		if( (shut = config.FeedsList.find(arg)) != config.FeedsList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tAdded needed Feed '%s'", arg.c_str());
			this->addNeededFeed( arg );
			return;
		} else {
			SelLog->Log('F', "\t\tFeed '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(!!(arg = striKWcmp( l, "-->> need_namedfeed=" ))){
		NamedFeedCollection::iterator shut;
		if( (shut = config.NamedFeedsList.find(arg)) != config.NamedFeedsList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tAdded needed NamedFeed '%s'", arg.c_str());
			this->addNeededNamedFeed( arg );
			return;
		} else {
			SelLog->Log('F', "\t\tNamedFeed '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
#endif
	}

	return this->Object::readConfigDirective(l, name, nameused);
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
		for(auto &i : this->required_topic){
			try {
				class MQTTTopic *tpc = config.TopicsList.at( i );

				enum SharedObjType type;
				SelSharedVar->getValue( tpc->getNameC(), &type, false );
				if(type == SOT_UNKNOWN){
					SelLog->Log('T', "Required topic \"%s\" not set : task/trigger will not be launched", this->getNameC());
					return false;
				}

				class MQTTTopic **topic = (class MQTTTopic **)lua_newuserdata(L, sizeof(class MQTTTopic *));
				assert(topic);

				*topic = tpc;
				luaL_getmetatable(L, "MajordomeMQTTTopic");
				lua_setmetatable(L, -2);

				lua_setglobal(L, i.c_str());
			} catch( std::out_of_range &e ){	// Not found
				return false;
			}
		}
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

	for(auto &i : this->needed_rendezvous){
		try {
			class Event *evt = config.EventsList.at( i );
			class Event **event = (class Event **)lua_newuserdata(L, sizeof(class Event *));
			assert(event);

			*event = evt;
			luaL_getmetatable(L, "MajordomeRendezVous");
			lua_setmetatable(L, -2);

			lua_setglobal(L, i.c_str());
		} catch( std::out_of_range &e ){	// Not found
			return false;
		}
	}

	for(auto &i : this->needed_topic){
		try {
			class MQTTTopic *tpc = config.TopicsList.at( i );
			class MQTTTopic **topic = (class MQTTTopic **)lua_newuserdata(L, sizeof(class MQTTTopic *));
			assert(topic);

			*topic = tpc;
			luaL_getmetatable(L, "MajordomeMQTTTopic");
			lua_setmetatable(L, -2);

			lua_setglobal(L, i.c_str());
		} catch( std::out_of_range &e ){	// Not found
			return false;
		}
	}

	for(auto &i : this->needed_timer){
		try {
			class Timer *tmr = config.TimersList.at( i );
			class Timer **timer = (class Timer **)lua_newuserdata(L, sizeof(class Timer *));
			assert(timer);

			*timer = tmr;
			luaL_getmetatable(L, "MajordomeTimer");
			lua_setmetatable(L, -2);

			lua_setglobal(L, i.c_str());
		} catch( std::out_of_range &e ){	// Not found 
			return false;
		}
	}

	for(auto &i : this->needed_tracker){
		try {
			class Tracker *trk = config.TrackersList.at( i );
			class Tracker **tracker = (class Tracker **)lua_newuserdata(L, sizeof(class Tracker *));
			assert(tracker);

			*tracker = trk;
			luaL_getmetatable(L, "MajordomeTracker");
			lua_setmetatable(L, -2);

			lua_setglobal(L, i.c_str());
		} catch( std::out_of_range &e ){	// Not found 
			return false;
		}
	}

	for(auto &i : this->needed_minmax){
		try {
			class MinMax *mm = config.MinMaxList.at( i );
			class MinMax **minmax = (class MinMax **)lua_newuserdata(L, sizeof(class MinMax *));
			assert(minmax);

			*minmax = mm;
			luaL_getmetatable(L, "MajordomeMinMax");
			lua_setmetatable(L, -2);

			lua_setglobal(L, i.c_str());
		} catch( std::out_of_range &e ){	// Not found 
			return false;
		}
	}

	for(auto &i : this->needed_namedminmax){
		try {
			class NamedMinMax *nmm = config.NamedMinMaxList.at( i );
			class NamedMinMax **nminmax = (class NamedMinMax **)lua_newuserdata(L, sizeof(class NamedMinMax *));
			assert(nminmax);

			*nminmax = nmm;
			luaL_getmetatable(L, "MajordomeNamedMinMax");
			lua_setmetatable(L, -2);

			lua_setglobal(L, i.c_str());
		} catch( std::out_of_range &e ){	// Not found 
			return false;
		}
	}

	for(auto &i : this->needed_shutdown){
		try {
			class Shutdown *s = config.ShutdownsList.at( i );
			class Shutdown **shut = (class Shutdown **)lua_newuserdata(L, sizeof(class Shutdown *));
			assert(shut);

			*shut = s;
			luaL_getmetatable(L, "MajordomeShutdown");
			lua_setmetatable(L, -2);

			lua_setglobal(L, i.c_str());
		} catch( std::out_of_range &e ){	// Not found 
			return false;
		}
	}

#ifdef DBASE
	for(auto &i : this->needed_feed){
		try {
			class Feed *s = config.FeedsList.at( i );
			class Feed **shut = (class Feed **)lua_newuserdata(L, sizeof(class Feed *));
			assert(shut);

			*shut = s;
			luaL_getmetatable(L, "MajordomeFeed");
			lua_setmetatable(L, -2);

			lua_setglobal(L, i.c_str());
		} catch( std::out_of_range &e ){	// Not found 
			return false;
		}
	}

	for(auto &i : this->needed_namedfeed){
		try {
			class NamedFeed *s = config.NamedFeedsList.at( i );
			class NamedFeed **shut = (class NamedFeed **)lua_newuserdata(L, sizeof(class NamedFeed *));
			assert(shut);

			*shut = s;
			luaL_getmetatable(L, "MajordomeNamedFeed");
			lua_setmetatable(L, -2);

			lua_setglobal(L, i.c_str());
		} catch( std::out_of_range &e ){	// Not found 
			return false;
		}
	}
#endif

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

bool LuaExec::prepareExecSync(lua_State *L){
	int err;
	if( (err = SelElasticStorage->loadsharedfunction( L, this->getFunc() )) ){
		SelLog->Log('E', "Unable to create task '%s' from '%s' : %s", this->getNameC(), this->getWhereC(), (err == LUA_ERRSYNTAX) ? "Syntax error" : "Memory error" );
		lua_close( L );
		return false;
	}

	if(verbose && !this->isQuiet())
		SelLog->Log('T', "Sync running Task '%s' from '%s'", this->getNameC(), this->getWhereC() );

	return true;
}

bool LuaExec::execSync(lua_State *L, enum boolRetCode *rc){
	if(!this->prepareExecSync(L))
		return false;

	if(lua_pcall( L, 0, 1, 0))
		SelLog->Log('E', "Can't execute task '%s' from '%s' : %s", this->getNameC(), this->getWhereC(), lua_tostring(L, -1));

	if(rc){
		*rc = boolRetCode::RCnil;
		if(lua_isboolean(L, -1))
			*rc = lua_toboolean(L, -1) ? boolRetCode::RCtrue : boolRetCode::RCfalse;
	}

	return true;
}

bool LuaExec::execSync(lua_State *L, enum boolRetCode *rc, lua_Number *retn){
	*retn = NAN;
	*rc = boolRetCode::RCnil;

	if(!this->prepareExecSync(L))
		return false;

	if(lua_pcall( L, 0, 1, 0))
		SelLog->Log('E', "Can't execute task '%s' from '%s' : %s", this->getNameC(), this->getWhereC(), lua_tostring(L, -1));

	if(lua_isboolean(L, -1))
		*rc = lua_toboolean(L, -1) ? boolRetCode::RCtrue : boolRetCode::RCfalse;

	if(lua_isnumber(L, -1)){
		*rc = boolRetCode::RCforced;
		*retn = lua_tonumber(L, -1);
	}

	return true;
	
}

bool LuaExec::execSync(lua_State *L, std::string *rs, enum boolRetCode *rc, lua_Number *retn){
	*rc = boolRetCode::RCnil;
	*retn = NAN;

	if(!this->prepareExecSync(L))
		return false;

	if(lua_pcall( L, 0, 2, 0))
		SelLog->Log('E', "Can't execute task '%s' from '%s' : %s", this->getNameC(), this->getWhereC(), lua_tostring(L, -1));

		/* -1 : numeric value if provided
		 * -2 : string value or RC
		 */

	if(lua_isboolean(L, -2))
		*rc = lua_toboolean(L, -2) ? boolRetCode::RCtrue : boolRetCode::RCfalse;

	if(lua_isstring(L, -2))
		*rs = lua_tostring(L, -2);

	if(lua_isnumber(L, -1)){
		*rc = boolRetCode::RCforced;
		*retn = lua_tonumber(L, -1);
	}

	return true;
}
