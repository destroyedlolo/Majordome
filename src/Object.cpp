#include "Object.h"
#include "Selene.h"
#include "Helpers.h"

#include <fstream>
#include <regex>
#include <filesystem>

#include <cstring>	// strerror()

Object::Object(const std::string &fch, std::string &where) : disabled(false), quiet(false){
}

void Object::loadConfigurationFile(const std::string &fch, std::string &where, std::stringstream *buffer){
	if(::verbose)
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

		if(d2){
			fd2 << this->getFullId() << ": " << this->getName() << std::endl;
			if(!this->description.empty())
				fd2 << this->getFullId() << ".tooltip :" << this->description << std::endl;
			if(!this->embeddedCom.empty())
				fd2 << this->getFullId() << ".comment :" << this->embeddedCom << " { class: Comment }" << std::endl;
		}

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

std::string Object::getContainer( void ){
	return( std::filesystem::path(this->where).filename() );
}

void Object::readConfigDirective(std::string &l){
	std::string arg;

	if(!(arg = striKWcmp( l, "-->> name=" )).empty()){
		this->name = arg;
		if(::verbose)
			SelLog->Log('C', "\t\tChanging name to '%s'", this->getNameC());
	} else if( l == "-->> quiet" ){
		if(::verbose)
			SelLog->Log('C', "\t\tBe quiet");
		this->beQuiet();
	} else if( l == "-->> verbose" ){
		if(::verbose)
			SelLog->Log('C', "\t\tBe verbose");
		this->beVerbose();
	} else if( l == "-->> disabled" ){
		if(::verbose)
			SelLog->Log('C', "\t\tDisabled");
		this->disable();
	} else if(!(arg = striKWcmp( l, "-->> desc=" )).empty()){
		this->description = arg;
		if(::verbose)
			SelLog->Log('C', "\t\tDescription : %s", description.c_str());
	} else if(!(arg = striKWcmp( l, "-->> ecom=" )).empty()){
		this->embeddedCom = arg;
		if(::verbose)
			SelLog->Log('C', "\t\tEmbedded comment : %s", embeddedCom.c_str());
	} else if(!(arg = striKWcmp( l, "-->> group=" )).empty()){
		this->group = arg;
		if(::verbose)
			SelLog->Log('C', "\t\tBelong to group : %s", group.c_str());
	} else if(!striKWcmp( l, "-->> ").empty()){
		SelLog->Log('F', "Unknown directive '%s'", l.c_str());
		exit(EXIT_FAILURE);
	}
}

std::string Object::getFullId( void ){
	std::string ret;

	if(!this->group.empty())
		ret = this->group + '.';
	ret += this->getContainer() + "." + this->getTri() + this->getName();

	return(ret);
}

bool Object::isVerbose(void){
	if(this->verbose)
		return true;
	else if(this->quiet)
		return false;
	else
		return ::verbose;
}
