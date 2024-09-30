/* Task object
 *
 * 03/08/2018 - LF - First version
 */

#ifndef TASK_H
#define TASK_H

#include "Selene.h"
#include "MayBeEmptyString.h"
#include "StringVector.h"
#include "Object.h"
#include "LuaExec.h"

class Config;

class LuaTask : public LuaExec {
	bool once;	// can run only once

	pthread_mutex_t running_access;	// we want an access to "running"
	bool running;	// Is the task running

	bool runatstartup;	// Launch at startup

public:
	/* Constructor from a file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 * -> L : Lua's state
	 */
	LuaTask( const std::string &file, std::string &where, std::string &name, lua_State *L );

	void setOnce( bool v ){ this->once = v; }
	bool getOnce( void ){ return this->once; }
	
	void setRunAtStartup( bool v ){ this->runatstartup = v; }
	bool getRunAtStartup( void ){ return this->runatstartup; }

	/* Overloading of LuaExec's in order to initialise Myself object */
	virtual void feedState( lua_State *L, const char *name, const char *topic=NULL, const char *payload=NULL, bool tracker=false, const char *trkstatus=NULL );

	/* Launch this tasks if possible
	 * Same arguments as LuaExec::exec()
	 */
	bool exec( const char *name, const char *topic=NULL, const char *payload=NULL, bool tracker=false, const char *trkstatus=NULL );

	/* Check if this task can run */
	bool canRun( void );

	/* tell this task finished */
	virtual void finished( void );

	/* Create Lua's object */
	static void initLuaObject( lua_State *L );
};
#endif
