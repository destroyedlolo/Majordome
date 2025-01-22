/* Task object
 *
 * 03/08/2018 - LF - First version
 */

#ifndef TASK_H
#define TASK_H

#include "LuaExec.h"
#include "ModuleInterface.h"

class Config;

class LuaTask : virtual public ModuleInterface, public LuaExec {
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

	virtual void readConfigDirective( std::string &l, bool &nameused );

	void setOnce( bool v ){ this->once = v; }
	bool getOnce( void ){ return this->once; }
	
	void setRunAtStartup( bool v ){ this->runatstartup = v; }
	bool getRunAtStartup( void ){ return this->runatstartup; }

	/* Create Lua's object */
	static void initLuaInterface( lua_State *L );

	/* Execution */
	virtual bool canRun( void );	// Check if this task can run
	void finished( void );			// tell this task finished

	void feedState(lua_State *L);

	/* Notez-bien : executing a task is ALWAYS asynchronous */
	virtual bool exec(void);
	virtual bool exec(lua_State *L);

	/* TODO */
};
#endif
