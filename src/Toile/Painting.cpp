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

Painting::Painting( const std::string &fch, std::string &where, std::string &name, lua_State *L ): Object(fch, where, name), surface(NULL), parentR(NULL), parentP(NULL){
	if(verbose){
		SelLog->Log('L', "\t'%s'", fch.c_str());
#if DEBUG
		SelLog->Log('D', "\t\tid : (%p)", this);
#endif
	}

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

			this->readConfigDirective(l, nameused);
		} while(true);

		file.close();
	} catch(const std::ifstream::failure &e){
		if(!file.eof()){
			SelLog->Log('F', "%s : %s", fch.c_str(), strerror(errno) );
			exit(EXIT_FAILURE);
		}
	}


		/* ***
		 * Sanity checks
		 * ***/
	if(!this->parentR && !this->parentP){
		SelLog->Log('F', "[Painting \"%s\"] No parent defined", this->name.c_str());
		exit(EXIT_FAILURE);
	}
}

void Painting::readConfigDirective( std::string &l, bool &nameused ){
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
#ifdef DEBUG
				SelLog->Log('C', "\t\tThe Parent is Renderer '%s' (%p)", arg.c_str(), &(renderer->second));
#else
				SelLog->Log('C', "\t\tThe Parent is Renderer '%s'", arg.c_str());
#endif
			this->parentR = renderer->second;
			renderer->second->addPainting( this );
		} else {
			SelLog->Log('F', "\t\tRenderer '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(!!(arg = striKWcmp( l, "-->> Origin=" ))){
		int r = sscanf(arg.c_str(), "%u,%u", &(this->geometry.x), &(this->geometry.y));
		if(r != 2)
			SelLog->Log('W', "Wasn't able to read Origine='s arguments");

		if(verbose)
			SelLog->Log('C', "\t\tPainting's origin : %u,%u", this->geometry.x,this->geometry.y);
	} else if(!!(arg = striKWcmp( l, "-->> Size=" ))){
				int r = sscanf(arg.c_str(), "%ux%u", &(this->geometry.w), &(this->geometry.h));
		if(r != 2)
			SelLog->Log('W', "Wasn't able to read Size='s arguments");

		if(verbose)
			SelLog->Log('C', "\t\tPainting's Size : %ux%u", this->geometry.w,this->geometry.h);
	} else if( Object::readConfigDirective(l, nameused) )
		// Don't use LuaExec's as "need_??" is not used
		nameused = true;
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
	if(debug)
		SelLog->Log('D', "Painting::exec()");

	if(this->parentR){
		if(debug)
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

	if(debug)
		SelLog->Log('D', "Painting::exec() - End");
}

void Painting::refresh(){
	for(auto &dn: this->DecorationsList){
		auto d = config.findDecoration(dn);
		d->exec(*this);
	}
}

void Painting::refreshAll(){
	this->refresh();

#if 0
		// refresh childs
	for(auto &paint: this->PaintingList)
		paint->refreshAll();
#endif
}
