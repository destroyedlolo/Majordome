/* LuaExec
 *
 * Execute a Lua function
 *
 * 06/04/2019 - LF - First version
 */

#ifndef LUAEXEC_H
#define LUAEXEC_H

#include <sstream>	// stringstream

#include <libSelene.h>

class LuaExec {
	struct elastic_storage func;	// Function to execute

public:
	LuaExec();

	/* Store Lua's code
	 *
	 * -> L : Lua's state
	 * -> buffer : function source
	 * -> name : function's name
	 * <- does the loading succeed
	 */
	bool LoadFunc( lua_State *L, std::stringstream &buffer, const char *name );


	struct elastic_storage *getFunc( void ){ return &(this->func); }
};

#endif
