/* Task object
 *
 * 03/08/2018 - LF - First version
 */

#ifndef TASK_H
#define TASK_H

#include <libSelene.h>

#include "MayBeEmptyString.h"
#include "Object.h"
#include "LuaExec.h"

class Config;

class LuaTask : public Object, public LuaExec {
	bool once;	// can run only once

	pthread_mutex_t running_access;	// we want an access to "running"
	bool running;	// Is the task running

public:
	/* Constructor from a file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 * -> L : Lua's state
	 */
	LuaTask( Config &cfg, const std::string &file, std::string &where, std::string &name, lua_State *L );

	void setOnce( bool v ){ this->once = v; }
	bool getOnce( void ){ return this->once; }
	
	/* Launch this tasks if possible
	 * -> name : name of the topic/timer/tracker that triggers this task
	 * -> topic : the topic itself (or the tracker status if tracker's stoppingTasks)
	 * -> tracker : true if launched by a tracker
	 * <- true if it has been launched, false otherwise
	 */
	bool exec( const char *name, const char *topic=NULL, const char *payload=NULL, bool tracker=false );

	/* Check if this task can run */
	bool canRun( void );

	/* tell this task finished */
	void finished( void );

	/* Create Lua's object */
	static int initLuaObject( lua_State *L );
};
#endif
