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

	bool prepareExecSync(lua_State *L);

public:
	enum boolRetCode { 
		RCnil=-1,		// no return
		RCfalse=false,	// bool false (rejected)
		RCtrue=true,	// bool true (accepted)
		RCforced=-2		// value provided
	};

		/* ***
		 * Lists of needed
		 * ***/

	StringVector needed_topic;
	StringVector required_topic;
	StringVector needed_task;
	StringVector needed_rendezvous;
	StringVector needed_timer;
	StringVector needed_minmax;
	StringVector needed_namedminmax;
	StringVector needed_shutdown;
	StringVector needed_tracker;
#ifdef TOILE
/* TODO Renderer
	StringVector needed_renderer;
*/
#endif
#ifdef DBASE
#	ifdef PGSQL
	StringVector needed_pgSQL;
#	endif
	StringVector needed_feed;
	StringVector needed_namedfeed;
#endif

protected:
	void addNeededTopic( std::string t ){ this->needed_topic.Add(t); }
	void addRequiredTopic( std::string t ){ this->required_topic.Add(t); }
	void addNeededTask( std::string t ){ this->needed_task.Add(t); }
	void addNeededRendezVous( std::string t ){ this->needed_rendezvous.Add(t); }
	void addNeededTimer( std::string t ){ this->needed_timer.Add(t); }
	void addNeededMinMax( std::string t ){ this->needed_minmax.Add(t); }
	void addNeededNamedMinMax( std::string t ){ this->needed_namedminmax.Add(t); }
	void addNeededShutdown( std::string t ){ this->needed_shutdown.Add(t); }
	void addNeededTracker( std::string t ){ this->needed_tracker.Add(t); }
#ifdef TOILE
/*	TODO Renderer
	void addNeededRenderer( std::string t ){ this->needed_renderer.Add(t); }
*/
#endif
#ifdef DBASE
#	ifdef PGSQL
	void addNeededpgSQL( std::string t ){ this->needed_pgSQL.Add(t); }
#	endif
	void addNeededFeed( std::string t ){ this->needed_feed.Add(t); }
	void addNeededNamedFeed( std::string t ){ this->needed_namedfeed.Add(t); }
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
	virtual bool execAsync(lua_State *L);

	/* Synchronously execute Lua code.
	 * Run with the same State as its caller.
	 * Globals objects are accessible and call code will create objects in
	 * the State.
	 *
	 * Notez-bien :
	 *	- it's up to the caller to clean Lua state
	 *	- in case of failure, return false, and the Lua state is already cleared
	 */

		// We are expecting only a YES/NO to accept a value
		// (default as well if we don't care about the result)
	bool execSync(lua_State *L, enum boolRetCode *rc=NULL);

		// boolean or value forced
	bool execSync(lua_State *L, enum boolRetCode *rc, lua_Number *retn);

		// name + boolean or value forced
	bool execSync(lua_State *L, std::string *rs, enum boolRetCode *rc, lua_Number *retn);	
};

#endif
