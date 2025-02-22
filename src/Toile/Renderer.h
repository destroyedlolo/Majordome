/* Renderer object
 * 	Embodies a low-level physical device.
 *
 * 	07/12/2024 - First version
 */

#ifndef RENDERER_H
#define RENDERER_H

#include "../Helpers.h"
#include "../Object.h"
#include "../LuaExec.h"

class Config;

#include <Selene/SelGenericSurface.h>

#include <vector>

class Renderer : public LuaExec {
	struct SelGenericSurface *surface;
	bool fatal;

public:

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

	void readConfigDirective( std::string &l, std::string &name, bool &nameused );

	/* ***
	 * Accessors
	 * ***/

	bool getFatal(){ return this->fatal; }
	struct SelGenericSurface *getSurface(){ return this->surface; }


	/* ***
	 * Childs' management
	 * ***/

	/* TODO */

	/* ***
	 * Renderer own's 
	 */

	bool exec(); 		// Run the Lua code to build the renderer
	void refresh();		// Refresh renderer's own decoration
	void refreshAll();	// Refresh renderer and its childs
};

#endif
