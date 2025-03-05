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

#include "ObjCollection.h"
#include "LuaTask.h"
#include "Event.h"
#include "MQTTTopic.h"
#include "Timer.h"
#include "MinMax.h"
#include "NamedMinMax.h"
#include "Tracker.h"
#include "Shutdown.h"

#ifdef PGSQL
#	include "pgsql/pgSQL.h"
#	include "pgsql/Feed.h"
#	include "pgsql/NamedFeed.h"
#endif

#ifdef TOILE
#	include "Toile/Renderer.h"
#	include "Toile/Painting.h"
#	include "Toile/Decoration.h"
#	include "Toile/Field.h"
#endif

#include <lua.hpp>	/* Lua's state needed */
#include <string>

class Config : public SortDir { 
	std::string configDir;

protected :
	virtual bool accept( const char *, std::string & );

public:
	
	/* Initialise this configuration against 'where' directory's content */
	void init(std::string &where, lua_State *L);

	/* Check if data overlaps
	 *	Notez-bien : in case of clash, the program exits
	 */
	void SanityChecks( void );

		/* Accessors */
	std::string getConfigDir(){ return this->configDir; };

		/* Objects collections */
	TaskCollection TasksList;
	EventCollection EventsList;
	TopicCollection TopicsList;
	TimerCollection TimersList;
	MinMaxCollection MinMaxList;
	NamedMinMaxCollection NamedMinMaxList;
	TrackerCollection TrackersList;
	ShutdownCollection ShutdownsList;

#ifdef DBASE
#	ifdef PGSQL
	pgSQLCollection pgSQLsList;
#	endif

	FeedCollection FeedsList;
	NamedFeedCollection NamedFeedsList;
#endif

#ifdef TOILE
	RendererCollection RendererList;
	PaintingCollection PaintingList;
	DecorationCollection DecorationList;
#endif

		/* Topics' */
	void SubscribeTopics( void );	// Subscribe to defined MQTT topics

		/* Timers' */
	void LaunchTimers( void );	// Launch slaves' threads
	void RunImmediates( void );	// Triggers immediate & over timers

		/* Execution */
	void RunStartups( void );	// Executes RunAtStartup marked tasks
	void RunShutdowns( void );
};

extern Config config;
#endif
