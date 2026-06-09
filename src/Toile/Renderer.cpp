/* Renderer object
 * 	Embodies a low-level rendering device.
 */

#include "Renderer.h"
#include "Decoration.h"
#include "../LuaExec.h"
#include "../Config.h"

#include <iostream>
#include <fstream>
#include <sstream>	// stringstream

#include <cstring>
#include <cassert>

Renderer::Renderer( const std::string &fch, std::string &where, lua_State *L ) : Object(fch, where), LuaExec(fch, where), fatal(false){
	this->loadConfigurationFile(fch, where, L);

	if(d2)
		fd2 << this->getFullId() << ".class: Renderer" << std::endl;
}

bool Renderer::readConfigDirective( std::string &l ){
	if( l == "-->> Fatal" ){
		if(::verbose)
			SelLog->Log('C', "\t\tFailure is fatal");
		this->fatal = true;
	} else
		return this->LuaExec::readConfigDirective(l);
	
	return true;
}

bool Renderer::init(void){	/* From LuaExec::execSync() */
	if(!this->canRun()){
		if(this->isVerbose())
			SelLog->Log('D', "Renderer '%s' from '%s' is disabled", this->getNameC(), this->getWhereC());
		return false;
	}

	if(::debug && this->isVerbose())
		SelLog->Log('D', "Renderer::init()");

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

	if(this->isVerbose())
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

	if(::debug && this->isVerbose())
		SelLog->Log('D', "Renderer::init() - End");

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
		d->exec(this->getSurface());
}

void Renderer::refreshChild(){
		// Refresh subsurface
	for(auto &child: this->getChildren())
		child->refreshAll();
}

void Renderer::refreshAll(){
	this->refresh();
	this->refreshChild();

		// Physically refresh the screen
	if(::debug && this->isVerbose())
		this->getSurface()->cb->Dump(this->getSurface());
	this->getSurface()->cb->Refresh(this->getSurface());
}


	/*****
	 * Lua exposed functions
	 *****/

static class Renderer *checkMajordomeRenderer(lua_State *L){
	class Renderer **r = (class Renderer **)SelLua->testudata(L, 1, "MajordomeRenderer");
	luaL_argcheck(L, r != NULL, 1, "'MajordomeRenderer' expected");
	return *r;
}

static int ltr_getContainer(lua_State *L){
	class Renderer *renderer= checkMajordomeRenderer(L);
	lua_pushstring( L, renderer->getWhereC() );
	return 1;
}

static int ltr_getName(lua_State *L){
	class Renderer *renderer= checkMajordomeRenderer(L);
	lua_pushstring( L, renderer->getName().c_str() );
	return 1;
}

static int ltr_isEnabled( lua_State *L ){
	class Renderer *renderer= checkMajordomeRenderer(L);
	lua_pushboolean( L, renderer->isEnabled() );
	return 1;
}

static int ltr_enabled( lua_State *L ){
	class Renderer *renderer= checkMajordomeRenderer(L);
	renderer->enable();
	return 0;
}

static int ltr_disable( lua_State *L ){
	class Renderer *renderer= checkMajordomeRenderer(L);
	renderer->disable();
	return 0;
}

static int ltr_isVisible( lua_State *L ){
	class Renderer *renderer= checkMajordomeRenderer(L);
	lua_pushboolean( L, renderer->isVisible() );
	lua_pushboolean( L, renderer->getOwnVisibility() );
	return 2;
}

static const struct luaL_Reg MajTPaintM [] = {
	{"getContainer", ltr_getContainer},
 	{"getName", ltr_getName},
	{"isEnabled", ltr_isEnabled},
	{"Enable", ltr_enabled},
	{"Disable", ltr_disable},
	{"isVisible", ltr_isVisible},
	{NULL, NULL}
};

		/* ***
		 * Renderer -> SelGenericSurface wrapper
		 * ***/

static int Selene_wrapper(lua_State *L){
	class Renderer *renderer = checkMajordomeRenderer(L);
	
	class SelGenericSurfaceLua *srf = (class SelGenericSurfaceLua *)lua_newuserdata(L, sizeof(class SelGenericSurfaceLua));
	assert(srf);

	srf->storage = renderer->getSurface();
	luaL_getmetatable(L, renderer->getSurface()->cb->LuaObjectName() );
	lua_setmetatable(L, -2);

	lua_replace(L, 1);	// swap the initial object (#1) by the surface one.

	lua_pushvalue(L, lua_upvalueindex(1));	// Get the methods
	lua_insert(L, 1);	// put it before the 1st argument

	lua_call(L, lua_gettop(L) - 1, LUA_MULTRET);

	return lua_gettop(L);
}

static int renderer_custom_index(lua_State *L){
	// input stack : 1: Renderer, 2: method
	class Renderer *renderer = checkMajordomeRenderer(L);

		/* Search in renderer */
	luaL_getmetatable(L, "MajordomeRenderer");	// is in Renderer ?
	lua_pushvalue(L, 2);
	lua_rawget(L, -2);
	if(!lua_isnil(L, -1))	// ok, return it
		return 1;

	lua_pop(L, 2);	// clean 'nil' and the metatable

		/* Search in Séléné */
	luaL_getmetatable(L, renderer->getSurface()->cb->LuaObjectName());
	if(lua_isnil(L, -1)){
		SelLog->Log('E', "Can't find %s's meta", renderer->getSurface()->cb->LuaObjectName());
		lua_pop(L, 1);
		return 0;
	}
	
	lua_pushvalue(L, 2);
	lua_gettable(L, -2);	// Lookup in Séléné methods

	if(lua_isfunction(L, -1)){	// Found it as a function, use the wrapper
		lua_pushcclosure(L, Selene_wrapper, 1);
		return 1;
	}

		// not found or not a function : return what we got
	return 1;
}

void Renderer::initLuaInterface( lua_State *L ){
	SelLua->objFuncs( L, "MajordomeRenderer", MajTPaintM );
//	SelLua->libCreateOrAddFuncs( L, "MajordomeFeed", MajFeedLib );

		/* Activate the wrapper */
	luaL_getmetatable(L, "MajordomeRenderer");
	if(!lua_isnil(L, -1)){
		lua_pushstring(L, "__index");
		lua_pushcfunction(L, renderer_custom_index);
		lua_settable(L, -3); // metatable.__index = renderer_custom_index
	} else
		SelLog->Log('E', "Can't find MajordomeRenderer's meta");
	lua_pop(L, 1);		// Cleaning getmetatable()
}
