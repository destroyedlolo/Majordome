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
class Painting;
class Decoration;
/*
class Renderer;	// Avoid nested includes
#include "Painting.h"
#include "Decoration.h"
*/

#include <Selene/SelGenericSurface.h>

#include <vector>

class Renderer : virtual public LuaExec {
	bool fatal;

protected:
	struct SelGenericSurface *surface;

	/* Read directives.
	 * These directives may apply to all derivates.
	 * Facing unknown directive, LuaExec's is called as well.
	 *
	 * -> l : directive line to parse
	 */
	void readConfigDirective( std::string &l );

public:
	std::vector<Decoration *> DecorationsList;	// List of decorations to apply
	std::vector<Painting *> PaintingList;		// List of painting

	/* constructor from file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 * -> L : Lua's state
	 */
	Renderer( const std::string &file, std::string &where, lua_State *L );
	Renderer() : fatal(false), surface(NULL){};
	
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

	void addDecoration( Decoration *t ){ this->DecorationsList.push_back(t); }
	void addPainting( Painting *p ){ this->PaintingList.push_back(p); }


	/* ***
	 * Renderer own's 
	 * ***/

	virtual bool exec(); 		// Run the Lua code to build the renderer
	void refresh();			// Refresh Painting's own decoration (background)
	void refreshChild();	// Refresh its child
	void refreshAll();		// Refresh everything

	virtual std::string getTri(){ return "RDR_"; }
};

typedef ObjCollection<Renderer *> RendererCollection;
#endif
