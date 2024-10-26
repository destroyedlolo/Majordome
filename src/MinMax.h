/* MinMax object
 *
 * 26/10/2024 Creation
 */

#ifndef MINMAX_H
#define MINMAX_H

#include "Event.h"
#include "LuaExec.h"

class MinMax : public Event, public LuaExec {
	bool empty;	// No value yet
	lua_Number min,max;

public:
	/* Constructor from a file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 * -> L : Lua's state
	 */
	MinMax( const std::string &file, std::string &where, std::string &name, lua_State *L );
};

#endif
