/*	Painting Incarnation
 * 	Surface to paint on.
 *
 * 	01/01/2025 - First version
 */

#ifndef PAINTING_H
#define PAINTING_H

class Painting;	// Avoid nested includes

#include "Toile.h"
#include "Renderer.h"

#include "../Helpers.h"
#include "../Object.h"

#include <Selene/SelGenericSurface.h>

class Painting : public Object {
	struct SelGenericSurface *surface;

	Renderer *parentR;	// The parent is a renderer
	Painting *parentP;	// The parent is a painting

	Toile::SurfaceGeometry geometry;

public:
	StringVector DecorationsList;

	Painting():surface(NULL), parentR(NULL), parentP(NULL){};
	Painting(const std::string &fch, std::string &where, std::string &name): Object(fch, where, name), surface(NULL), parentR(NULL), parentP(NULL){};

	/* constructor from file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 * -> L : Lua's state
	 */
	Painting( const std::string &file, std::string &where, std::string &name, lua_State *L );

	/* Read directives.
	 * These directives may apply to all derivates.
	 * Facing unknown directive, LuaExec's is called as well.
	 *
	 * -> l : directive line to parse
	 * -> nameused : is the name already used ?
	 * <- true if a directive requiring the name has been understood
	 */
	void readConfigDirective( std::string &l, bool &nameused );

#ifdef DEBUG
	void dump();
#endif

	/* ***
	 * Accessors
	 * ***/

	struct SelGenericSurface *getSurface(){ return this->surface; }


	/* ***
	 * Childs' management
	 * ***/

	void addDecoration( std::string t ){ this->DecorationsList.Add(t); }


	/* ***
	 * Renderer own's 
	 */

		// Try to initialize missing fields from its parent
	 	// Then create the subsurface.
	 	// Fails in case of error.
	void exec();
	void refresh();		// Refresh Painting's own decoration
	void refreshAll();	// Refresh Painting and its childs
};
#endif
