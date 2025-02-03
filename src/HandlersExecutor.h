/* Handler.h
 * Abstract objects that can execute handlers
 */
#ifndef HANDLEREXECUTOR_H
#define HANDLEREXECUTOR_H

#include "Object.h"
#include "Handler.h"
#include "Helpers.h"

class HandlersExecutor : virtual public Object, public std::vector<Handler *> {
public :
	/* Handlers execution
	 * (runs sequentially)
	 */
	void execHandlers(void);		// fresh State but in the same thread

	virtual void feedHandlersStat(lua_State *L) {};	// Add custom object in Lua state
};

#endif
