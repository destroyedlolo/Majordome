#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

#include "Helpers.h"
#include "Timer.h"

Timer::Timer( const std::string &fch, std::string &where, std::string &name ) : sample(0), at(-1) {
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
