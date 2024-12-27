/* Decoration object
 * 	Task to be called when an object needs to be refreshed
 *
 * 	19/12/2024 - First version
 */

#ifndef DECORATION_H
#define DECORATION_H

class Decoration;	// Avoid nested includes

#include "../Config.h"
#include "../Helpers.h"
#include "../Object.h"
#include "../LuaExec.h"

#include <Selene/SelGenericSurface.h>

class Decoration : public LuaExec {
public:
	/* constructor from file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 * -> L : Lua's state
	 */
	Decoration( const std::string &file, std::string &where, std::string &name, lua_State *L );
	
	/* Run the Lua code to build the renderer */
	void exec(Renderer &);
};

#endif
