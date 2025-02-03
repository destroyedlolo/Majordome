#include "Tracker.h"
#include "Config.h"
#include "Helpers.h"

#include <fstream>

#include <cstring>
#include <cassert>
#include <regex>

Tracker::Tracker(const std::string &fch, std::string &where, std::string &name, lua_State *L) : Object(fch, where, name), Handler(fch, where, name){
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

			this->readConfigDirective(l, name, nameused);
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

void Tracker::readConfigDirective( std::string &l, std::string &name, bool &nameused ){
	MayBeEmptyString arg;

	if( !!(arg = striKWcmp( l, "-->> listen=" ))){
		TopicCollection::iterator topic;
		if( (topic = config.TopicsList.find(arg)) != config.TopicsList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tAdded to topic '%s'", arg.c_str());
			topic->second->addHandler( dynamic_cast<Handler *>(this) );
//			nameused = true;
		} else {
			SelLog->Log('F', "\t\tTopic '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if( !!(arg = striKWcmp( l, "-->> howmany=" ))){
		if((this->howmany = strtoul(arg.c_str(), NULL, 0))<1)
			this->howmany = 1;
		if(verbose)
			SelLog->Log('C', "\t\tHow many: '%d'", this->howmany);
	} else if( !!(arg = striKWcmp( l, "-->> statustopic=" ))){
		setStatusTopic( std::regex_replace(arg, std::regex("%ClientID%"), MQTT_ClientID) );
		if(verbose)
			SelLog->Log('C', "\t\tStatus topic : '%s'", this->getStatusTopic().c_str());
	} else if( !!(arg = striKWcmp( l, "-->> start=" ))){
		TimerCollection::iterator timer;
		if( (timer = config.TimersList.find(arg)) != config.TimersList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tStart timer '%s'", arg.c_str());
			timer->second->addStartTracker( this );
			nameused = true;
		} else {
			SelLog->Log('F', "\t\tTimer '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if( !!(arg = striKWcmp( l, "-->> stop=" ))){
		TimerCollection::iterator timer;
		if( (timer = config.TimersList.find(arg)) != config.TimersList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tStop timer '%s'", arg.c_str());
			timer->second->addStopTracker( this );
			nameused = true;
		} else {
			SelLog->Log('F', "\t\tTimer '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if( !!(arg = striKWcmp( l, "-->> enableRDV=" ))){
		EventCollection::iterator event;
		if( (event = config.EventsList.find(arg)) != config.EventsList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tEnabling rendez-vous '%s'", arg.c_str());
	 		event->second->addTrackerEN( this );
		} else {
			SelLog->Log('F', "\t\tRendez-vous '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if( !!(arg = striKWcmp( l, "-->> disableRDV=" ))){
		EventCollection::iterator event;
		if( (event = config.EventsList.find(arg)) != config.EventsList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tDisabling rendez-vous '%s'", arg.c_str());
	 		event->second->addTrackerDIS( this );
		} else {
			SelLog->Log('F', "\t\tRendez-vous '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if( l == "-->> activated" ){
		if(verbose)
			SelLog->Log('C', "\t\tActivated at startup");
		this->status = _status::CHECKING;
		this->hm_counter = this->howmany;
	} else 
		this->LuaExec::readConfigDirective(l, name, nameused);
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
	/* TODO */
}

void Tracker::start( void ){
	if( this->isEnabled() && this->getStatus() != _status::CHECKING ){
		for(auto &t : this->startingTasks)	// Execute attached starting tasks
			t->exec(this);
		if(verbose)
			SelLog->Log('T', "Tracker '%s' is checking", this->getNameC() );
		this->status = _status::CHECKING;
		this->hm_counter = this->howmany;
		this->publishstatus();
		this->notifyChanged();
	}
}

void Tracker::stop( void ){
	if( this->isEnabled() && this->getStatus() != _status::WAITING ){
		for(auto &t : this->stoppingTasks)	// Execute attached stopping tasks
			t->exec(this);
		if(verbose)
			SelLog->Log('T', "Tracker '%s' is waiting", this->getNameC() );
		this->status = _status::WAITING;
		this->publishstatus();
		this->notifyChanged();
	}
}

void Tracker::done( void ){
	if( this->isEnabled() && this->getStatus() != _status::CHECKING ){
		for(auto &t : *this)	// Execute attached done tasks
			t->exec(this);
		if(verbose)
			SelLog->Log('T', "Tracker '%s' is done", this->getNameC() );
		this->status = _status::DONE;
		this->publishstatus();
		this->notifyChanged();
	}
}

void Tracker::feedState(lua_State *L){
	try {
		class Tracker *tsk = config.TrackersList.at( this->getNameC() );
		class Tracker **task = (class Tracker **)lua_newuserdata(L, sizeof(class Tracker *));
		assert(task);

		*task = tsk;
		luaL_getmetatable(L, "MajordomeTracker");
		lua_setmetatable(L, -2);
		lua_setglobal( L, "MAJORDOME_Myself" );
	} catch( std::out_of_range &e ){	// Not found 
		SelLog->Log('F', "Can't find tracker '%s'", this->getNameC() );
		exit(EXIT_FAILURE);
	}
}

void Tracker::feedHandlersState(lua_State *L){
	lua_pushstring( L, this->getNameC() );	// Push the name
	lua_setglobal( L, "MAJORDOME_TRACKER" );

	lua_pushstring( L, this->getStatusC() );	// Push the name
	lua_setglobal( L, "MAJORDOME_TRACKER_STATUS" );
}

