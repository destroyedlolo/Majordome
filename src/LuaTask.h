/* Task object
 *
 * 03/08/2018 - LF - First version
 */

#ifndef TASK_H
#define TASK_H

#include "LuaExec.h"
#include "Handler.h"
#include "ObjCollection.h"

class LuaTask : public Handler {
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
	LuaTask( const std::string &file, std::string &where, lua_State *L );

	virtual void readConfigDirective( std::string &l );

	void setOnce( bool v ){ this->once = v; }
	bool getOnce( void ){ return this->once; }
	
	void setRunAtStartup( bool v ){ this->runatstartup = v; }
	bool getRunAtStartup( void ){ return this->runatstartup; }

	/* Create Lua's object */
	static void initLuaInterface( lua_State *L );

	/* Execution */
	virtual bool canRun( void );	// Check if this task can run
	void finished( void );			// tell this task finished

	virtual void feedState(lua_State *L);

	virtual std::string getTri(){ return LuaTask::trigramme(); }
	static std::string trigramme(){ return "TSK_"; }
};

typedef ObjCollection<LuaTask *> TaskCollection;
typedef ObjVector<LuaTask *> TaskVector;

#endif
