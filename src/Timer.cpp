#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

#include <sys/timerfd.h>

#include "Helpers.h"
#include "Timer.h"

Timer::Timer( const std::string &fch, std::string &where, std::string &name ) : every(0), at((unsigned long)-1), immediate(false), runifover(false), cond(PTHREAD_COND_INITIALIZER), mutex(PTHREAD_MUTEX_INITIALIZER) {
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
				this->at = strtoul( arg.c_str(), NULL, 0 );
				if(verbose)
					publishLog('C', "\t\tRunning at %lu", this->at);
			} else if( !!(arg = striKWcmp( l, "every=" )) ){
				this->every = strtoul( arg.c_str(), NULL, 0 );
				if(verbose)
					publishLog('C', "\t\tRunning every %lu second%c", this->every, this->every > 1 ? 's':' ');
			} else if( l == "immediate" ){
				this->immediate = false;
				if(verbose)
					publishLog('C', "\t\tImmediate");
			} else if( l == "runifover" ){
				this->immediate = false;
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
