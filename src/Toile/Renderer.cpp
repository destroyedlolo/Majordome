/* Renderer object
 * 	Embodies a low-level rendering device.
 */

#include "Renderer.h"
#include "../LuaExec.h"
#include "../Config.h"

#include <iostream>
#include <fstream>
#include <sstream>	// stringstream

#include <cstring>
#include <cassert>

Renderer::Renderer( const std::string &fch, std::string &where, std::string &name, lua_State *L ) : Object(fch, where, name), LuaExec(fch, where, name), surface(NULL), fatal(false){
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

void Renderer::readConfigDirective( std::string &l, std::string &name, bool &nameused ){
	if( l == "-->> disabled" ){
		SelLog->Log('F', "\t\tDisabled is not supported with renderer");
		exit(EXIT_FAILURE);
	} else if( l == "-->> Fatal" ){
		if(verbose)
			SelLog->Log('C', "\t\tFailure is fatal");
		this->fatal = true;
	} else
		LuaExec::readConfigDirective(l, name, nameused);
}

bool Renderer::exec(){	/* From LuaExec::execSync() */
 		/* TODO */
	return false;
}

#ifdef DEBUG
void Renderer::dump(){
	std::cout << "Renderer::dump() of " << static_cast<void*>(this) << std::endl;
	std::cout << "\tName : " << this->getName() << std::endl;
	std::cout << "\tWhere : " << this->getWhere() << std::endl;
	std::cout << "\tsurface : " << static_cast<void*>(this->getSurface()) << std::endl;
}
#endif

void Renderer::refresh(){
	/* TODO */
}

void Renderer::refreshAll(){
	/* TODO */
}
