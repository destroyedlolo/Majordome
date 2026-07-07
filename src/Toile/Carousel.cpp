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


