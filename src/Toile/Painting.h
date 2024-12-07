/* Painting object
 * 	Embodies a low-level rendering device.
 *
 * 	07/12/2024 - First version
 */

#ifndef PAINTING_H
#define PAINTING_H

#include "../Config.h"
#include "../Helpers.h"
#include "../Object.h"
#include "../LuaExec.h"

#include <Selene/SelGenericSurface.h>

class Painting : virtual public Object, public LuaExec {
	struct SelGenericSurface *surface;

public:
	/* constructor from file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 * -> L : Lua's state
	 */
	Painting( const std::string &file, std::string &where, std::string &name, lua_State *L );

	/* Run the Lua code to build the renderer */
	bool exec();
};

#endif
