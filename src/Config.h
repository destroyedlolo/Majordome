/* Application configuration handling
 *
 * 26/07/2018 - LF - First version
 * 27/07/2018 - LF - handle all Configuration aspects
 * 20/05/2024 - LF - Migrate to v4
 * 20/01/2025 - LF - Migrate to v6
 */
#ifndef CONFIG_H
#define CONFIG_H

#include "SortDir.h"

#include "LuaTask.h"
#include "Event.h"

#include <lua.hpp>	/* Lua's state needed */
#include <string>
#include <unordered_map>

class Config : public SortDir { 
	std::string configDir;

protected :
	virtual bool accept( const char *, std::string & );

public:
	template <class T> class ObjCollection : public std::unordered_map<std::string, T> {};

	/* Initialise this configuration against 'where' directory's content */
	void init(std::string &where, lua_State *L);

	/* Check if data overlaps
	 *	Notez-bien : in case of clash, the program exits
	 */
	void SanityChecks( void );

		/* Accessors */
	std::string getConfigDir(){ return this->configDir; };

		/* Objects collections */
	typedef ObjCollection<LuaTask *> TaskCollection;
	TaskCollection TasksList;

	typedef ObjCollection<Event *> EventCollection;
	EventCollection EventsList;

		/* Execution */
	void RunStartups( void );	// Executes RunAtStartup marked tasks
	void RunShutdowns( void );
};

extern Config config;
#endif
