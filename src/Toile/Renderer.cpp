/* Renderer object
 * 	Embodies a low-level rendering device.
 */

#include "Renderer.h"
#include "Painting.h"
#include "Decoration.h"
#include "../LuaExec.h"
#include "../Config.h"

#include <iostream>
#include <fstream>
#include <sstream>	// stringstream

#include <cstring>
#include <cassert>

Renderer::Renderer( const std::string &fch, std::string &where, lua_State *L ) : Object(fch, where), LuaExec(fch, where), surface(NULL), fatal(false){
	this->loadConfigurationFile(fch, where, L);

	if(d2)
		fd2 << this->getFullId() << ".class: Renderer" << std::endl;
}

void Renderer::readConfigDirective( std::string &l ){
	if( l == "-->> Fatal" ){
		if(verbose)
			SelLog->Log('C', "\t\tFailure is fatal");
		this->fatal = true;
	} else
		this->LuaExec::readConfigDirective(l);
}

bool Renderer::exec(){	/* From LuaExec::execSync() */
	if(debug)
		SelLog->Log('D', "Renderer::exec()");

	lua_State *L = luaL_newstate();
	if( !L ){
		SelLog->Log('E', "Unable to create a new Lua State for '%s' from '%s'", this->getNameC(), this->getWhereC() );
		return false;
	}

	luaL_openlibs(L);
	threadEnvironment(L);

	int err;
	if( (err = SelElasticStorage->loadsharedfunction( L, this->getFunc() )) ){
		SelLog->Log('E', "Unable to Renderer task '%s' from '%s' : %s", this->getNameC(), this->getWhereC(), (err == LUA_ERRSYNTAX) ? "Syntax error" : "Memory error" );
		lua_close( L );
		return false;
	}

	if(verbose && !this->isQuiet())
		SelLog->Log('T', "Running Renderer '%s' from '%s'", this->getNameC(), this->getWhereC() );

	if(lua_pcall( L, 0, 1, 0)){
		SelLog->Log('E', "Can't execute Renderer '%s' from '%s' : %s", this->getNameC(), this->getWhereC(), lua_tostring(L, -1));
		return false;
	}

		/* Notez-Bien : we are checking only for userdata but there is strictly
		 * no way to ensure it's a SelGenericSurface derivate.
		 * Providing bullshits leads to crashing.
		 */
	struct SelGenericSurfaceLua *srf = (struct SelGenericSurfaceLua *)lua_touserdata(L, -1);

	if(!srf || !checkCapabilities((SelObject *)srf->storage, SELCAP_RENDERER)){
		SelLog->Log('E', "Not suitable object returned by Renderer code");
		return false;
	}
	this->surface = srf->storage;

		/* cleaning */
	lua_close(L);

	if(debug)
		SelLog->Log('D', "Renderer::exec() - End");

	return true;
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
	for(auto &d: this->DecorationsList)
		d->exec(*this);
}

void Renderer::refreshChild(){
		// Refresh subsurface
	for(auto &paint: this->PaintingList)
		paint->refreshAll();
}

void Renderer::refreshAll(){
	this->refresh();
	this->refreshChild();
}
