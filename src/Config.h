/* Configuration handling
 *
 * 26/07/2018 - LF - First version
 * 27/07/2018 - LF - handle all Configuration aspects
 * 20/05/2024 - LF - Migrate to v4
 */
#ifndef CONFIG_H
#define CONFIG_H

#include <unordered_map>
#include <string>

#include <lua.hpp>	/* Lua's state needed */

#include "SortDir.h"

#include "LuaTask.h"
#include "Event.h"

class Config : virtual public SortDir {
protected :
	virtual bool accept( const char *, std::string & );

public:
	typedef std::unordered_map<std::string, Event> EventElements;
	EventElements EventsList;

	typedef std::unordered_map<std::string, LuaTask> TaskElements;
	TaskElements TasksList;
	
	/* Initialise this configuration against 'where' directory's content */
	void init(std::string &where, lua_State *L);

	/* Find a task/tracker by its name 
	 *	throw an exception if not found
	 */
	LuaTask &findTask( std::string & );
//	Tracker &findTracker( std::string & );
};

extern Config config;
#endif
