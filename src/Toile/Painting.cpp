/*	Painting Incarnation
 * 	Surface to paint on.
 *
 * 	01/01/2025 - First version
 */

#include "Painting.h"
#include "../Config.h"

#include <iostream>
#include <fstream>
#include <sstream>	// stringstream

#include <cstring>
#include <cassert>

Painting::Painting( const std::string &fch, std::string &where, lua_State *L ): Object(fch, where), startedvisible(true), persistent(false){
	this->loadConfigurationFile(fch, where);

	if(d2)
		fd2 << this->getFullId() << ".class: Painting" << std::endl;
}

void Painting::assertSanity(void){
	if(!this->isPersistent() && !this->startedvisible){
		SelLog->Log('F', "[\"%s\"] Only a persistent surface can be hidden", this->getNameC());
		exit(EXIT_FAILURE);
	}
	this->ToileObject::assertSanity();
}

bool Painting::readConfigDirective( std::string &l ){
	if(!this->readConfigDirectiveOnly(l))
		return Object::readConfigDirective(l);
	return true;
}

bool Painting::readConfigDirectiveOnly( std::string &l ){
	std::string arg;

	if(!(arg = striKWcmp( l, "-->> Origin=" )).empty()){
		int r = sscanf(arg.c_str(), "%u,%u", &(this->geometry.x), &(this->geometry.y));
		if(r != 2)
			SelLog->Log('W', "Wasn't able to read Origine='s arguments");

		if(::verbose)
			SelLog->Log('C', "\t\tOrigin : %u,%u", this->geometry.x,this->geometry.y);
		return true;
	} else if(!(arg = striKWcmp( l, "-->> Size=" )).empty()){
				int r = sscanf(arg.c_str(), "%ux%u", &(this->geometry.w), &(this->geometry.h));
		if(r != 2)
			SelLog->Log('W', "Wasn't able to read Size='s arguments");

		if(::verbose)
			SelLog->Log('C', "\t\tSize : %ux%u", this->geometry.w,this->geometry.h);
		return true;
	} else if( l == "-->> Hidden" ){
		if(::verbose)
			SelLog->Log('C', "\t\tHidden");
		this->startedvisible = false;

		return true;
	} else if(l == "-->> Persistent"){
		this->persistent = true;
		if(::verbose)
			SelLog->Log('C', "\t\tPersistent");
		return true;
	} else if(!(arg = striKWcmp( l, "-->> ApplyOn Carousel=" )).empty()){	// Only Painting can apply on carousel
		if(this->parent){
			SelLog->Log('F', "\t\tA Painting can have only a single parent");
			exit(EXIT_FAILURE);
		}

			// Search the parent carousel
		CarouselCollection::iterator carousel;
		if((carousel = config.CarouselList.find(arg)) != config.CarouselList.end()){
			if(::verbose)
#ifdef DEBUG
				SelLog->Log('C', "\t\tThe Parent is Carousel '%s' (%p)", arg.c_str(), &(carousel->second));
#else
				SelLog->Log('C', "\t\tThe Parent is Carousel '%s'", arg.c_str());
#endif

			this->parent = dynamic_cast<ToileObject *>(carousel->second);
			carousel->second->addChild( this );

			if(d2)
				fd2 << carousel->second->getFullId() << " <- " << this->getFullId() << ": ApplyOn Carousel { class: llink }" << std::endl;
		} else {
			SelLog->Log('F', "\t\tCarousel '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}

		return true;
	} else
		return this->ToileObject::readConfigDirective(l);
}

#ifdef DEBUG
void Painting::dump(){
	std::cout << "Painting::dump() of " << static_cast<void*>(this) << std::endl;
	std::cout << "\tName : " << this->getName() << std::endl;
	std::cout << "\tWhere : " << this->getWhere() << std::endl;
	std::cout << "\tsurface : " << static_cast<void*>(this->surface) << std::endl;
	std::cout << "\tparent : " << static_cast<void*>(this->getParent()) << std::endl;
	std::cout << "\tOrigin : " << this->geometry.x << "x" << this->geometry.y << std::endl;
	std::cout << "\tSize : " << this->geometry.w << "x" << this->geometry.h << std::endl;
}

#endif

bool Painting::init(void){
printf("******************* %s \n", this->getNameC());
	this->assertSanity();

	if(!this->isEnabled()){
		if(this->isVerbose())
			SelLog->Log('D', "Painting '%s' from '%s' is disabled", this->getNameC(), this->getWhereC());
		return false;
	}

	if(::debug && this->isVerbose())
		SelLog->Log('D', "[%s] Painting::init()", this->getNameC());

	if(this->getParent()->getTri() == Carousel::trigramme()){
		/* As part of a Carousel, the geometry is forced to be the same
		 * as it's parent, to be persistant and started hidden
		 */
		if(this->geometry.w || this->geometry.h || this->geometry.x || this->geometry.y)
			SelLog->Log('F', "[Painting \"%s\"] Getting the geometry from parent is not supported", this->name.c_str());

		auto parent = dynamic_cast<Carousel *>(this->getParent());
		this->geometry = parent->getGeometry();

			// Carousel surface's are hidden and persistant
		this->startedvisible = false;
		this->persistent = true;

	} else if(!this->geometry.w || !this->geometry.h){	// size not set
		uint32_t w,h;
		if(!this->getParent()->getSurface()->cb->getSize(this->getParent()->getSurface(), &w,&h)){
				SelLog->Log('F', "[Painting \"%s\"] Getting the geometry from parent is not supported", this->name.c_str());
			exit(EXIT_FAILURE);
		} else
			SelLog->Log('D', "[Painting \"%s\"] Get geometry from parent : %lux%lu", this->name.c_str(), w,h);

		if(this->geometry.x >= w || this->geometry.y >= h){
			SelLog->Log('W', "[Painting \"%s\"] Origin outsize its parent", this->name.c_str());
			this->geometry.w = w;
			this->geometry.h = h;
		} else {
			this->geometry.w = w - this->geometry.x;
			this->geometry.h = h - this->geometry.y;
		}

		SelLog->Log('D', "[Painting \"%s\"] Guessed geometry : %lux%lu", this->name.c_str(), this->geometry.w,this->geometry.h);
	}

	struct SelGenericSurface *(*srfFunc)(struct SelGenericSurface *, uint32_t,  uint32_t,  uint32_t,  uint32_t, void *) = this->isPersistent() ? this->getParent()->getSurface()->cb->Surface : this->getParent()->getSurface()->cb->subSurface;

	if(!(this->surface = srfFunc( this->getParent()->getSurface(), this->geometry.x, this->geometry.y, this->geometry.w, this->geometry.h, this->getParent()->getSurface()->cb->getPrimary(this->getParent()->getSurface())))){
		SelLog->Log('F', "[Painting \"%s\"] Can't create subsurface", this->name.c_str());
		exit(EXIT_FAILURE);
	}

	if(!this->startedvisible)
		this->getSurface()->cb->setVisibility(this->getSurface(), false);

			// Initialize subsurfaces
	if(::debug && this->isVerbose())
		SelLog->Log('D', "[%s] Painting::init() - Children", this->getNameC());
	for(auto &child: this->getChildren())
		child->init();

	if(::debug && this->isVerbose())
		SelLog->Log('D', "[%s] Painting::init() - End", this->getNameC());

	return true;
}

void Painting::refresh(){
	if(!this->isEnabled()){
		if(this->isVerbose())
			SelLog->Log('D', "Painting '%s' from '%s' is disabled", this->getNameC(), this->getWhereC());
		return;
	}

	if(!this->isVisible())
		return;
}

void Painting::refreshBackground(){
	if(!this->isEnabled()){
		if(this->isVerbose())
			SelLog->Log('D', "Painting '%s' from '%s' is disabled", this->getNameC(), this->getWhereC());
		return;
	}

	if(!this->isVisible())
		return;

	if(::debug && this->isVerbose())
		SelLog->Log('D', "[%s] Painting::refreshBackground()", this->getNameC());

	this->getSurface()->cb->Clear(this->getSurface());
	for(auto &d: this->DecorationsList)
		d->exec(this->getSurface());
}

void Painting::refreshChild(){
#if 0	/* ToDo */
		// refresh childs
	for(auto &paint: this->PaintingList)
		paint->refreshAll();
#endif
}

void Painting::refreshAll(){
	if(!this->isEnabled()){
		if(this->isVerbose())
			SelLog->Log('D', "Painting '%s' from '%s' is disabled", this->getNameC(), this->getWhereC());
		return;
	}

	this->refreshBackground();
	this->refreshChild();

	this->refresh();
}

	/*****
	 * Lua exposed functions
	 *****/

static class Painting *checkMajordomePainting(lua_State *L){
	class Painting **r = (class Painting **)SelLua->testudata(L, 1, "MajordomePainting");
	luaL_argcheck(L, r != NULL, 1, "'MajordomePainting' expected");
	return *r;
}

static int ltp_getContainer(lua_State *L){
	class Painting *painting= checkMajordomePainting(L);
	lua_pushstring( L, painting->getWhereC() );
	return 1;
}

static int ltp_getName(lua_State *L){
	class Painting *painting= checkMajordomePainting(L);
	lua_pushstring( L, painting->getName().c_str() );
	return 1;
}

static int ltp_isEnabled( lua_State *L ){
	class Painting *painting= checkMajordomePainting(L);
	lua_pushboolean( L, painting->isEnabled() );
	return 1;
}

static int ltp_enabled( lua_State *L ){
	class Painting *painting= checkMajordomePainting(L);
	painting->enable();
	return 0;
}

static int ltp_disable( lua_State *L ){
	class Painting *painting= checkMajordomePainting(L);
	painting->disable();
	return 0;
}

static int ltp_isVisible( lua_State *L ){
	class Painting *painting= checkMajordomePainting(L);
	lua_pushboolean( L, painting->ToileObject::isVisible() );	//bypass local status for persistant surfaces
	return 1;
}

static int ltp_setVisibility( lua_State *L ){
	class Painting *painting= checkMajordomePainting(L);
	bool v = lua_toboolean(L, 2);

	if((::debug || painting->isVerbose()) && !painting->isPersistent())
		SelLog->Log('W', "[%s] The visibility can be changed only on Persistent Painting", painting->getNameC());

	painting->getSurface()->cb->setVisibility(painting->getSurface(), v);

	return 0;
}

static const struct luaL_Reg MajTPaintM [] = {
	{"getContainer", ltp_getContainer},
 	{"getName", ltp_getName},
	{"isEnabled", ltp_isEnabled},
	{"Enable", ltp_enabled},
	{"Disable", ltp_disable},
	{"isVisible", ltp_isVisible},
	{"setVisibility", ltp_setVisibility},
	{NULL, NULL}
};

		/* ***
		 * Painting -> SelGenericSurface wrapper
		 * ***/

static int Selene_wrapper(lua_State *L){
	class Painting *painting = checkMajordomePainting(L);
	
	class SelGenericSurfaceLua *srf = (class SelGenericSurfaceLua *)lua_newuserdata(L, sizeof(class SelGenericSurfaceLua));
	assert(srf);

	srf->storage = painting->getSurface();
	luaL_getmetatable(L, painting->getSurface()->cb->LuaObjectName() );
	lua_setmetatable(L, -2);

	lua_replace(L, 1);	// swap the initial object (#1) by the surface one.

	lua_pushvalue(L, lua_upvalueindex(1));	// Get the methods
	lua_insert(L, 1);	// put it before the 1st argument

	lua_call(L, lua_gettop(L) - 1, LUA_MULTRET);

	return lua_gettop(L);
}

static int painting_custom_index(lua_State *L){
	// input stack : 1: Painting, 2: method
	class Painting *painting = checkMajordomePainting(L);

		/* Search in painting */
	luaL_getmetatable(L, "MajordomePainting");	// is in Painting ?
	lua_pushvalue(L, 2);
	lua_rawget(L, -2);
	if(!lua_isnil(L, -1))	// ok, return it
		return 1;

	lua_pop(L, 2);	// clean 'nil' and the metatable

		/* Search in Séléné */
	luaL_getmetatable(L, painting->getSurface()->cb->LuaObjectName());
	if(lua_isnil(L, -1)){
		SelLog->Log('E', "Can't find %s's meta", painting->getSurface()->cb->LuaObjectName());
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

void Painting::initLuaInterface( lua_State *L ){
	SelLua->objFuncs( L, "MajordomePainting", MajTPaintM );
//	SelLua->libCreateOrAddFuncs( L, "MajordomeFeed", MajFeedLib );

		/* Activate the wrapper */
	luaL_getmetatable(L, "MajordomePainting");
	if(!lua_isnil(L, -1)){
		lua_pushstring(L, "__index");
		lua_pushcfunction(L, painting_custom_index);
		lua_settable(L, -3); // metatable.__index = painting_custom_index
	} else
		SelLog->Log('E', "Can't find MajordomePainting's meta");
	lua_pop(L, 1);		// Cleaning getmetatable()
}
