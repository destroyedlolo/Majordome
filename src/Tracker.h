/* Tracker object
 * 
 * 16/03/2019
 */

#ifndef TRACKER_H
#define TRACKER_H

#include <libSelene.h>

#include "MayBeEmptyString.h"
#include "Event.h"
#include "LuaExec.h"

class Config;

class Tracker : public Event {	// Event contains tasks to launch when tracker changes to DONE

	StringVector stoppingTasks;	// Tasks to launch when stopping the tracker

	struct elastic_storage func;	// Function to execute

	enum _status {
		WAITING,	// Waiting for start time
		CHECKING,	// checking periode (b/w start and stop time)
		DONE,		// the condition has been met during checking periode
	} status;

public:
	/* Constructor from a file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 * -> L : Lua's state
	 */
	Tracker( Config &cfg, const std::string &file, std::string &where, std::string &name, lua_State *L );

	enum _status getStatus( void ){ return this->status; }

	struct elastic_storage *getFunc( void ){ return &(this->func); }

	/* Launch lua script if applicable
	 * -> name : name of the topic/timer that triggers this task
	 * -> topic : the topic itself
	 * <- true if it has been launched, false otherwise
	 */
	bool exec( const char *name, const char *topic=NULL, const char *payload=NULL );

	/* Change tracker status */
	void start( void );
	void stop( void );
	void done( void );

		/* Create Lua's object */
	static int initLuaObject( lua_State *L );
};

#endif
