#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cassert>

extern "C" {
    #include "lualib.h"
    #include "lauxlib.h"
};

#include "Helpers.h"
#include "Timer.h"
#include "Config.h"

Timer::Timer( const std::string &fch, std::string &where, std::string &name ) : every(0), at((unsigned short)-1), immediate(false), runifover(false), cond(PTHREAD_COND_INITIALIZER), mutex(PTHREAD_MUTEX_INITIALIZER) {
	this->extrName( fch, name );
	this->name = name;
	this->where = where;

	/*
	 * Reading file's content
	 */
	if(verbose)
		SelLog->Log('L', "\t'%s'", fch.c_str());

	std::ifstream file;
	file.exceptions ( std::ios::eofbit | std::ios::failbit ); // No need to check failbit
	try {
		std::string l;

		file.open(fch);
		while( std::getline( file, l) ){
			MayBeEmptyString arg;

			if( !!(arg = striKWcmp( l, "name=" )) ){
				this->name = name = arg;
				if(verbose)
					SelLog->Log('C', "\t\tChanging name to '%s'", name.c_str());
			} else if( !!(arg = striKWcmp( l, "at=" )) ){
				unsigned long v = strtoul( arg.c_str(), NULL, 10 );
				this->at = v / 100;
				this->min = v % 100;
				if(verbose)
					SelLog->Log('C', "\t\tRunning at %u:%u", this->at, this->min);
			} else if( !!(arg = striKWcmp( l, "every=" )) ){
				this->every = strtoul( arg.c_str(), NULL, 0 );
				if(verbose)
					SelLog->Log('C', "\t\tRunning every %lu second%c", this->every, this->every > 1 ? 's':' ');
			} else if( l == "immediate" ){
				this->immediate = true;
				if(verbose)
					SelLog->Log('C', "\t\tImmediate");
			} else if( l == "runifover" ){
				this->runifover = true;
				if(verbose)
					SelLog->Log('C', "\t\tRun if over");
			} else if( l == "quiet" ){
				if(verbose)
					SelLog->Log('C', "\t\tBe quiet");
				this->beQuiet();
			} else if( l == "disabled" ){
				if(verbose)
					SelLog->Log('C', "\t\tDisabled");
				this->disable();
			}
#if 0
else SelLog->Log('D', "Ignore '%s'", l.c_str());
#endif
		}
	} catch(const std::ifstream::failure &e){
		if(!file.eof()){
			SelLog->Log('F', "%s : %s", fch.c_str(), strerror(errno) );
			exit(EXIT_FAILURE);
		}
	} catch(const std::invalid_argument &e){
		SelLog->Log('F', "%s : invalid argument", fch.c_str() );
		exit(EXIT_FAILURE);
	}

	file.close();
}

void Timer::launchThread( void ){
	/*
	 * Create a detached thread
	 */
	pthread_attr_t thread_attr;
	if( pthread_attr_init(&thread_attr) ){
		SelLog->Log('F', "Can't initialise a new thread for '%s' : %s", this->getWhereC(), strerror(errno) );
		exit(EXIT_FAILURE);
	}
	if( pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED) ){
		SelLog->Log('F', "Can't setdetechstate for a new thread for '%s' : %s", this->getWhereC(), strerror(errno) );
		exit(EXIT_FAILURE);
	}
	if(pthread_create( &(this->thread), &thread_attr, this->threadedslave, this )){
		SelLog->Log('F', "Can't create a new thread for '%s' : %s", this->getWhereC(), strerror(errno) );
		exit(EXIT_FAILURE);
	}
	pthread_attr_destroy(&thread_attr);
}

void *Timer::threadedslave(void *arg){
	class Timer *me = (class Timer *)arg;	// 'this' in this thread

	pthread_mutex_lock( &(me->mutex) );
	for(;;){
		struct timeval tv;
		struct timespec ts;

		gettimeofday(&tv, NULL);
		TIMEVAL_TO_TIMESPEC( &tv, &ts );

		if( me->every )
			ts.tv_sec += me->every;
		else if( me->at != (unsigned short)-1 ){
			unsigned long sec;	// Seconds to add
			for(;;) {
				struct tm now;
				localtime_r( &ts.tv_sec, &now );
				if( now.tm_hour < me->at )	// future
					sec = ((me->at - now.tm_hour)*60 + me->min - now.tm_min) * 60 - now.tm_sec;
				else if( now.tm_hour > me->at ) // Past, switch to next day
					sec = ((me->at - now.tm_hour + 24)*60 + me->min - now.tm_min) * 60 - now.tm_sec;
				else if( now.tm_min < me->min ) // Same hour or but future minute
					sec = (me->min - now.tm_min) * 60 - now.tm_sec;
				else	// Same hour but minute in the past
					sec = ((me->at - now.tm_hour + 24)*60 + me->min - now.tm_min) * 60 - now.tm_sec;
				if( ts.tv_sec == time(NULL) )	// Done in the same second
					break;
				else {	// race condition, redo the test
					gettimeofday(&tv, NULL);
					TIMEVAL_TO_TIMESPEC( &tv, &ts );
				}
			}
#ifdef DEBUG
			if( debug )
				SelLog->Log('D', "Timer %s : %lu second(s) to wait", me->getNameC(), sec );
#endif
			ts.tv_sec += sec;
		} else {
			SelLog->Log('F', "Timer '%s' : No time defined", me->getNameC());
			exit(EXIT_FAILURE);
		}

		int rc;
		if( (rc = pthread_cond_timedwait(&(me->cond), &(me->mutex), &ts)) != ETIMEDOUT ){
// SelLog->Log('d', "Interrupted : %s", strerror(rc));
			if( me->cmd == Commands::RESET ){
// SelLog->Log('d', "reset");
				continue;	// Rethink the timer without launching tasks
			}
		}

#ifdef DEBUG
		if( debug ){
			time_t current_time = time(NULL);
			SelLog->Log('D', "Timer %s : it's %s", me->getNameC(), SeleneCore->ctime(&current_time, NULL, 0) );
		}
#endif
		me->execTasks();
	}
	return NULL;
}

bool Timer::isOver( void ){
	if( this->every || !this->runifover )
		return false;

	time_t t;
	struct tm now;
	time(&t);
	localtime_r( &t, &now );

	if( now.tm_hour < this->at )
		return false;
	else if( now.tm_hour > this->at )
		return true;
	else if( now.tm_min < this->min )
		return false;
	else	// If it's exactly the same hh:mm, it's already over
		return true;
}

bool Timer::inEveryMode( void ){
	return( !!this->every );
}

void Timer::execTasks( void ){
	if( this->isEnabled() ){
		for( Entries::iterator trk = this->startTrackers.begin(); trk != this->startTrackers.end(); trk++){	// starting tracker
			try {
				Tracker &tracker = config.findTracker( *trk );
				tracker.start();
			} catch (...) {
				SelLog->Log('F', "Internal error : can't find tracker \"%s\"", (*trk).c_str() );
				exit(EXIT_FAILURE);
			}
		}

		for( Entries::iterator trk = this->stopTrackers.begin(); trk != this->stopTrackers.end(); trk++){	// starting tracker
			try {
				Tracker &tracker = config.findTracker( *trk );
				tracker.stop();
			} catch (...) {
				SelLog->Log('F', "Internal error : can't find tracker \"%s\"", (*trk).c_str() );
				exit(EXIT_FAILURE);
			}
		}

		Event::execTasks( config, this->getNameC() );
#ifdef DEBUG
	} else if( debug ) {
		SelLog->Log('D', "Timer %s is disabled : no tasks launched nor trackers touched", this->getNameC() );
#endif
	}
}

void Timer::lock( void ){
	pthread_mutex_lock( &(this->mutex) );
}

void Timer::unlock( void ){
	pthread_mutex_unlock( &(this->mutex) );
}

void Timer::sendCommand( enum Commands c ){
	this->lock();
	this->cmd = c;
	pthread_cond_signal( &(this->cond) );
	this->unlock();
}

	/*****
	 * Lua exposed functions
	 *****/
static class Timer *checkMajordomeTimer(lua_State *L){
	class Timer **r = (class Timer **)SelLua->testudata(L, 1, "MajordomeTimer");
	luaL_argcheck(L, r != NULL, 1, "'MajordomeTimer' expected");
	return *r;
}

static int mtmr_find(lua_State *L){
	const char *name = luaL_checkstring(L, 1);

	try {
		class Timer &tmr = config.TimersList.at( name );
		class Timer **timer = (class Timer **)lua_newuserdata(L, sizeof(class Timer *));
		assert(timer);

		*timer = &tmr;
		luaL_getmetatable(L, "MajordomeTimer");
		lua_setmetatable(L, -2);

		return 1;
	} catch( std::out_of_range &e ){	// Not found 
		return 0;
	}
}

static const struct luaL_Reg MajTimerLib [] = {
	{"find", mtmr_find},
	{NULL, NULL}
};

static int mtmr_getEvery( lua_State *L ){
	class Timer *timer = checkMajordomeTimer(L);
	lua_pushnumber( L, (lua_Number)timer->getEvery() );
	return 1;
}

static int mtmr_setEvery( lua_State *L ){
	class Timer *timer = checkMajordomeTimer(L);
	timer->setEvery( luaL_checkinteger(L, 2) );
	return 0;
}

static int mtmr_getAt( lua_State *L ){
	class Timer *timer = checkMajordomeTimer(L);
	timer->lock();	// As "at" is stored in 2 fields, we have to avoid race condition
	lua_Number r = timer->getAt() + timer->getMin()/100.0;
	lua_pushnumber( L, r );
	timer->unlock();
	return 1;
}

static int mtmr_getAtHM( lua_State *L ){
	class Timer *timer = checkMajordomeTimer(L);
	timer->lock();	// As "at" is stored in 2 fields, we have to avoid race condition
	lua_pushnumber( L, timer->getAt() );
	lua_pushnumber( L, timer->getMin() );
	timer->unlock();
	return 2;
}

static void internal_setAt( class Timer *timer, unsigned short h, unsigned short m ){
	h += m/60;
	m %= 60;

	timer->lock();	// As "at" is stored in 2 fields, we have to avoid race condition
	timer->setAt( h );
	timer->setMin( m );
	timer->unlock();

	if( !timer->inEveryMode() )	// The timer need to be recalculed only if in 'at' mode
		timer->sendCommand( Timer::Commands::RESET );
}

static int mtmr_setAt( lua_State *L ){
	class Timer *timer = checkMajordomeTimer(L);
	lua_Number v = luaL_checknumber(L, 2);
	v += 0.0005;	// Avoid FFP rounding

	unsigned short h = (unsigned short)v;
	internal_setAt( timer, h, (v - h) * 100 );
	return 0;
}

static int mtmr_setAtHM( lua_State *L ){
	class Timer *timer = checkMajordomeTimer(L);
	short h = luaL_checkinteger(L, 2);
	short m = luaL_checkinteger(L, 3);

	while( m<0 ){	// If it's in previous hour
		h--;
		m += 60;
	}

	internal_setAt( timer, h, m );
	return 0;
}

static int mtmr_Reset( lua_State *L ){
	class Timer *timer = checkMajordomeTimer(L);

	if( timer->inEveryMode() )	// Only usefull in Every mode
		timer->sendCommand( Timer::Commands::RESET );

	return 0;
}

static int mtmr_Launch( lua_State *L ){
	class Timer *timer = checkMajordomeTimer(L);

	timer->sendCommand( Timer::Commands::LAUNCH );

	return 0;
}

static int mtmr_getContainer( lua_State *L ){
	class Timer *timer = checkMajordomeTimer(L);
	lua_pushstring( L, timer->getWhereC() );
	return 1;
}

static int mtmr_getName( lua_State *L ){
	class Timer *timer = checkMajordomeTimer(L);
	lua_pushstring( L, timer->getName().c_str() );
	return 1;
}

static int mtmr_enabled( lua_State *L ){
	class Timer *timer = checkMajordomeTimer(L);
	timer->enable();
	return 0;
}

static int mtmr_disable( lua_State *L ){
	class Timer *timer = checkMajordomeTimer(L);
	timer->disable();
	return 0;
}

static int mtmr_isEnabled( lua_State *L ){
	class Timer *timer = checkMajordomeTimer(L);
	lua_pushboolean( L, timer->isEnabled() );
	return 1;
}

static const struct luaL_Reg MajTimerM [] = {
	{"getEvery", mtmr_getEvery},
	{"setEvery", mtmr_setEvery},
	{"getAt", mtmr_getAt},
	{"getAtHM", mtmr_getAtHM},
	{"setAt", mtmr_setAt},
	{"setAtHM", mtmr_setAtHM},
	{"Reset", mtmr_Reset},
	{"Launch", mtmr_Launch},
	{"getContainer", mtmr_getContainer},
	{"getName", mtmr_getName},
	{"isEnabled", mtmr_isEnabled},
	{"Enable", mtmr_enabled},
	{"Disable", mtmr_disable},
	{NULL, NULL}
};

void Timer::initLuaObject( lua_State *L ){
	SelLua->objFuncs( L, "MajordomeTimer", MajTimerM );
	SelLua->libCreateOrAddFuncs( L, "MajordomeTimer", MajTimerLib );
}

