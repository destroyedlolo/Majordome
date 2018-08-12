/* Task object
 *
 * 03/08/2018 - LF - First version
 */

#ifndef TASK_H
#define TASK_H

#include <libSelene.h>

#include "MayBeEmptyString.h"
#include "Object.h"

class Config;

class LuaTask : public Object {
	struct elastic_storage func;	// Function to execute
	bool once;	// can run only once

public:
	/* Constructor from a file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 * -> L : Lua's state
	 */
	LuaTask( Config &cfg, const std::string &file, std::string &where, std::string &name, lua_State *L );

	void setOnce( bool v ){ this->once = v; }
	bool getOnce( void ){ return this->once; }

	/* Launch this tasks if possible
	 * -> name : topic that triggers this task
	 * <- true if it has been launched, false otherwise
	 */
	bool exec( const char *name );
};

#endif
