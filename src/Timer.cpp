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
		publishLog('L', "\t'%s'", fch.c_str());

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
					publishLog('C', "\t\tChanging name to '%s'", name.c_str());
			} else if( !!(arg = striKWcmp( l, "at=" )) ){
				unsigned long v = strtoul( arg.c_str(), NULL, 10 );
				this->at = v / 100;
				this->min = v % 100;
				if(verbose)
					publishLog('C', "\t\tRunning at %u:%u", this->at, this->min);
			} else if( !!(arg = striKWcmp( l, "every=" )) ){
				this->every = strtoul( arg.c_str(), NULL, 0 );
				if(verbose)
					publishLog('C', "\t\tRunning every %lu second%c", this->every, this->every > 1 ? 's':' ');
			} else if( l == "immediate" ){
				this->immediate = true;
				if(verbose)
					publishLog('C', "\t\tImmediate");
			} else if( l == "runifover" ){
				this->runifover = true;
				if(verbose)
					publishLog('C', "\t\tRun if over");
			} else if( l == "disabled" ){
				if(verbose)
					publishLog('C', "\t\tDisabled");
				this->disable();
			}
#if 0
else publishLog('D', "Ignore '%s'", l.c_str());
#endif
		}
	} catch(const std::ifstream::failure &e){
		if(!file.eof()){
			publishLog('F', "%s : %s", fch.c_str(), strerror(errno) );
			exit(EXIT_FAILURE);
		}
	} catch(const std::invalid_argument &e){
		publishLog('F', "%s : invalid argument", fch.c_str() );
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
		publishLog('F', "Can't initialise a new thread for '%s' : %s", this->getWhereC(), strerror(errno) );
		exit(EXIT_FAILURE);
	}
	if( pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED) ){
		publishLog('F', "Can't setdetechstate for a new thread for '%s' : %s", this->getWhereC(), strerror(errno) );
		exit(EXIT_FAILURE);
	}
	if(pthread_create( &(this->thread), &thread_attr, this->threadedslave, this )){
		publishLog('F', "Can't create a new thread for '%s' : %s", this->getWhereC(), strerror(errno) );
		exit(EXIT_FAILURE);
	}
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
			publishLog('D', "Timer %s : %lu second(s) to wait", me->getNameC(), sec );
#endif
			ts.tv_sec += sec;
		} else {
			publishLog('F', "Timer '%s' : No time defined", me->getNameC());
			exit(EXIT_FAILURE);
		}

		int rc;
		if( (rc = pthread_cond_timedwait(&(me->cond), &(me->mutex), &ts)) != ETIMEDOUT ){
				/* AF : Handle command */
			printf("Interrupted : %s", strerror(rc));
			exit(0);
		}

#ifdef DEBUG
		time_t current_time = time(NULL);
		publishLog('D', "Timer %s : it's %s", me->getNameC(), ctime(&current_time) );
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

void Timer::execTasks( void ){
	if( this->isEnabled() )
		Event::execTasks( config, this->getNameC() );
#ifdef DEBUG
	else
		publishLog('D', "Timer %s is disabled : no tasks launched", this->getNameC() );
#endif
}

	/*****
	 * Lua exposed functions
	 *****/
static class Timer *checkMajordomeTimer(lua_State *L){
	class Timer **r = (class Timer **)luaL_testudata(L, 1, "MajordomeTimer");
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
	return 1;
}

static int mtmr_getContainer( lua_State *L ){
	class Timer *timer = checkMajordomeTimer(L);
	lua_pushstring( L, timer->getWhereC() );
	return 1;
}

static int mtmr_enabled( lua_State *L ){
	class Timer *timer = checkMajordomeTimer(L);
	timer->enable();
	return 1;
}

static int mtmr_disable( lua_State *L ){
	class Timer *timer = checkMajordomeTimer(L);
	timer->disable();
	return 1;
}

static int mtmr_isEnabled( lua_State *L ){
	class Timer *timer = checkMajordomeTimer(L);
	lua_pushboolean( L, timer->isEnabled() );
	return 1;
}

static const struct luaL_Reg MajTimerM [] = {
	{"getEvery", mtmr_getEvery},
	{"setEvery", mtmr_setEvery},
	{"getContainer", mtmr_getContainer},
	{"isEnabled", mtmr_isEnabled},
	{"enable", mtmr_enabled},
	{"disable", mtmr_disable},
	{NULL, NULL}
};

int Timer::initLuaObject( lua_State *L ){
	libSel_objFuncs( L, "MajordomeTimer", MajTimerM );
	libSel_libFuncs( L, "MajordomeTimer", MajTimerLib );

	return 1;
}

