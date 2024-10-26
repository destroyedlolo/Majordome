#include <iostream>
#include <fstream>
#include <sstream>	// stringstream

#include <cstring>
#include <cassert>

extern "C" {
    #include "lualib.h"
    #include "lauxlib.h"
};

#include "Config.h"
#include "Helpers.h"
#include "MinMax.h"

MinMax::MinMax( const std::string &fch, std::string &where, std::string &name, lua_State *L ): empty(true){
	if(verbose)
		SelLog->Log('L', "\t'%s'", fch.c_str());

	this->extrName( fch, name );
	this->name = name;
	this->where = where;

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

			MayBeEmptyString arg;
			if( !!(arg = striKWcmp( l, "-->> name=" ))){
				if( nameused ){
					SelLog->Log('F', "\t\tName can be changed only before listen, until or waitfor directives");
					exit(EXIT_FAILURE);
				}

				this->name = name = arg;
				if(verbose)
					SelLog->Log('C', "\t\tChanging name to '%s'", name.c_str());
			} else if( !!(arg = striKWcmp( l, "-->> listen=" ))){
				Config::TopicElements::iterator topic;
				if( (topic = config.TopicsList.find(arg)) != config.TopicsList.end()){
					if(verbose)
						SelLog->Log('C', "\t\tAdded to topic '%s'", arg.c_str());
	 				topic->second.addMinMax( this->getName() );
					nameused = true;
				} else {
					SelLog->Log('F', "\t\tTopic '%s' is not (yet ?) defined", arg.c_str());
					exit(EXIT_FAILURE);
				}
			}
		} while(true);

		/*
		 * Reading the remaining of the script and keep it as 
		 * an Lua's script
		 */

		buffer << file.rdbuf();
		file.close();
	} catch(const std::ifstream::failure &e){
		if(!file.eof()){
			SelLog->Log('F', "%s : %s", fch.c_str(), strerror(errno) );
			exit(EXIT_FAILURE);
		}
	}

	if( !this->LoadFunc( L, buffer, this->name.c_str() ))
		exit(EXIT_FAILURE);
}

bool MinMax::exec( const char *name, const char *topic, const char *payload ){
	if( !this->isEnabled() ){
		if(verbose)
			SelLog->Log('T', "MinMax'%s' from '%s' is disabled", this->getNameC(), this->getWhereC() );
		return false;
	}

	LuaExec::boolRetCode rc;
	bool r = this->LuaExec::execSync(name, topic, payload, true, &rc);

	if( rc != LuaExec::boolRetCode::RCfalse ){
		if(debug)
			SelLog->Log('T', "MinMax '%s' from '%s' is accepting \"%s\"", this->getNameC(), this->getWhereC(), payload );

		double val = atof(payload);
		if(this->empty){
			this->empty = false;
			this->nbre = 1;
			this->min = this->max = this->sum = val; 
		} else {
			if(val < this->min)
				this->min = val;
			if(val > this->max)
				this->max = val;
		}

		if(debug)
			SelLog->Log('T', "MinMax '%s' min:%.0f max:%.0f", this->getNameC(), this->min, this->max);
	}

	return r;
}
