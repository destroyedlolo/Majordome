#include "Object.h"
#include "Selene.h"
#include "Helpers.h"

#include <fstream>
#include <regex>

#include <cstring>	// strerror()

Object::Object(const std::string &fch, std::string &where) : disabled(false), quiet(false){
}

void Object::loadConfigurationFile(const std::string &fch, std::string &where, std::stringstream *buffer){
	if(verbose)
		SelLog->Log('L', "\t'%s'", fch.c_str());

	this->extrName( fch, name );
	this->name = name;
	this->where = where;

		/*
		 * Read the configuration file
		 */
	std::ifstream file;
	file.exceptions ( std::ios::eofbit | std::ios::failbit );
	try {
		std::ifstream file(fch);
		std::streampos pos;

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

			this->readConfigDirective(l);
		} while(true);


		/*
		 * Reading the remaining of the script and keep it as 
		 * an Lua's script
		 */
		if(buffer)
			*buffer << file.rdbuf();
		file.close();
	} catch(const std::ifstream::failure &e){
		if(!file.eof()){
			SelLog->Log('F', "%s : %s", fch.c_str(), strerror(errno) );
			exit(EXIT_FAILURE);
		}
	}
}

void Object::extrName( const std::string &fch, std::string &name){
	name = fch;
	const size_t last_slash_idx = name.find_last_of("/");	// Filename only
	if(std::string::npos != last_slash_idx)
		name.erase(0, last_slash_idx + 1);

	const size_t period_idx = name.rfind('.');	// Remove extension if present.
	if (std::string::npos != period_idx)
		name.erase(period_idx);
}

void Object::readConfigDirective(std::string &l){
	std::string arg;

	if(!(arg = striKWcmp( l, "-->> name=" )).empty()){
		this->name = arg;
		if(verbose)
			SelLog->Log('C', "\t\tChanging name to '%s'", this->getNameC());
	} else if( l == "-->> quiet" ){
		if(verbose)
			SelLog->Log('C', "\t\tBe quiet");
		this->beQuiet();
	} else if( l == "-->> disabled" ){
		if(verbose)
			SelLog->Log('C', "\t\tDisabled");
		this->disable();
	} else if(!(arg = striKWcmp( l, "-->> desc=" )).empty()){
		this->description = arg;
		if(verbose)
			SelLog->Log('C', "\t\tDescription : %s", description.c_str());
	} else if(!striKWcmp( l, "-->> ").empty()){
		SelLog->Log('F', "Unknown directive '%s'", l.c_str());
		exit(EXIT_FAILURE);
	}
}
