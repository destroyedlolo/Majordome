/* Task object
 *
 * 03/08/2018 - LF - First version
 */

#ifndef TASK_H
#define TASK_H

#include <libSelene.h>

#include "MayBeEmptyString.h"
#include "Object.h"

class LuaTask : public Object {
	struct elastic_storage func;	// Function to execute

public:
	/* Constructor from a file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 * -> L : Lua's state
	 */
	LuaTask( const std::string &file, std::string &where, std::string &name, lua_State *L );

};

#endif
