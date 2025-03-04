/* Renderer object
 * 	Embodies a low-level physical device.
 *
 * 	07/12/2024 - First version
 */

#ifndef RENDERER_H
#define RENDERER_H

#include "../Helpers.h"
#include "../Object.h"
#include "../ObjCollection.h"
#include "../LuaExec.h"

class Config;

class Renderer;	// Avoid nested includes
#include "Painting.h"

#include <Selene/SelGenericSurface.h>

#include <vector>

class Renderer : public LuaExec {
	struct SelGenericSurface *surface;
	bool fatal;

public:
#if 0	/* TODO Decoration */
	StringVector DecorationsList;			// List of decorations to apply
#endif
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

	/* Read directives.
	 * These directives may apply to all derivates.
	 * Facing unknown directive, LuaExec's is called as well.
	 *
	 * -> l : directive line to parse
	 * -> nameused : is the name already used ?
	 */
	void readConfigDirective( std::string &l, std::string &name, bool &nameused );

	/* ***
	 * Accessors
	 * ***/

	bool getFatal(){ return this->fatal; }
	struct SelGenericSurface *getSurface(){ return this->surface; }


	/* ***
	 * Childs' management
	 * ***/

#if 0	/* TODO Decoration */
	void addDecoration( std::string t ){ this->DecorationsList.Add(t); }
#endif
	void addPainting( Painting *p ){ this->PaintingList.push_back(p); }


	/* ***
	 * Renderer own's 
	 */

	bool exec(); 		// Run the Lua code to build the renderer
	void refresh();		// Refresh renderer's own decoration
	void refreshAll();	// Refresh renderer and its childs
};

typedef ObjCollection<Renderer *> RendererCollection;
#endif
