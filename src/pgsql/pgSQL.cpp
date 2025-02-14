#include "pgSQL.h"
#include "../Config.h"
#include "../Helpers.h"

#include <fstream>

#include <cstring>
#include <cassert>

pgSQL::pgSQL(const std::string &fch, std::string &where, std::string &name) : Object(fch, where, name){

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

void pgSQL::readConfigDirective( std::string &l, std::string &name, bool &nameused ){
	MayBeEmptyString arg;

	if( !!(arg = striKWcmp( l, "-->> pgConnectionstring=" ))){
		this->conninfo = arg;

		if(verbose)
			SelLog->Log('C', "\t\tConnexion string : '%s'", this->conninfo.c_str());
	} else 
		this->Object::readConfigDirective(l, name, nameused);
}
