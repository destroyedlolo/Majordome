#include <iostream>
#include <fstream>
#include <sstream>	// stringstream

#include <cstring>
#include <cassert>

#include "Components.h"
#include "LuaTask.h"

LuaTask::LuaTask( const std::string &fch, std::string &where, std::string &name, lua_State *L ) : once( false ){
#ifdef DEBUG
	publishLog('L', "\t'%s'", fch.c_str());
#endif

	assert( EStorage_init(&this->func) );

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
			if( !!(arg = striKWcmp( l, "-->> name=" ))){
				this->name = name = arg;
#ifdef DEBUG
				publishLog('D', "\t\tChanging name to '%s'", name.c_str());
#endif
			} else if( l == "-->> once" ){
#ifdef DEBUG
				publishLog('D', "\t\tOnly one instance is allowed to run (once)");
#endif
				this->setOnce( true );
			} else if( l == "-->> disabled" ){
#ifdef DEBUG
				publishLog('D', "\t\tDisabled");
#endif
				this->disable();
			}
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
			publishLog('F', "%s : %s", fch.c_str(), strerror(errno) );
			exit(EXIT_FAILURE);
		}
	}

puts( buffer.str().c_str() );
//	lua_load(
}
