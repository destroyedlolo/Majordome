#include "Tracker.h"
#include "Config.h"
#include "Helpers.h"

#include <fstream>

#include <cstring>
#include <cassert>
#include <regex>

Tracker::Tracker(const std::string &fch, std::string &where, lua_State *L) : Object(fch, where), Handler(fch, where){
	this->loadConfigurationFile(fch, where,L);

	if(d2)
		fd2 << this->getFullId() << ".class: Tracker" << std::endl;
}

void Tracker::readConfigDirective( std::string &l ){
	std::string arg;

	if(!(arg = striKWcmp( l, "-->> howmany=" )).empty()){
		if((this->howmany = strtoul(arg.c_str(), NULL, 0))<1)
			this->howmany = 1;
		if(verbose)
			SelLog->Log('C', "\t\tHow many: '%d'", this->howmany);
	} else if(!(arg = striKWcmp( l, "-->> statustopic=" )).empty()){
		this->setStatusTopic( std::regex_replace(arg, std::regex("%ClientID%"), MQTT_ClientID) );
		if(verbose)
			SelLog->Log('C', "\t\tStatus topic : '%s'", this->getStatusTopic().c_str());
	} else if(!(arg = striKWcmp( l, "-->> start=" )).empty()){
		TimerCollection::iterator timer;
		if( (timer = config.TimersList.find(arg)) != config.TimersList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tStart timer '%s'", arg.c_str());
			timer->second->addStartTracker( this );
		} else {
			SelLog->Log('F', "\t\tTimer '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(!(arg = striKWcmp( l, "-->> stop=" )).empty()){
		TimerCollection::iterator timer;
		if( (timer = config.TimersList.find(arg)) != config.TimersList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tStop timer '%s'", arg.c_str());
			timer->second->addStopTracker( this );
		} else {
			SelLog->Log('F', "\t\tTimer '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(!(arg = striKWcmp( l, "-->> enableRDV=" )).empty()){
		EventCollection::iterator event;
		if( (event = config.EventsList.find(arg)) != config.EventsList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tEnabling rendez-vous '%s'", arg.c_str());
	 		event->second->addTrackerEN( this );
		} else {
			SelLog->Log('F', "\t\tRendez-vous '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(!(arg = striKWcmp( l, "-->> disableRDV=" )).empty()){
		EventCollection::iterator event;
		if( (event = config.EventsList.find(arg)) != config.EventsList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tDisabling rendez-vous '%s'", arg.c_str());
	 		event->second->addTrackerDIS( this );
		} else {
			SelLog->Log('F', "\t\tRendez-vous '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(l == "-->> activated"){
		if(verbose)
			SelLog->Log('C', "\t\tActivated at startup");
		this->status = _status::CHECKING;
		this->hm_counter = this->howmany;
	} else if(this->readConfigDirectiveNoData(l))
		;
	else 
		this->Handler::readConfigDirective(l);
}

bool Tracker::execAsync(lua_State *L){
	if( !this->isEnabled() || this->status != _status::CHECKING ){
		if(verbose && !this->isQuiet())
			SelLog->Log('T', "Tracker '%s' from '%s' is disabled or inactive", this->getNameC(), this->getWhereC() );
		lua_close( L );
		return false;
	}
	
	LuaExec::boolRetCode rc;

	bool r = this->LuaExec::execSync(L, &rc);

	if( rc == LuaExec::boolRetCode::RCfalse ){
		this->hm_counter = this->howmany;
#ifdef DEBUG
		if(debug && !this->isQuiet())
			SelLog->Log('D', "[%s] Reset to %u", this->getNameC(), this->getCounter());
#endif
	} else if( rc == LuaExec::boolRetCode::RCtrue ){
		if(!--this->hm_counter)
			this->done();
	}

	lua_close( L );
	return r;
}

const char *Tracker::getStatusC( void ){
	switch( this->status ){
	case _status::WAITING :
		return "WAITING";
	case _status::CHECKING :
		return "CHECKING";
	case _status::DONE :
		return "DONE";
	}

	return "?? Invalid tracker status ??";
}

void Tracker::publishstatus( void ){
	if( this->asStatusTopic() ){
		SelMQTT->mqttpublish( MQTT_client, this->getStatusTopic().c_str(), strlen(this->getStatusC()), (void *)this->getStatusC(), false);
	}
}

void Tracker::notifyChanged( void ){
	if( this->isEnabled() ){
		for(auto &t : this->changingHandlers)	// Execute attached starting tasks
			t->exec(this);
	}
}

void Tracker::start( void ){
	if( this->isEnabled() && this->getStatus() != _status::CHECKING ){
		for(auto &t : this->startingHandlers)	// Execute attached starting tasks
			t->exec(this);
		if(verbose && !this->isQuiet())
			SelLog->Log('T', "Tracker '%s' is checking", this->getNameC() );
		this->status = _status::CHECKING;
		this->hm_counter = this->howmany;
		this->publishstatus();
		this->notifyChanged();
	}
}

void Tracker::stop( void ){
	if( this->isEnabled() && this->getStatus() != _status::WAITING ){
		for(auto &t : this->stoppingHandlers)	// Execute attached stopping tasks
			t->exec(this);
		if(verbose && !this->isQuiet())
			SelLog->Log('T', "Tracker '%s' is waiting", this->getNameC() );
		this->status = _status::WAITING;
		this->publishstatus();
		this->notifyChanged();
	}
}

void Tracker::done( void ){
	if( this->isEnabled() && this->getStatus() == _status::CHECKING ){
		for(auto &t : *this)	// Execute attached done tasks
			t->exec(this);
		if(verbose && !this->isQuiet())
			SelLog->Log('T', "Tracker '%s' is done", this->getNameC() );
		this->status = _status::DONE;
		this->publishstatus();
		this->notifyChanged();
	}
}

void Tracker::feedState(lua_State *L){
	class Tracker **task = (class Tracker **)lua_newuserdata(L, sizeof(class Tracker *));
	assert(task);

	*task = this;
	luaL_getmetatable(L, "MajordomeTracker");
	lua_setmetatable(L, -2);
	lua_setglobal( L, "MAJORDOME_Myself" );

	this->feedHandlersState(L);
}

void Tracker::feedHandlersState(lua_State *L){
	lua_pushstring( L, this->getNameC() );	// Push the name
	lua_setglobal( L, "MAJORDOME_TRACKER" );

	lua_pushstring( L, this->getStatusC() );	// Push the name
	lua_setglobal( L, "MAJORDOME_TRACKER_STATUS" );
}

	/*****
	 * Lua exposed functions
	 *****/

static class Tracker *checkMajordomeTracker(lua_State *L){
	class Tracker **r = (class Tracker **)SelLua->testudata(L, 1, "MajordomeTracker");
	luaL_argcheck(L, r != NULL, 1, "'MajordomeTracker' expected");
	return *r;
}

static int mtrk_find(lua_State *L){
	const char *name = luaL_checkstring(L, 1);

	try {
		class Tracker *trk = config.TrackersList.at( name );
		class Tracker **tracker = (class Tracker **)lua_newuserdata(L, sizeof(class Tracker *));
		assert(tracker);

		*tracker = trk;
		luaL_getmetatable(L, "MajordomeTracker");
		lua_setmetatable(L, -2);

		return 1;
	} catch( std::out_of_range &e ){	// Not found 
		return 0;
	}
}

static const struct luaL_Reg MajTrackerLib [] = {
	{"find", mtrk_find},
	{NULL, NULL}
};

static int mtrk_getContainer(lua_State *L){
	class Tracker *tracker = checkMajordomeTracker(L);
	lua_pushstring( L, tracker->getWhereC() );
	return 1;
}

static int mtrk_getName(lua_State *L){
	class Tracker *tracker = checkMajordomeTracker(L);
	lua_pushstring( L, tracker->getName().c_str() );
	return 1;
}

static int mtrk_enabled( lua_State *L ){
	class Tracker *tracker = checkMajordomeTracker(L);
	tracker->enable();
	return 0;
}

static int mtrk_disable( lua_State *L ){
	class Tracker *tracker = checkMajordomeTracker(L);
	tracker->disable();
	return 0;
}

static int mtrk_isEnabled( lua_State *L ){
	class Tracker *tracker = checkMajordomeTracker(L);
	lua_pushboolean( L, tracker->isEnabled() );
	return 1;
}

static int mtrk_getCounter(lua_State *L){
	class Tracker *tracker = checkMajordomeTracker(L);
	lua_pushinteger( L, tracker->getCounter() );
	return 1;
}

static int mtrk_resetCounter(lua_State *L){
	class Tracker *tracker = checkMajordomeTracker(L);
	tracker->resetCounter();
	return 0;
}

static int mtrk_getStatus(lua_State *L){
	class Tracker *tracker = checkMajordomeTracker(L);
	lua_pushstring( L, tracker->getStatusC() );
	return 1;
}

static int mtrk_setStatus(lua_State *L){
	class Tracker *tracker = checkMajordomeTracker(L);
	const char *arg = luaL_checkstring(L, 2);

	if( !strcmp(arg, "CHECKING") || !strcmp(arg, "START") )
		tracker->start();
	else if( !strcmp(arg, "WAITING") || !strcmp(arg, "STOP") )
		tracker->stop();
	else 
		tracker->done();
	return 0;
}

static const struct luaL_Reg MajTrackerM [] = {
	{"getContainer", mtrk_getContainer},
	{"getName", mtrk_getName},
	{"isEnabled", mtrk_isEnabled},
	{"Enable", mtrk_enabled},
	{"Disable", mtrk_disable},
	{"getCounter", mtrk_getCounter},
	{"resetCounter", mtrk_resetCounter},
	{"getStatus", mtrk_getStatus},
	{"setStatus", mtrk_setStatus},
	{NULL, NULL}
};

void Tracker::initLuaInterface( lua_State *L ){
	SelLua->objFuncs( L, "MajordomeTracker", MajTrackerM );
	SelLua->libCreateOrAddFuncs( L, "MajordomeTracker", MajTrackerLib );
}

