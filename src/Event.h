/* Event
 * Master class to object that has action attached
 */
#ifndef EVENT_H
#define EVENT_H

#include <vector>

#include "Object.h"
#include "LuaTask.h"

class Config;

class Event : public Object {

	// Note : 
	// creating a vector of reference is note widely allowed
	// using pointers will crash as the task object used during configuration reading
	// will not survive to the said reading
	// Consequently, we are stored here only the name of the task.

	typedef std::vector<std::string> TaskEntries;	// List of tasks'name to launch when a message arrives
	TaskEntries tasks;

public:
	typedef TaskEntries::iterator iterator;
	typedef TaskEntries::const_iterator const_iterator;
	iterator begin() { return tasks.begin(); }
	iterator end() { return tasks.end(); }

	void addTasks( std::string t ){ this->tasks.push_back(t); }

	/* launch tasks associated to this event (topic)
	 * -> name of the object that triggers the task
	 * -> topic : the one that triggers the task
	 */
	void execTasks( Config &, const char *name, const char *topic, const char *payload );

	/* launch tasks associated to this event (timer)
	 * -> name of the object that triggers the task
	 */
	void execTasks( Config &, const char *name );
};

#endif
