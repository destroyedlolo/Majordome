/* Event
 * Master class to objects having actions attached.
 * Used also to impersonate Rendez-Vous.
 */
#ifndef EVENT_H
#define EVENT_H

#include <vector>

#include "Object.h"
#include "StringVector.h"
#include "LuaTask.h"

class Config;

class Event : virtual public Object, public StringVector {
	// The StringVector contains the lists of LuaTasks to launch

protected:
	/* Default empty constructor to be only used by derived classes 
	 */
Event() = default;

public:
	/* Constructor from a file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 */
	Event( const std::string &file, std::string &where, std::string &name  );


	void addTask( std::string t ){ this->Add(t); }

	/* launch tasks associated to this event (topic or tracker)
	 * -> name of the object that triggers the task
	 * -> topic : the one that triggers the task
	 * -> payload
	 * -> tracker : true if it's a tracker
	 * -> trkstatus : status of the tracker
	 */
	void execTasks( Config &, const char *name, const char *topic, const char *payload, bool tracker=false, const char *trkstatus=NULL );

	/* launch tasks associated to this event (timer)
	 * -> name of the object that triggers the task
	 */
	void execTasks( Config &, const char *name );

private:
	StringVector trackersToEnable;	// Trackers to be enable by this event
	StringVector trackersToDisable;	// Trackers to be disabled by this event

public:
	/* Add a tracker to Enable or Disable list */
	void addTrackerEN( std::string t ){ this->trackersToEnable.Add(t); }
	void addTrackerDIS( std::string t ){ this->trackersToDisable.Add(t); }

	/* Enable/Disable trackers from lists */
	void enableTrackers( void );
	void disableTrackers( void );

	/* Create Lua's object */
	static int initLuaObject( lua_State *L );
};

#endif
