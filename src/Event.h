/* Event
 * Master class to object that has action attached
 */
#ifndef EVENT_H
#define EVENT_H

#include <vector>

#include "Object.h"
#include "StringVector.h"
#include "LuaTask.h"

class Config;

class Event : public Object, StringVector {

	typedef std::vector<std::string> TrackerEntries;	// List of tasks'name to launch when a message arrives
	TrackerEntries trackers;

protected:
	/* Default empty constructor to be only used by derived classes 
	 */
	Event(){}

public:
	/* Constructor from a file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 */
	Event( const std::string &file, std::string &where, std::string &name  );


	void addTask( std::string t ){ this->list.push_back(t); }
	void addTracker( std::string t ){ this->trackers.push_back(t); }

	/* launch tasks associated to this event (topic)
	 * -> name of the object that triggers the task
	 * -> topic : the one that triggers the task
	 */
	void execTasks( Config &, const char *name, const char *topic, const char *payload );
	void execTrackers( Config &, const char *name, const char *topic, const char *payload );

	/* launch tasks associated to this event (timer)
	 * -> name of the object that triggers the task
	 */
	void execTasks( Config &, const char *name );
	void execTrackers( Config &, const char *name );

	/* Create Lua's object */
	static int initLuaObject( lua_State *L );
};

#endif
