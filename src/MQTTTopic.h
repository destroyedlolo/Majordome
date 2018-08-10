/* MQTT topic object
 *
 * 28/07/2018 - LF - First version
 */

#ifndef MQTTTOPIC_H
#define MQTTTOPIC_H

#include <vector>

#include "MayBeEmptyString.h"
#include "Event.h"

class MQTTTopic : public Event {
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
};

#endif
