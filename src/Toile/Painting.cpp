/* Painting object
 * 	Embodies a low-level rendering device.
 */

#include "Painting.h"

Painting::Painting( const std::string &fch, std::string &where, std::string &name, lua_State *L ): surface(NULL){
	if(verbose)
		SelLog->Log('L', "\t'%s'", fch.c_str());

	this->extrName( fch, name );
	this->name = name;
	this->where = where;

}
