#include <cstring>
#include <cassert>
#include <iostream>

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

bool LuaExec::feedbyNeeded( lua_State *L, bool require ){
	if(require){
		for(auto &i : this->required_topic){
			try {
				class MQTTTopic &tpc = config.TopicsList.at( i );

				enum SharedObjType type;
				SelSharedVar->getValue( tpc.getNameC(), &type, false );
				if(type == SOT_UNKNOWN){
					SelLog->Log('T', "Required topic \"%s\" not set : task/trigger will not be launched", this->getNameC());
					return false;
				}

				class MQTTTopic **topic = (class MQTTTopic **)lua_newuserdata(L, sizeof(class MQTTTopic *));
				assert(topic);

				*topic = &tpc;
				luaL_getmetatable(L, "MajordomeMQTTTopic");
				lua_setmetatable(L, -2);

				lua_setglobal(L, i.c_str());
			} catch( std::out_of_range &e ){	// Not found
				return false;
			}
		}
	}

	for(auto &i : this->needed_topic){
		try {
			class MQTTTopic &tpc = config.TopicsList.at( i );
			class MQTTTopic **topic = (class MQTTTopic **)lua_newuserdata(L, sizeof(class MQTTTopic *));
			assert(topic);

			*topic = &tpc;
			luaL_getmetatable(L, "MajordomeMQTTTopic");
			lua_setmetatable(L, -2);

			lua_setglobal(L, i.c_str());
		} catch( std::out_of_range &e ){	// Not found
			return false;
		}
	}

	for(auto &i : this->needed_rendezvous){
		try {
			class Event &evt = config.EventsList.at( i );
			class Event **event = (class Event **)lua_newuserdata(L, sizeof(class Event *));
			assert(event);

			*event = &evt;
			luaL_getmetatable(L, "MajordomeRendezVous");
			lua_setmetatable(L, -2);

			lua_setglobal(L, i.c_str());
		} catch( std::out_of_range &e ){	// Not found
			return false;
		}
	}

	for(auto &i : this->needed_tracker){
		try {
			class Tracker &trk = config.TrackersList.at( i );
			class Tracker **tracker = (class Tracker **)lua_newuserdata(L, sizeof(class Tracker *));
			assert(tracker);

			*tracker = &trk;
			luaL_getmetatable(L, "MajordomeTracker");
			lua_setmetatable(L, -2);

			lua_setglobal(L, i.c_str());
		} catch( std::out_of_range &e ){	// Not found 
			return false;
		}
	}

	for(auto &i : this->needed_timer){
		try {
			class Timer &tmr = config.TimersList.at( i );
			class Timer **timer = (class Timer **)lua_newuserdata(L, sizeof(class Timer *));
			assert(timer);

			*timer = &tmr;
			luaL_getmetatable(L, "MajordomeTimer");
			lua_setmetatable(L, -2);

			lua_setglobal(L, i.c_str());
		} catch( std::out_of_range &e ){	// Not found 
			return false;
		}
	}

	for(auto &i : this->needed_task){
		try {
			class LuaTask &tsk = config.TasksList.at( i );
			class LuaTask **task = (class LuaTask **)lua_newuserdata(L, sizeof(class LuaTask *));
			assert(task);

			*task = &tsk;
			luaL_getmetatable(L, "MajordomeTask");
			lua_setmetatable(L, -2);

			lua_setglobal(L, i.c_str());
		} catch( std::out_of_range &e ){	// Not found 
			return false;
		}
	}

	for(auto &i : this->needed_minmax){
		try {
			class MinMax &mm = config.MinMaxList.at( i );
			class MinMax **minmax = (class MinMax **)lua_newuserdata(L, sizeof(class MinMax *));
			assert(minmax);

			*minmax = &mm;
			luaL_getmetatable(L, "MajordomeMinMax");
			lua_setmetatable(L, -2);

			lua_setglobal(L, i.c_str());
		} catch( std::out_of_range &e ){	// Not found 
			return false;
		}
	}

	for(auto &i : this->needed_namedminmax){
		try {
			class NamedMinMax &mm = config.NamedMinMaxList.at( i );
			class NamedMinMax **minmax = (class NamedMinMax **)lua_newuserdata(L, sizeof(class NamedMinMax *));
			assert(minmax);

			*minmax = &mm;
			luaL_getmetatable(L, "MajordomeNamedMinMax");
			lua_setmetatable(L, -2);

			lua_setglobal(L, i.c_str());
		} catch( std::out_of_range &e ){	// Not found 
			return false;
		}
	}

#ifdef TOILE
	for(auto &i : this->needed_renderer){
		try {
			class Renderer &rd = config.RendererList.at( i );
			class SelGenericSurfaceLua *renderer = (class SelGenericSurfaceLua *)lua_newuserdata(L, sizeof(class SelGenericSurfaceLua));
			assert(renderer);

			renderer->storage = rd.getSurface();
if(debug) printf("****** surface : %p\n", renderer->storage);
			luaL_getmetatable(L, rd.getSurface()->cb->LuaObjectName() );
			lua_setmetatable(L, -2);

			lua_setglobal(L, i.c_str());
		} catch( std::out_of_range &e ){	// Not found 
			return false;
		}
	}
#endif

	return true;
}

bool LuaExec::readConfigDirective( std::string &l, bool &nameused ){
	MayBeEmptyString arg;

	if( !!(arg = striKWcmp( l, "-->> name=" ))){
		if( nameused ){
			SelLog->Log('F', "\t\tName can be changed only before any other directives");
			exit(EXIT_FAILURE);
		}

		this->name = name = arg;
		if(verbose)
			SelLog->Log('C', "\t\tChanging name to '%s'", name.c_str());
		return false;
	} else if( l == "-->> quiet" ){
		if(verbose)
			SelLog->Log('C', "\t\tBe quiet");
		this->beQuiet();
		return false;
	} else if( l == "-->> disabled" ){
		if(verbose)
			SelLog->Log('C', "\t\tDisabled");
		this->disable();
		return false;
	} else if(!!(arg = striKWcmp( l, "-->> need_topic=" ))){
		Config::TopicElements::iterator topic;
		if((topic = config.TopicsList.find(arg)) != config.TopicsList.end()){
#if 0	/* getVal() will fail if not stored but doesn't prevent to need it */
			if(!topic->second.toBeStored()){
				SelLog->Log('F', "Can't need \"%s\" topic : not stored", arg.c_str());
				exit(EXIT_FAILURE);
			}
#endif
			if(verbose)
				SelLog->Log('C', "\t\tAdded needed topic '%s'", arg.c_str());
			this->addNeededTopic(arg);
			return true;
		} else {
			SelLog->Log('F', "\t\tTopic '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(!!(arg = striKWcmp( l, "-->> require_topic=" ))){
		Config::TopicElements::iterator topic;
		if( (topic = config.TopicsList.find(arg)) != config.TopicsList.end()){
			if(!topic->second.toBeStored()){
				SelLog->Log('F', "Can't required \"%s\" topic : not stored", arg.c_str());
				exit(EXIT_FAILURE);
			}
			if(verbose)
				SelLog->Log('C', "\t\tAdded required topic '%s'", arg.c_str());
			this->addRequiredTopic(arg);
			return true;
		} else {
			SelLog->Log('F', "\t\tTopic '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(!!(arg = striKWcmp( l, "-->> need_timer=" ))){
		Config::TimerElements::iterator timer;
		if( (timer = config.TimersList.find(arg)) != config.TimersList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tAdded needed timer '%s'", arg.c_str());
			this->addNeededTimer(arg);
			return true;
		} else {
			SelLog->Log('F', "\t\ttimer '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(!!(arg = striKWcmp( l, "-->> need_rendezvous=" ))){
		Config::EventElements::iterator event;
		if( (event = config.EventsList.find(arg)) != config.EventsList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tAdded needed rendezvous '%s'", arg.c_str());
			this->addNeededRendezVous(arg);
			return true;
		} else {
			SelLog->Log('F', "\t\tRendezvous '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(!!(arg = striKWcmp( l, "-->> need_tracker=" ))){
		Config::TrackerElements::iterator tracker;
		if( (tracker = config.TrackersList.find(arg)) != config.TrackersList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tAdded needed tracker '%s'", arg.c_str());
			this->addNeededTracker( arg );
			return true;
		} else {
			SelLog->Log('F', "\t\ttracker '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(!!(arg = striKWcmp( l, "-->> need_task=" ))){
			/* No way to test if the task exists or not (as it could be
			 * defined afterward. Will be part of sanity checks
			 */
		if(verbose)
			SelLog->Log('C', "\t\tAdded needed task '%s'", arg.c_str());
		this->addNeededTask( arg );
		return true;
	} else if(!!(arg = striKWcmp( l, "-->> need_minmax=" ))){
		Config::MinMaxElements::iterator minmax;
		if( (minmax = config.MinMaxList.find(arg)) != config.MinMaxList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tAdded needed minmax '%s'", arg.c_str());
			this->addNeededMinMax( arg );
			return true;
		} else {
			SelLog->Log('F', "\t\tminmax '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(!!(arg = striKWcmp( l, "-->> need_namedminmax=" ))){
		Config::NamedMinMaxElements::iterator nminmax;
		if( (nminmax = config.NamedMinMaxList.find(arg)) != config.NamedMinMaxList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tAdded needed namedminmax '%s'", arg.c_str());
			this->addNeededNamedMinMax( arg );
			return true;
		} else {
			SelLog->Log('F', "\t\tnamedminmax '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
#ifdef TOILE
	} else if(!!(arg = striKWcmp( l, "-->> need_renderer=" ))){
		Config::RendererElements::iterator renderer;
		if( (renderer = config.RendererList.find(arg)) != config.RendererList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tAdded needed renderer '%s'", arg.c_str());
			this->addNeededRenderer( arg );
			return true;
		} else {
			SelLog->Log('F', "\t\tRenderer '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
#endif
	}

	return Object::readConfigDirective(l, nameused);
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
	threadEnvironment(arg->L);
	if(!this->feedbyNeeded(arg->L)){
		lua_close( arg->L );
		this->finished();
		delete arg;
		return false;
	}

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

bool LuaExec::execSync( const char *name, const char *topic, const char *payload, bool tracker, enum boolRetCode *rc, std::string *rs, lua_Number *retn ){
	lua_State *L = luaL_newstate();
	if( !L ){
		SelLog->Log('E', "Unable to create a new Lua State for '%s' from '%s'", this->getNameC(), this->getWhereC() );
		return false;
	}

	luaL_openlibs(L);
	threadEnvironment(L);
	if(!this->feedbyNeeded(L)){
		lua_close( L );
		return false;
	}

	int err;
	if( (err = SelElasticStorage->loadsharedfunction( L, this->getFunc() )) ){
		SelLog->Log('E', "Unable to create task '%s' from '%s' : %s", this->getNameC(), this->getWhereC(), (err == LUA_ERRSYNTAX) ? "Syntax error" : "Memory error" );
		lua_close( L );
		return false;
	}

	this->feedState( L, name, topic, payload, tracker );

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

	lua_close(L);

	return true;
}
