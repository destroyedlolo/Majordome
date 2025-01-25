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
		 */
	virtual bool exec(void);	// Run asynchronously, fresh state
	virtual bool exec(lua_State *L, enum boolRetCode *rc = NULL, std::string *rs = NULL, lua_Number *retn = NULL);	// Run synchronously

	virtual void feedState(lua_State *L) = 0;	// Feed Lua's state with objects owns
};

#endif
