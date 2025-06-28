/* Decoration object
 * 	Task to be called when an object needs to be refreshed
 */

#include "../Config.h"
#include "Decoration.h"

#include <iostream>
#include <fstream>
#include <sstream>	// stringstream

#include <cstring>
#include <cassert>

Decoration::Decoration( const std::string &fch, std::string &where, lua_State *L ) : Object(fch, where), LuaExec(fch, where){
	this->loadConfigurationFile(fch, where, L);

	if(d2)
		fd2 << this->getFullId() << ".class: Decoration" << std::endl;
}

bool Decoration::readConfigDirective( std::string &l ){
	std::string arg;
	if(!(arg = striKWcmp( l, "-->> ApplyOnRenderer=" )).empty()){
			// Search the renderer to apply on
		RendererCollection::iterator renderer;
		if((renderer = config.RendererList.find(arg)) != config.RendererList.end()){
			if(::verbose)
				SelLog->Log('C', "\t\tAdded to renderer '%s'", arg.c_str());
			renderer->second->addDecoration( this );

			if(d2)
				fd2 << renderer->second->getFullId() << " <- " << this->getFullId() << ": ApplyOnRenderer { class: llink }" << std::endl;
		} else {
			SelLog->Log('F', "\t\tRenderer '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(!(arg = striKWcmp( l, "-->> ApplyOn=" )).empty()){
			// Search the Painting to apply on
		PaintingCollection::iterator paint;
		if((paint = config.PaintingList.find(arg)) != config.PaintingList.end()){
			if(::verbose)
				SelLog->Log('C', "\t\tAdded to Painting '%s'", arg.c_str());
			paint->second->addDecoration( this );

			if(d2)
				fd2 << paint->second->getFullId() << " <- " << this->getFullId() << ": ApplyOn { class: llink }" << std::endl;
		} else {
			SelLog->Log('F', "\t\tPainting '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(this->ToileObject::readConfigDirective(l))
		;
	else
		return this->Object::readConfigDirective(l);
	return true;
}

void Decoration::exec(struct SelGenericSurface *srf){	/* From LuaExec::execSync() */
	if(!this->isVisible())
		return;

	if(!this->canRun()){
		if(this->isVerbose())
			SelLog->Log('D', "Decoration '%s' from '%s' is disabled", this->getNameC(), this->getWhereC());
		return;
	}

	if(::debug && this->isVerbose())
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

	if(this->isVerbose())
		SelLog->Log('T', "Running Decoration '%s' from '%s'", this->getNameC(), this->getWhereC() );

	if(lua_pcall( L, 0, 0, 0)){
		SelLog->Log('E', "Can't execute Decoration '%s' from '%s' : %s", this->getNameC(), this->getWhereC(), lua_tostring(L, -1));
		return;
	}
		/* cleaning */
	lua_close(L);

	if(::debug && this->isVerbose())
		SelLog->Log('D', "Decoration::exec() - End");

	return;
}

void Decoration::exec(Renderer &rd){ this->exec(rd.getSurface()); };
void Decoration::exec(Painting &pt){ this->exec(pt.getSurface()); };

