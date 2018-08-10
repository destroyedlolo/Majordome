/* Event
 * Master class to object that has action attached
 */
#ifndef EVENT_H
#define EVENT_H

#include <vector>

#include "Object.h"
#include "LuaTask.h"

class Event : public Object {
	typedef std::vector<LuaTask *> TaskEntries;	// List of tasks to launch when a message arrives
	TaskEntries tasks;

public:
	typedef TaskEntries::iterator iterator;
	typedef TaskEntries::const_iterator const_iterator;
	iterator begin() { return tasks.begin(); }
	iterator end() { return tasks.end(); }

	void addTasks( LuaTask *t ){ this->tasks.push_back(t); }

};

#endif
