/* Handler.h
 * Abstract all handlers interfaces
 */
#ifndef HANDLER_H
#define HANDLER_H

#include "LuaExec.h"
#include "ObjCollection.h"

class HandlersExecutor;

class Handler : public LuaExec {
protected :
	Handler(const std::string &fch, std::string &where) : LuaExec(fch, where) {};

	virtual void readConfigDirective(std::string &l);
	virtual bool readConfigDirectiveNoData(std::string &l);	// Linked to events only
	virtual bool readConfigDirectiveData(std::string &l);	// Linked to events that are providing data
public :

		/* Execute Lua code while an event rise
		 */
	virtual bool exec(HandlersExecutor *);		// Run asynchronously, fresh state
	virtual lua_State *prepareExec(void);	// Create the State, feed it but not launch execution. To add custom stuffs in the State (see MQTTTopic)

	virtual bool exec(lua_State *L);	// Run synchronously

	virtual void feedState(lua_State *L) = 0;	// Feed Lua's state with objects owns
};

typedef ObjCollection<Handler *> HandlerCollection;
typedef ObjVector<Handler *> HandlerVector;
#endif
