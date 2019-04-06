/* LuaExec
 *
 * Execute a Lua function
 *
 * 06/04/2019 - LF - First version
 */

#ifndef LUAEXEC_H
#define LUAEXEC_H

#include <libSelene.h>

class LuaExec {
	struct elastic_storage func;	// Function to execute

public:
	LuaExec();

	struct elastic_storage *getFunc( void ){ return &(this->func); }
};

#endif
