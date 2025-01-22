/* LuaExec
 *
 * Execute a Lua function
 *
 * 06/04/2019 - LF - First version
 */

#ifndef LUAEXEC_H
#define LUAEXEC_H

#include "StringVector.h"
#include "Selene.h"
#include "Object.h"

#include <sstream>	// stringstream
#include <lua.hpp>	/* Lua's state needed */

class LuaExec : virtual public Object {
	struct elastic_storage func;	// Storage for the function to execute

public:
	enum boolRetCode { RCnil=-1, RCfalse=false, RCtrue=true };

		/* ***
		 * Lists of needed
		 * ***/

	StringVector needed_topic;
	StringVector needed_task;
#if 0	/* TODO */
	StringVector needed_rendezvous;
	StringVector needed_tracker;
	StringVector needed_timer;
	StringVector required_topic;
	StringVector needed_minmax;
	StringVector needed_namedminmax;
#ifdef TOILE
	StringVector needed_renderer;
#endif
#endif

protected:
	void addNeededTopic( std::string t ){ this->needed_topic.Add(t); }
	void addNeededTask( std::string t ){ this->needed_task.Add(t); }
#if 0	/* TODO */
	void addNeededRendezVous( std::string t ){ this->needed_rendezvous.Add(t); }
	void addNeededTracker( std::string t ){ this->needed_tracker.Add(t); }
	void addNeededTimer( std::string t ){ this->needed_timer.Add(t); }
	void addRequiredTopic( std::string t ){ this->required_topic.Add(t); }
	void addNeededMinMax( std::string t ){ this->needed_minmax.Add(t); }
	void addNeededNamedMinMax( std::string t ){ this->needed_namedminmax.Add(t); }

#ifdef TOILE
	void addNeededRenderer( std::string t ){ this->needed_renderer.Add(t); }
#endif
#endif

	virtual void readConfigDirective( std::string &l, std::string &name, bool &nameused );

public:
	LuaExec(const std::string &fch, std::string &where, std::string &name);
	struct elastic_storage *getFunc( void ){ return &(this->func); }

	lua_State *createLuaState(void);

	/* Store Lua's code
	 *
	 * -> L : Lua's state
	 * -> buffer : function source
	 * -> name : function's name
	 * <- does the loading succeed
	 */
	bool LoadFunc( lua_State *L, std::stringstream &buffer, const char *name );

	/* Tell when a task is finished
	 * (mostly for tasks with "once" parameter)
	 */
	virtual void finished( void ){}

	/* Determine if the code can run (true) or not (false)
	 * No cleaning is done.
	 */
	virtual bool canRun( void );

	/* Feed Lua State as per needs
	 * -> require : do we have to include required as well
	 */
	bool feedbyNeeded( lua_State *, bool require=true );

	/* Asynchronously execute Lua code.
	 * Run in a new thread with a dedicated State.
	 * Consequently, global objects are not accessible.
	 */
	bool execAsync(lua_State *L);

	/* Synchronously execute Lua code.
	 * Run with the same State as its caller.
	 * Globals objects are accessible and call code will create objects in
	 * the State.
	 */
	bool execSync(lua_State *L, enum boolRetCode *rc = NULL, std::string *rs = NULL, lua_Number *retn = NULL);	
};

#endif
