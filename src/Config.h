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
#include "Timer.h"
#include "MQTTTopic.h"
#include "Tracker.h"
#include "MinMax.h"
#include "NamedMinMax.h"
#include "Shutdown.h"
#ifdef TOILE
#	include "Toile/Renderer.h"
#endif

class Config : virtual public SortDir {
	std::string configDir;

protected :
	virtual bool accept( const char *, std::string & );

public:
		/* Objects collections */
	typedef std::unordered_map<std::string, Event> EventElements;
	EventElements EventsList;

	typedef std::unordered_map<std::string, LuaTask> TaskElements;
	TaskElements TasksList;
	
	typedef std::unordered_map<std::string, Timer> TimerElements;
	TimerElements TimersList;

	typedef std::unordered_map<std::string, MQTTTopic> TopicElements;
	TopicElements TopicsList;

	typedef std::unordered_map<std::string, Tracker> TrackerElements;
	TrackerElements TrackersList;

	typedef std::unordered_map<std::string, MinMax> MinMaxElements;
	MinMaxElements MinMaxList;

	typedef std::unordered_map<std::string, NamedMinMax> NamedMinMaxElements;
	NamedMinMaxElements NamedMinMaxList;

#ifdef TOILE
	typedef std::unordered_map<std::string, Renderer> RendererElements;
	RendererElements RendererList;
#endif

	/* Initialise this configuration against 'where' directory's content */
	void init(std::string &where, lua_State *L);

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

		/* Run startup functions */
	void RunStartups( void );

	/* Find a task/tracker by its name 
	 *	throw an exception if not found
	 */
	LuaTask &findTask( std::string & );
	Tracker &findTracker( std::string & );
	MinMax &findMinMax( std::string & );
	NamedMinMax &findNamedMinMax( std::string & );
#ifdef TOILE
	Renderer &findRenderer( std::string & );
#endif

	std::string getConfigDir(){ return this->configDir; }
};

extern Config config;
#endif
