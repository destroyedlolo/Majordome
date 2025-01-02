/*	Painting Incarnation
 * 	Surface to paint on.
 *
 * 	01/01/2025 - First version
 */

#include "Painting.h"

#include <iostream>
#include <fstream>
#include <sstream>	// stringstream

#include <cstring>
#include <cassert>

Painting::Painting( const std::string &fch, std::string &where, std::string &name, lua_State *L ): surface(NULL), parentR(NULL), parentP(NULL){
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
			if(!!(arg = striKWcmp( l, "-->> Renderer Parent=" ))){
				if(this->parentR || this->parentP){
					SelLog->Log('F', "\t\tA Painting can't have multiple parents");
					exit(EXIT_FAILURE);
				}
	
					// Search the parent renderer
				Config::RendererElements::iterator renderer;
				if( (renderer = config.RendererList.find(arg)) != config.RendererList.end()){
					if(verbose)
						SelLog->Log('C', "\t\tThe Parent is Renderer '%s'", arg.c_str());
					this->parentR = &(renderer->second);
					renderer->second.addPainting( this );
				} else {
					SelLog->Log('F', "\t\tRenderer '%s' is not (yet ?) defined", arg.c_str());
					exit(EXIT_FAILURE);
				}
			} else if( Object::readConfigDirective(l, nameused) )
				// Don't use LuaExec's as "need_??" is not used
				nameused = true;
		} while(true);

		file.close();
	} catch(const std::ifstream::failure &e){
		if(!file.eof()){
			SelLog->Log('F', "%s : %s", fch.c_str(), strerror(errno) );
			exit(EXIT_FAILURE);
		}
	}
}

