#include "MinMax.h"
#include "Config.h"
#include "Helpers.h"

#include <fstream>

#include <cstring>
#include <cassert>


MinMax::MinMax(const std::string &fch, std::string &where, std::string &name, lua_State *L) : Object(fch, where, name), Handler(fch, where, name), empty(true){
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

void MinMax::readConfigDirective( std::string &l, std::string &name, bool &nameused ){
	MayBeEmptyString arg;

	if( !!(arg = striKWcmp( l, "-->> listen=" ))){
		Config::TopicCollection::iterator topic;
		if( (topic = config.TopicsList.find(arg)) != config.TopicsList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tAdded to topic '%s'", arg.c_str());
			topic->second->addHandler( dynamic_cast<Handler *>(this) );
//			nameused = true;
		} else {
			SelLog->Log('F', "\t\tTopic '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else 
		LuaExec::readConfigDirective(l, name, nameused);
}

void MinMax::feedState( lua_State *L ){
	try {
		class MinMax *mm = config.MinMaxList.at( this->getNameC() );
		class MinMax **minmax = (class MinMax **)lua_newuserdata(L, sizeof(class MinMax *));
		assert(minmax);

		lua_pushstring( L, this->getNameC() );	// Push the name of the tracker
		lua_setglobal( L, "MAJORDOME_MINMAX" );

		*minmax = mm;
		luaL_getmetatable(L, "MajordomeMinMax");
		lua_setmetatable(L, -2);
		lua_setglobal( L, "MAJORDOME_Myself" );
	} catch( std::out_of_range &e ){	// Not found 
		SelLog->Log('F', "Can't find minmax '%s'", this->getNameC() );
		exit(EXIT_FAILURE);
	}
}


