#include "Config.h"
#include "Helpers.h"
#include "Shutdown.h"

#include <iostream>
#include <fstream>

#include <cstring>
#include <cassert>

Shutdown::Shutdown(const std::string &fch, std::string &where, std::string &name, lua_State *L) : Object(fch, where, name), LuaExec(fch, where, name){
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

void Shutdown::exec( void ){
	lua_State *L = createLuaState();
	if(L){
		threadEnvironment(L);	// Feed environment with generals
		if(this->feedbyNeeded(L))
			this->execSync(L);
		lua_close(L);
	}
}
