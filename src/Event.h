/* Event
 * Master class to objects having actions attached.
 * Used also to impersonate Rendez-Vous.
 */
#ifndef EVENT_H
#define EVENT_H

#include <vector>

#include "Handler.h"
#include "Object.h"
#include "ObjCollection.h"
#include "Tracker.h"

class Event : virtual public Object, public std::vector<Handler *> {
	// The vector contains the collection of handler to launch

	TrackerVector trackersToEnable;
	TrackerVector trackersToDisable;

public:
	/* Constructor from a file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 */
	Event( const std::string &file, std::string &where, std::string &name  );
	Event(){};

	void addHandler( Handler *h ){ this->push_back(h); }

	/* Handlers execution
	 * (runs sequentially)
	 */
	void execHandlers(void);		// fresh State but in the same thread
	void execHandlers(lua_State *);	// same thread, same State

	/* Trackers */
	void addTrackerEN( Tracker *t ){ this->trackersToEnable.Add(t); }
	void addTrackerDIS( Tracker *t ){ this->trackersToDisable.Add(t); }

	/* Create Lua's object */
	static void initLuaInterface(lua_State *L);
};

typedef ObjCollection<Event *> EventCollection;
typedef ObjVector<Event *> EventVector;

#endif
