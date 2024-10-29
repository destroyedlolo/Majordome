/* LuaExec
 *
 * Execute a Lua function
 *
 * 06/04/2019 - LF - First version
 */

#ifndef LUAEXEC_H
#define LUAEXEC_H

#include <sstream>	// stringstream

#include "Selene.h"

class LuaExec : virtual public Object {
	struct elastic_storage func;	// Function to execute

public:
	enum boolRetCode { RCnil=-1, RCfalse=false, RCtrue=true };

	StringVector needed_rendezvous;
	StringVector needed_tracker;
	StringVector needed_timer;
	StringVector needed_topic;
	StringVector required_topic;
	StringVector needed_task;
	StringVector needed_minmax;
	StringVector needed_namedminmax;

protected:
	void addNeededRendezVous( std::string t ){ this->needed_rendezvous.Add(t); }
	void addNeededTracker( std::string t ){ this->needed_tracker.Add(t); }
	void addNeededTimer( std::string t ){ this->needed_timer.Add(t); }
	void addNeededTopic( std::string t ){ this->needed_topic.Add(t); }
	void addRequiredTopic( std::string t ){ this->required_topic.Add(t); }
	void addNeededTask( std::string t ){ this->needed_task.Add(t); }
	void addNeededMinMax( std::string t ){ this->needed_minmax.Add(t); }
	void addNeededNamedMinMax( std::string t ){ this->needed_namedminmax.Add(t); }

	bool readConfigDirective( std::string &l );

public:
	LuaExec();
	struct elastic_storage *getFunc( void ){ return &(this->func); }

	bool feedbyNeeded( lua_State * );

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
	 * -> trkstatus : status of the tracker
	 *
	 *  May be overloaded if some local objects have to be created
	 */
	virtual void feedState( lua_State *L, const char *name, const char *topic=NULL, const char *payload=NULL, bool tracker=false, const char *trkstatus=NULL );

	/* Launch a task in a slave thread
	 * -> same as feedState()
	 * <- true if it has been launched, false otherwise
	 */
	bool execAsync( const char *name, const char *topic=NULL, const char *payload=NULL, bool tracker=false, const char *trkstatus=NULL );

	/* Execute a task in the same thread
	 * -> same as feedState()
	 * -> return code from the function (NULL = Ignored)
	 * -> return string if returned (NULL = Ignored)
	 * -> return number if returned (NULL = Ignored) :  must have a string before
	 * <- true if it has been executed, false otherwise
	 */
	bool execSync( const char *name, const char *topic=NULL, const char *payload=NULL, bool tracker=false, enum boolRetCode *rc=NULL, std::string *rs=NULL, lua_Number *n=NULL );

	/* Tell when a task is finished
	 * (mostly for tasks with "once" parameter)
	 */
	virtual void finished( void ){}
};

#endif
