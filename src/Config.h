/* Configuration handling
 *
 * 26/07/2018 - LF - First version
 * 27/07/2018 - LF - handle all Configuration aspects
 */
#ifndef CONFIG_H
#define CONFIG_H

#include <unordered_map>

#include <lua.hpp>	/* Lua's state needed */

#include "SortDir.h"
#include "MQTTTopic.h"
#include "Timer.h"

#include "LuaTask.h"

class Config : virtual public SortDir {
protected :
	virtual bool accept( const char *, std::string & );

public:
	typedef std::unordered_map<std::string, MQTTTopic> TopicElements;
	TopicElements TopicsList;

	typedef std::unordered_map<std::string, Timer> TimerElements;
	TimerElements Timerslist;

	typedef std::unordered_map<std::string, LuaTask> TaskElements;
	TaskElements TasksList;

	/* Initialise this configuration against 'where' directory's content */
	void init(const char *where, lua_State *L);

	/* Check if data overlaps
	 *	Notez-bien : in case of clash, the program exits
	 */
	void SanityChecks( void );

	/* Subscribe to defined MQTT topics */
	void SubscribeTopics( void );

	/* Launch timer slave threads */
	void LaunchTimers( void );

	/* Triggers immediate & over timers */
	void RunImmediates( void );

	/* Find a task by its name 
	 *	throw an exception if not found
	 */
	LuaTask &findTask( std::string & );
};

extern Config config;
#endif
