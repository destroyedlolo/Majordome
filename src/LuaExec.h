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

class LuaExec : virtual public Object {
	struct elastic_storage func;	// Function to execute

public:
	LuaExec();
	struct elastic_storage *getFunc( void ){ return &(this->func); }

	/* Store Lua's code
	 *
	 * -> L : Lua's state
	 * -> buffer : function source
	 * -> name : function's name
	 * <- does the loading succeed
	 */
	bool LoadFunc( lua_State *L, std::stringstream &buffer, const char *name );

	/* Initialise the Lua state
	 * -> name : name of the topic/timer/tracker that triggers this task
	 * -> topic : the topic itself (or the tracker status if tracker's stoppingTasks)
	 * -> tracker : true if launched by a tracker
	 */
	void feedState( lua_State *L, const char *name, const char *topic=NULL, const char *payload=NULL, bool tracker=false );

	/* Launch a task in a slave thread
	 * -> same as feedState()
	 * <- true if it has been launched, false otherwise
	 */
	bool execAsync( const char *name, const char *topic=NULL, const char *payload=NULL, bool tracker=false );

	/* Execute a task in the same thread
	 * -> same as feedState()
	 * <- true if it has been executed, false otherwise
	 */
	bool execSync( const char *name, const char *topic=NULL, const char *payload=NULL, bool tracker=false );

	virtual void finished( void ){}
};

#endif
