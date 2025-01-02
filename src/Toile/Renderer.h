/* Renderer object
 * 	Embodies a low-level physical device.
 *
 * 	07/12/2024 - First version
 */

#ifndef RENDERER_H
#define RENDERER_H

class Renderer;	// Avoid nested includes

#include "../Config.h"
#include "../Helpers.h"
#include "../Object.h"
#include "../LuaExec.h"

#include "Painting.h"

#include <Selene/SelGenericSurface.h>

#include <vector>

class Renderer : public LuaExec {
	struct SelGenericSurface *surface;
	bool fatal;

	std::vector< std::reference_wrapper<Painting *> > PaintingList;

public:
	StringVector DecorationsList;	// List of decorations to apply

	/* constructor from file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 * -> L : Lua's state
	 */
	Renderer( const std::string &file, std::string &where, std::string &name, lua_State *L );

	/* Accessor */
	bool getFatal(){ return this->fatal; }

	/* Run the Lua code to build the renderer */
	bool exec();

	/* Accessors */
	struct SelGenericSurface *getSurface(){ return this->surface; }

	void addDecoration( std::string t ){ this->DecorationsList.Add(t); }
	void addPainting( Painting *p ){ this->PaintingList.push_back(p); }
};

#endif
