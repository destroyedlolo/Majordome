/* MQTT topic object
 *
 * 28/07/2018 - LF - First version
 */

#ifndef MQTTTOPIC_H
#define MQTTTOPIC_H

#include <vector>

#include "MayBeEmptyString.h"
#include "Object.h"
#include "LuaTask.h"

class MQTTTopic : public Object {
	typedef std::vector<LuaTask *> TaskEntries;	// List of tasks to launch when a message arrives
	TaskEntries tasks;

public:
	typedef TaskEntries::iterator iterator;
	typedef TaskEntries::const_iterator const_iterator;
	iterator begin() { return tasks.begin(); }
	iterator end() { return tasks.end(); }

protected:
	MayBeEmptyString topic;	// Topic to look for
	unsigned int qos;		// Associated QoS
	bool wildcard;		// True if the topic contains wildcard

public:
	/* Constructor from a file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 */
	MQTTTopic( const std::string &file, std::string &where, std::string &name  );

	const char *getTopic( void ){ return this->topic.c_str(); };
	bool hasWildcard( void ){ return this->wildcard; };

	void addTasks( LuaTask *t ){ this->tasks.push_back(t); }
};

#endif
