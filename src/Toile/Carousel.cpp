/*	Carousel Incarnation
 * 	Cycle through a list of paintings
 *
 * 	08/07/2026 - First version
 */

#include "Carousel.h"
#include "../Config.h"

#include <iostream>
#include <fstream>
#include <sstream>	// stringstream

#include <cstring>
#include <cassert>

Carousel::Carousel( const std::string &fch, std::string &where, lua_State *L ): Object(fch, where){
	this->loadConfigurationFile(fch, where);

	if(d2)
		fd2 << this->getFullId() << ".class: Carousel" << std::endl;
}

bool Carousel::readConfigDirective( std::string &l ){
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
	} else
		return this->ToileObject::readConfigDirective(l);
}

#ifdef DEBUG
void Carousel::dump(){
	std::cout << "Carousel::dump() of " << static_cast<void*>(this) << std::endl;
	std::cout << "\tName : " << this->getName() << std::endl;
	std::cout << "\tWhere : " << this->getWhere() << std::endl;
	std::cout << "\tsurface : " << static_cast<void*>(this->surface) << std::endl;
	std::cout << "\tparent : " << static_cast<void*>(this->getParent()) << std::endl;
	std::cout << "\tOrigin : " << this->geometry.x << "x" << this->geometry.y << std::endl;
	std::cout << "\tSize : " << this->geometry.w << "x" << this->geometry.h << std::endl;
}
#endif

void Carousel::assertSanity(void){
	if(!this->getChildren().size()){
		SelLog->Log('F', "[\"%s\"] Carousels are useless without attached painting", this->getNameC());
		exit(EXIT_FAILURE);
	}
	this->ToileObject::assertSanity();
}

bool Carousel::init(void){
	this->assertSanity();

	if(!this->isEnabled()){
		if(this->isVerbose())
			SelLog->Log('D', "Carousel '%s' from '%s' is disabled", this->getNameC(), this->getWhereC());
		return false;
	}

	if(::debug && this->isVerbose())
		SelLog->Log('D', "[%s] Carousel::init()", this->getNameC());

	if(!this->geometry.w || !this->geometry.h){	// size not set
		uint32_t w,h;
		if(!this->getParent()->getSurface()->cb->getSize(this->getParent()->getSurface(), &w,&h)){
				SelLog->Log('F', "[Carousel \"%s\"] Getting the geometry from parent is not supported", this->name.c_str());
			exit(EXIT_FAILURE);
		} else
			SelLog->Log('D', "[Carousel \"%s\"] Get geometry from parent : %lux%lu", this->name.c_str(), w,h);

		if(this->geometry.x >= w || this->geometry.y >= h){
			SelLog->Log('W', "[Carousel \"%s\"] Origin outsize its parent", this->name.c_str());
			this->geometry.w = w;
			this->geometry.h = h;
		} else {
			this->geometry.w = w - this->geometry.x;
			this->geometry.h = h - this->geometry.y;
		}

		SelLog->Log('D', "[Carousel \"%s\"] Guessed geometry : %lux%lu", this->name.c_str(), this->geometry.w,this->geometry.h);
	}

/*
	struct SelGenericSurface *(*srfFunc)(struct SelGenericSurface *, uint32_t,  uint32_t,  uint32_t,  uint32_t, void *) = this->isPersistent() ? this->getParent()->getSurface()->cb->Surface : this->getParent()->getSurface()->cb->subSurface;*/

	if(!(this->surface = this->getParent()->getSurface()->cb->Surface( this->getParent()->getSurface(), this->geometry.x, this->geometry.y, this->geometry.w, this->geometry.h, this->getParent()->getSurface()->cb->getPrimary(this->getParent()->getSurface())))){
		SelLog->Log('F', "[Painting \"%s\"] Can't create carousel", this->name.c_str());
		exit(EXIT_FAILURE);
	}

			// Initialize subsurfaces
	if(::debug && this->isVerbose())
		SelLog->Log('D', "[%s] Painting::init() - Children", this->getNameC());

	for(auto &child: this->getChildren())
		child->init();

		// makes the 1set child visible
	this->getChildren().front()->getSurface()->cb->setVisibility(this->getChildren().front()->getSurface(), true);

	if(::debug && this->isVerbose())
		SelLog->Log('D', "[%s] Carousel::init() - End", this->getNameC());

	return true;
}


void Carousel::refreshAll(){
	if(!this->isEnabled() || !this->isVisible())
		return;

	for(auto &child : this->getChildren()){
		// As all children need to be persistent, no need to check if it is visible or not
		child->refreshAll();
	}
}

void Carousel::initLuaInterface( lua_State *L ){
}
