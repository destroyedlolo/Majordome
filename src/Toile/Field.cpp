/* Field to display some information
 *
 *	11/01/2025 - First version
 */

#include "Field.h"

#include <iostream>
#include <fstream>
#include <sstream>	// stringstream

#include <cstring>
#include <cassert>

Field::Field( const std::string &fch, std::string &where, std::string &name, lua_State *L ) : Painting(fch, where, name){
#if DEBUG
	if(verbose)
		SelLog->Log('D', "\t\tid : (%p)", this);
#endif

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

		do {
			std::string l;
			pos = file.tellg();

			std::getline( file, l);
			if( l.compare(0, 2, "--") ){	// End of comments
				file.seekg( pos );
				break;
			}

			this->readConfigDirective(l, nameused);
		} while(true);

		file.close();
	} catch(const std::ifstream::failure &e){
		if(!file.eof()){
			SelLog->Log('F', "%s : %s", fch.c_str(), strerror(errno) );
			exit(EXIT_FAILURE);
		}
	}


		/* ***
		 * Sanity checks
		 * ***/
	if(!this->parentR && !this->parentP){
		SelLog->Log('F', "[Painting \"%s\"] No parent defined", this->name.c_str());
		exit(EXIT_FAILURE);
	}

	if(!this->geometry.h){
		this->geometry.h = 1;
		if(verbose)
			SelLog->Log('C', "\t\tHeight defaulted to : 1");
	}
}

void Field::readConfigDirective( std::string &l, bool &nameused ){
	MayBeEmptyString arg;
	if(!!(arg = striKWcmp( l, "-->> Sample=" ))){
		this->geometry.w = arg.length();
		if(verbose)
			SelLog->Log('C', "\t\tWidth guessed to : %u", this->geometry.w);
	} else Painting::readConfigDirective(l, nameused);
}


