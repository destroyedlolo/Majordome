/* Renderer object
 * 	Embodies a low-level physical device.
 *
 * 	07/12/2024 - First version
 */

#ifndef RENDERER_H
#define RENDERER_H

#include "ToileContainer.h"

#include "../Helpers.h"
#include "../Object.h"
#include "../ObjCollection.h"
#include "../LuaExec.h"

#include <vector>

class Renderer : virtual public ToileContainer {
	bool fatal;

protected:
	/* Read directives.
	 * These directives may apply to all derivates.
	 * Facing unknown directive, LuaExec's is called as well.
	 *
	 * -> l : directive line to parse
	 */
	bool readConfigDirective( std::string &l );

public:
	/* constructor from file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 * -> L : Lua's state
	 */
	Renderer( const std::string &file, std::string &where, lua_State *L );
	Renderer() : fatal(false){};
	
#ifdef DEBUG
	void dump();
#endif

	/* ***
	 * Accessors
	 * ***/

	bool getFatal(){ return this->fatal; }

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
