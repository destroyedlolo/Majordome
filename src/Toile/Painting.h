/*	Painting Incarnation
 * 	Surface to paint on.
 *
 * 	01/01/2025 - First version
 */

#ifndef PAINTING_H
#define PAINTING_H

class Painting;	// Avoid nested includes

#include "../Config.h"
#include "../Helpers.h"
#include "../Object.h"
#include "Renderer.h"

#include <Selene/SelGenericSurface.h>

class Painting : public Object {
	struct SelGenericSurface *surface;

	Renderer *parentR;	// The parent is a renderer
	Painting *parentP;	// The parent is a painting

public:
	/* constructor from file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 * -> L : Lua's state
	 */
	Painting( const std::string &file, std::string &where, std::string &name, lua_State *L );

#ifdef DEBUG
	void dump();
#endif

	/* Try to initialize missing fields from its parent
	 * Fails in case of error.
	 */
	void initFromParent();
};
#endif
