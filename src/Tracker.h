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

class Tracker : public Event, public LuaExec {	// Event contains tasks to launch when tracker changes to DONE

	StringVector startingTasks;	// Tasks to launch when starting the tracker
	StringVector stoppingTasks;	// Tasks to launch when stopping the tracker

public:
	enum _status {
		WAITING,	// Waiting for start time
		CHECKING,	// checking periode (b/w start and stop time)
		DONE,		// the condition has been met during checking periode
	};

private:
	enum _status status;

public:
	/* Constructor from a file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 * -> L : Lua's state
	 */
	Tracker( Config &cfg, const std::string &file, std::string &where, std::string &name, lua_State *L );

	enum _status getStatus( void ){ return this->status; }
	const char *getStatusC( void );

	/* Overloading of LuaExec's in order to initialise Myself object */
	virtual void feedState( lua_State *L, const char *name, const char *topic=NULL, const char *payload=NULL, bool tracker=false, const char *trkstatus=NULL );

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

	void addDone( std::string t ){ this->Add(t); }
	void addStarted( std::string t ){ this->startingTasks.Add(t); }
	void addStopped( std::string t ){ this->stoppingTasks.Add(t); }

		/* Create Lua's object */
	static int initLuaObject( lua_State *L );
};

#endif
