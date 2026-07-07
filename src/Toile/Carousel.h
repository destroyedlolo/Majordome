/*	Carousel Incarnation
 * 	Cycle through a list of paintings
 *
 * 	08/07/2026 - First version
 */
#ifndef CAROUSEL_H
#define CAROUSEL_H

#include "Toile.h"
#include "ToileObject.h"
#include "ToileContainer.h"

#include "../Helpers.h"
#include "../Object.h"
#include "../ObjCollection.h"

class Carousel : virtual public ToileContainer, virtual public ToileObject {
protected:
	Toile::SurfaceGeometry geometry;

public:
//	Carousel():surface(NULL){};
	Carousel(const std::string &fch, std::string &where): Object(fch, where){};

	/* constructor from file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 * -> L : Lua's state
	 */
	Carousel( const std::string &file, std::string &where, lua_State *L );

protected:
	/* Read directives.
	 * These directives may apply to all derivates.
	 * Facing unknown directive, LuaExec's is called as well.
	 *
	 * -> l : directive line to parse
	 * -> nameused : is the name already used ?
	 */
	bool readConfigDirective( std::string &l );	// if unrecognized, call only
												// ToileObject's and Object's


	virtual std::string getTri(){ return "CAR_"; }
};

typedef ObjCollection<Carousel *> CarouselCollection;

#endif
