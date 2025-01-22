/* Handler.h
 * Abstract all handlers interfaces
 */
#ifndef HANDLER_H
#define HANDLER_H

#include "LuaExec.h"

class Handler : virtual public LuaExec {
protected :
	Handler(const std::string &fch, std::string &where, std::string &name) : Object(fch, where, name), LuaExec(fch, where, name) {};

public :

		/* Execute Lua code while an event rise
		 * Notez-Bien : Code is always run ASYNCHRONOUSLY and Lua's State
		 * will be cleaned by LuaExec when the execution is over.
		 */
	virtual bool exec(void);	// Create the Lua's State, feed with basic object and then call exec(lua_State *L)
	virtual bool exec(lua_State *L);

	virtual void feedState(lua_State *L) = 0;	// Feed Lua's state with objects owns
};

#endif
