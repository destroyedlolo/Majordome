/* Tracker object
 * 
 * 16/03/2019
 */

#ifndef TRACKER_H
#define TRACKER_H

#include <libSelene.h>

#include "MayBeEmptyString.h"
#include "Object.h"

class Config;

class Tracker : public Object {
	struct elastic_storage func;	// Function to execute

	std::string startTimerName;
	std::string stopTimerName;
	enum _status {
		WAITING,	// Waiting for start time
		CHECKING,	// checking periode (b/w start and stop time)
		DONE,		// the condition has been met during checking periode
	} status;
	bool active;	//	Is the tracker handling incoming messages

public:
	/* Constructor from a file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 * -> L : Lua's state
	 */
	Tracker( Config &cfg, const std::string &file, std::string &where, std::string &name, lua_State *L );

	struct elastic_storage *getFunc( void ){ return &(this->func); }

	/* Launch this tasks if possible
	 * -> name : name of the topic/timer that triggers this task
	 * -> topic : the topic itself
	 * <- true if it has been launched, false otherwise
	 */
	bool exec( const char *name, const char *topic=NULL, const char *payload=NULL );

		/* Create Lua's object */
	static int initLuaObject( lua_State *L );
};

#endif
