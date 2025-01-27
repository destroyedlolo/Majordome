#include "Timer.h"
#include "Helpers.h"
#include "Config.h"

#include <fstream>

#include <sys/time.h>
#include <cstring>

Timer::Timer( const std::string &fch, std::string &where, std::string &name ) : Object(fch, where, name), every(0), at((unsigned short)-1), immediate(false), runifover(false), cond(PTHREAD_COND_INITIALIZER), mutex(PTHREAD_MUTEX_INITIALIZER) {

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

		file.close();
	} catch(const std::ifstream::failure &e){
		if(!file.eof()){
			SelLog->Log('F', "%s : %s", fch.c_str(), strerror(errno) );
			exit(EXIT_FAILURE);
		}
	}

}

void Timer::readConfigDirective( std::string &l, std::string &name, bool &nameused ){
	MayBeEmptyString arg;

	if( !!(arg = striKWcmp( l, "-->> at=" )) ){
		uint32_t v = strtoul( arg.c_str(), NULL, 10 );
		this->at = v / 100;
		this->min = v % 100;
		if(verbose)
			SelLog->Log('C', "\t\tRunning at %u:%u", this->at, this->min);
	} else if( !!(arg = striKWcmp( l, "-->> every=" )) ){
		this->every = strtoul( arg.c_str(), NULL, 0 );
		if(verbose)
			SelLog->Log('C', "\t\tRunning every %lu second%c", this->every, this->every > 1 ? 's':' ');
	} else if( l == "-->> immediate" ){
		this->immediate = true;
		if(verbose)
			SelLog->Log('C', "\t\tImmediate");
	} else if( l == "-->> runifover" ){
		this->runifover = true;
		if(verbose)
			SelLog->Log('C', "\t\tRun if over");
 	} else 
		Object::readConfigDirective(l, name, nameused);
}

void *Timer::threadedslave(void *arg){
	class Timer *me = static_cast<Timer *>(arg);	// 'this' in this thread

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
				SelLog->Log('D', "[Timer %s] %lu second(s) to wait", me->getNameC(), sec );
#endif
			ts.tv_sec += sec;
		} else {
			SelLog->Log('F', "[Timer '%s'] No time defined", me->getNameC());
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
			SelLog->Log('D', "[Timer %s] it's %s", me->getNameC(), SeleneCore->ctime(&current_time, NULL, 0) );
		}
#endif
		me->execHandlers();
	}
	return NULL;
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

void Timer::execHandlers(void){
	this->Event::execHandlers();	// Execute slaves' handlers

	/* TODO start/stop */
}
