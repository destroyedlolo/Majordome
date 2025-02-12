/* Decoration object
 * 	Task to be called when an object needs to be refreshed
 */

#include "Decoration.h"

#include <iostream>
#include <fstream>
#include <sstream>	// stringstream

#include <cstring>
#include <cassert>

extern "C" {
    #include "lualib.h"
    #include "lauxlib.h"
};


Decoration::Decoration( const std::string &fch, std::string &where, std::string &name, lua_State *L ) : Object(fch, where, name){
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
			if(!!(arg = striKWcmp( l, "-->> ApplyOnRenderer=" ))){
					// Search the renderer to apply on
				Config::RendererElements::iterator renderer;
				if( (renderer = config.RendererList.find(arg)) != config.RendererList.end()){
					if(verbose)
						SelLog->Log('C', "\t\tAdded to renderer '%s'", arg.c_str());
					nameused = true;
					renderer->second->addDecoration( this->name );
				} else {
					SelLog->Log('F', "\t\tRenderer '%s' is not (yet ?) defined", arg.c_str());
					exit(EXIT_FAILURE);
				}
			} else if(!!(arg = striKWcmp( l, "-->> ApplyOn=" ))){
					// Search the Painting to apply on
				Config::PaintingElements::iterator paint;
				if( (paint = config.PaintingList.find(arg)) != config.PaintingList.end()){
					if(verbose)
						SelLog->Log('C', "\t\tAdded to Painting '%s'", arg.c_str());
					nameused = true;
					paint->second->addDecoration( this->name );
				} else {
					SelLog->Log('F', "\t\tPainting '%s' is not (yet ?) defined", arg.c_str());
					exit(EXIT_FAILURE);
				}
			} else if( Object::readConfigDirective(l, nameused) )
				// Don't use LuaExec's as "need_??" is not used
				nameused = true;
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

void Decoration::exec(struct SelGenericSurface *srf){	/* From LuaExec::execSync() */
	if(debug)
		SelLog->Log('D', "Decoration::exec(%p)", srf);

		/* Put the cursor to its origin
		 * The surface is not cleared : if needed it has to be done
		 * explicitly !
		 */
	srf->cb->Home(srf);

	lua_State *L = luaL_newstate();
	if( !L ){
		SelLog->Log('E', "Unable to create a new Lua State for '%s' from '%s'", this->getNameC(), this->getWhereC() );
		return;
	}

	luaL_openlibs(L);
	threadEnvironment(L);

	class SelGenericSurfaceLua *renderer = (class SelGenericSurfaceLua *)lua_newuserdata(L, sizeof(class SelGenericSurfaceLua));
	assert(renderer);

	renderer->storage = srf;
	luaL_getmetatable(L, srf->cb->LuaObjectName() );
	lua_setmetatable(L, -2);
	lua_setglobal( L, "MAJORDOME_PAINTING" );

	int err;
	if( (err = SelElasticStorage->loadsharedfunction( L, this->getFunc() )) ){
		SelLog->Log('E', "Unable to create Decoration '%s' from '%s' : %s", this->getNameC(), this->getWhereC(), (err == LUA_ERRSYNTAX) ? "Syntax error" : "Memory error" );
		lua_close( L );
		return;
	}

	if(verbose && !this->isQuiet())
		SelLog->Log('T', "Running Decoration '%s' from '%s'", this->getNameC(), this->getWhereC() );

	if(lua_pcall( L, 0, 0, 0)){
		SelLog->Log('E', "Can't execute Decoration '%s' from '%s' : %s", this->getNameC(), this->getWhereC(), lua_tostring(L, -1));
		return;
	}
		/* cleaning */
	lua_close(L);

	if(debug)
		SelLog->Log('D', "Decoration::exec() - End");

	return;
}

void Decoration::exec(Renderer &rd){ this->exec(rd.getSurface()); };
void Decoration::exec(Painting &pt){ this->exec(pt.getSurface()); };

