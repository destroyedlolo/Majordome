/* Renderer object
 * 	Embodies a low-level physical device.
 *
 * 	07/12/2024 - First version
 */

#ifndef RENDERER_H
#define RENDERER_H

class Renderer;	// Avoid nested includes
class Config;

// #include "../Config.h"
#include "../Helpers.h"
#include "../Object.h"
#include "../LuaExec.h"

#include "Painting.h"

#include <Selene/SelGenericSurface.h>

#include <vector>

class Renderer : public LuaExec {
	struct SelGenericSurface *surface;
	bool fatal;

public:
	StringVector DecorationsList;			// List of decorations to apply
	std::vector<Painting *> PaintingList;	// List of painting

	/* constructor from file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 * -> L : Lua's state
	 */
	Renderer( const std::string &file, std::string &where, std::string &name, lua_State *L );

#ifdef DEBUG
	void dump();
#endif


	/* ***
	 * Accessors
	 * ***/

	bool getFatal(){ return this->fatal; }
	struct SelGenericSurface *getSurface(){ return this->surface; }


	/* ***
	 * Childs' management
	 * ***/

	void addDecoration( std::string t ){ this->DecorationsList.Add(t); }
	void addPainting( Painting *p ){ this->PaintingList.push_back(p); }


	/* ***
	 * Renderer own's 
	 */

	bool exec(); 		// Run the Lua code to build the renderer
	void refresh();		// Refresh renderer's own decoration
	void refreshAll();	// Refresh renderer and its childs
};

#endif
