#include "Config.h"
#include "Helpers.h"
#include "Handler.h"
#include "HandlersExecutor.h"

lua_State *Handler::prepareExec(void){
	if(!this->canRun())
		return NULL;

	/* ***
	 * Right now, the tasks is marked as running 
	 * ***/

	lua_State *L = LuaExec::createLuaState();
	if(!L){	// Can't create the state
		this->finished();
		return NULL;
	}

	threadEnvironment(L);	// Feed environment with generals
	if(!this->feedbyNeeded(L)){
		this->finished();
		lua_close( L );
		return NULL;
	}

	this->feedState(L);

	return L;
}

bool Handler::readConfigDirective(std::string &l){
		/* No specific directive */
	return this->LuaExec::readConfigDirective(l);
}

bool Handler::readConfigDirectiveNoData(std::string &l){
	std::string arg;

	if(!(arg = striKWcmp( l, "-->> waitfor=" )).empty()){
		EventCollection::iterator event;
		if( (event = config.EventsList.find(arg)) != config.EventsList.end()){
			if(::verbose)
				SelLog->Log('C', "\t\tAdded to rendezvous '%s'", arg.c_str());
			event->second->addHandler( dynamic_cast<Handler *>(this) );

			if(d2)
				fd2 << this->getFullId() << " <- " << event->second->getFullId() << ": waitfor { class: llink }" << std::endl;
		} else {
			SelLog->Log('F', "\t\tRendezvous '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
		return true;
	} else if(!(arg = striKWcmp( l, "-->> when=" )).empty()){
		TimerCollection::iterator timer;
		if( (timer = config.TimersList.find(arg)) != config.TimersList.end()){
			if(::verbose)
				SelLog->Log('C', "\t\tAdded to timer '%s'", arg.c_str());
			timer->second->addHandler( dynamic_cast<Handler *>(this) );

			if(d2)
				fd2 << this->getFullId() << " <- " << timer->second->getFullId() << ": when { class: llink }" << std::endl;
		} else {
			SelLog->Log('F', "\t\ttimer '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
		return true;
	} else if(!(arg = striKWcmp( l, "-->> whenDone=" )).empty()){
		TrackerCollection::iterator tracker;
		if( (tracker = config.TrackersList.find(arg)) != config.TrackersList.end()){
			if(::verbose)
				SelLog->Log('C', "\t\tAdded to tracker '%s' as Done task", arg.c_str());
			tracker->second->addDone( this );

			if(d2)
				fd2 << this->getFullId() << " <- " << tracker->second->getFullId() << ": whenDone { class: llink }" << std::endl;
		} else {
			SelLog->Log('F', "\t\tTracker '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
		return true;
	} else if(!(arg = striKWcmp( l, "-->> whenStarted=" )).empty()){
		TrackerCollection::iterator tracker;
		if( (tracker = config.TrackersList.find(arg)) != config.TrackersList.end()){
			if(::verbose)
				SelLog->Log('C', "\t\tAdded to tracker '%s' as Started task", arg.c_str());
	 		tracker->second->addStarted( this );

			if(d2)
				fd2 << this->getFullId() << " <- " << tracker->second->getFullId() << ": whenStarted { class: llink }" << std::endl;
		} else {
			SelLog->Log('F', "\t\tTracker '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
		return true;
	} else if(!(arg = striKWcmp( l, "-->> whenStopped=" )).empty()){
		TrackerCollection::iterator tracker;
		if( (tracker = config.TrackersList.find(arg)) != config.TrackersList.end()){
			if(::verbose)
				SelLog->Log('C', "\t\tAdded to tracker '%s' as Stopped task", arg.c_str());
		 	tracker->second->addStopped( this );

			if(d2)
				fd2 << this->getFullId() << " <- " << tracker->second->getFullId() << ": whenStopped { class: llink }" << std::endl;
		} else {
			SelLog->Log('F', "\t\tTracker '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
		return true;
	} else if(!(arg = striKWcmp( l, "-->> whenChanged=" )).empty()){
		TrackerCollection::iterator tracker;
		if( (tracker = config.TrackersList.find(arg)) != config.TrackersList.end()){
			if(::verbose)
				SelLog->Log('C', "\t\tAdded to tracker '%s' as Changed task", arg.c_str());
			tracker->second->addChanged( this );

			if(d2)
				fd2 << this->getFullId() << " <- " << tracker->second->getFullId() << ": whenChanged { class: llink }" << std::endl;
		} else {
			SelLog->Log('F', "\t\ttracker '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
		return true;
	}

	return false;
}

bool Handler::readConfigDirectiveData(std::string &l){
	std::string arg;

	if(!(arg = striKWcmp( l, "-->> listen=" )).empty()){
		TopicCollection::iterator topic;
		if( (topic = config.TopicsList.find(arg)) != config.TopicsList.end()){
			if(::verbose)
				SelLog->Log('C', "\t\tAdded to topic '%s'", arg.c_str());
			topic->second->addHandler( dynamic_cast<Handler *>(this) );

			if(d2)
				fd2 << this->getFullId() << " <- " << topic->second->getFullId() << ": listen { class: llink }" << std::endl;
		} else {
			SelLog->Log('F', "\t\tTopic '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
		return true;
	} else
		return false;
}

bool Handler::exec(HandlersExecutor *h){
	lua_State *L;

	if(!(L = this->prepareExec()))
		return false;

	if(h)
		h->feedHandlersState(L);

	return this->execAsync(L);
}

bool Handler::exec(lua_State *L /*, enum boolRetCode *rc, std::string *rs, lua_Number *retn*/){
	if(!this->canRun())
		return false;

	/* ***
	 * Right now, the tasks is marked as running 
	 * ***/

	if(!this->feedbyNeeded(L)){
		this->finished();
		return false;
	}

	this->feedState(L);

	return this->LuaExec::execSync(L);
}
