#include "Timer.h"
#include "Helpers.h"
#include "Config.h"

#include <fstream>

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
	} else 
		Object::readConfigDirective(l, name, nameused);
}
