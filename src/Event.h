/* Event
 * Master class to objects having actions attached.
 * Used also to impersonate Rendez-Vous.
 */
#ifndef EVENT_H
#define EVENT_H

#include <vector>

#include "Handler.h"
#include "HandlersExecutor.h"
#include "Object.h"
#include "ObjCollection.h"
#include "Tracker.h"

class Event : virtual public Object, virtual public HandlersExecutor {
	// The vector contains the collection of handler to launch

	TrackerVector trackersToEnable;
	TrackerVector trackersToDisable;

protected:
	virtual bool readConfigDirective(std::string &l);

public:
	/* Constructor from a file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 */
	Event( const std::string &file, std::string &where );
	Event(){};

	void addHandler( Handler *h ){ this->push_back(h); }
	void execHandlers(lua_State *);	// same thread, same State
	void execHandlers(void){ this->HandlersExecutor::execHandlers(); }

	/* Trackers */
	void addTrackerEN( Tracker *t ){ this->trackersToEnable.Add(t); }
	void addTrackerDIS( Tracker *t ){ this->trackersToDisable.Add(t); }

	/* Enable/Disable trackers from lists */
	void enableTrackers( void );
	void disableTrackers( void );

	/* Create Lua's object */
	static void initLuaInterface(lua_State *L);

	virtual std::string getTri(){ return Event::trigramme(); }
	static std::string trigramme(){ return "EVT_"; }
};

typedef ObjCollection<Event *> EventCollection;
typedef ObjVector<Event *> EventVector;

#endif
