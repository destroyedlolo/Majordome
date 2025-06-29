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

Painting::Painting( const std::string &fch, std::string &where, lua_State *L ): Object(fch, where), surface(NULL){
	this->loadConfigurationFile(fch, where);

	if(d2)
		fd2 << this->getFullId() << ".class: Painting" << std::endl;


		/* ***
		 * Sanity checks
		 * ***/
	if(!this->parentR && !this->parentP){
		SelLog->Log('F', "[Painting \"%s\"] No parent defined", this->name.c_str());
		exit(EXIT_FAILURE);
	}
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
	} else
		return this->ToileObject::readConfigDirective(l);
}

#ifdef DEBUG
void Painting::dump(){
	std::cout << "Painting::dump() of " << static_cast<void*>(this) << std::endl;
	std::cout << "\tName : " << this->getName() << std::endl;
	std::cout << "\tWhere : " << this->getWhere() << std::endl;
	std::cout << "\tsurface : " << static_cast<void*>(this->surface) << std::endl;
	std::cout << "\tparentR : " << static_cast<void*>(this->parentR) << std::endl;
	std::cout << "\tparentP : " << static_cast<void*>(this->parentP) << std::endl;
	std::cout << "\tOrigin : " << this->geometry.x << "x" << this->geometry.y << std::endl;
	std::cout << "\tSize : " << this->geometry.w << "x" << this->geometry.h << std::endl;
}

#endif

void Painting::exec(){
	if(!this->isEnabled()){
		if(this->isVerbose())
			SelLog->Log('D', "Painting '%s' from '%s' is disabled", this->getNameC(), this->getWhereC());
		return;
	}

	if(::debug && this->isVerbose())
		SelLog->Log('D', "Painting::exec()");

	if(this->parentR){
		if(::debug && this->isVerbose())
			SelLog->Log('D', "[Painting \"%s\"] ParentR's type : '%s'", this->name.c_str(), this->parentR->getSurface()->cb->LuaObjectName());
		if(!this->geometry.w || !this->geometry.h){	// size not set
			uint32_t w,h;
			if(!this->parentR->getSurface()->cb->getSize(this->parentR->getSurface(), &w,&h)){
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

		if(!(this->surface = this->parentR->getSurface()->cb->subSurface(this->parentR->getSurface(), this->geometry.x, this->geometry.y, this->geometry.w, this->geometry.h, this->parentR->getSurface()->cb->getPrimary(this->parentR->getSurface())))){
			SelLog->Log('F', "[Painting \"%s\"] Can't create subsurface", this->name.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(this->parentP){
	} else {
		SelLog->Log('F', "[Painting \"%s\"] No parent defined", this->name.c_str());
		exit(EXIT_FAILURE);
	}

	if(::debug && this->isVerbose())
		SelLog->Log('D', "Painting::exec() - End");
}

void Painting::refresh(){
	if(!this->isEnabled()){
		if(this->isVerbose())
			SelLog->Log('D', "Painting '%s' from '%s' is disabled", this->getNameC(), this->getWhereC());
		return;
	}

	this->getSurface()->cb->Clear(this->getSurface());
	for(auto &d: this->DecorationsList)
		d->exec(*this);
}

void Painting::refreshChild(){
#if 0	/* TODO */
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

	this->refresh();
	this->refreshChild();
}
