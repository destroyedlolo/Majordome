/* Task to be launched at Majordome's shutdown
 *
 * 9/12/2024 - LF - First version
 */

#include <iostream>
#include <fstream>

#include <cstring>
#include <cassert>

extern "C" {
    #include "lualib.h"
    #include "lauxlib.h"
};

#include "Config.h"
#include "Helpers.h"
#include "Shutdown.h"

Shutdown::Shutdown( const std::string &fch, std::string &where, std::string &name, lua_State *L ){
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

			MayBeEmptyString arg;
			if( LuaExec::readConfigDirective(l, nameused) )
				nameused = true;
#if 0
else printf("Ignore '%s'\n", l.c_str());
#endif
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

void Shutdown::exec( void ){
	if( !this->isEnabled() ){
		if(verbose)
			SelLog->Log('T', "Shutdown '%s' from '%s' is disabled", this->getNameC(), this->getWhereC() );
		return;
	}

	lua_State *L = luaL_newstate();
	if( !L ){
		SelLog->Log('E', "Unable to create a new Lua State for '%s' from '%s'", this->getNameC(), this->getWhereC() );
		return;
	}

	luaL_openlibs(L);
	threadEnvironment(L);
	if(!this->feedbyNeeded(L, false)){
		lua_close( L );
		return;
	}

	int err;
	if( (err = SelElasticStorage->loadsharedfunction( L, this->getFunc() )) ){
		SelLog->Log('E', "Unable to create Shutdown '%s' from '%s' : %s", this->getNameC(), this->getWhereC(), (err == LUA_ERRSYNTAX) ? "Syntax error" : "Memory error" );
		lua_close( L );
		return;
	}

	if(verbose && !this->isQuiet())
		SelLog->Log('T', "Running Shutdown '%s' from '%s'", this->getNameC(), this->getWhereC() );

	if(lua_pcall( L, 0, 0, 0))
		SelLog->Log('E', "Can't execute Shutdown '%s' from '%s' : %s", this->getNameC(), this->getWhereC(), lua_tostring(L, -1));

	lua_close(L);
}
