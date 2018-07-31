/* MQTT topic object
 *
 * 28/07/2018 - LF - First version
 */

#ifndef MQTTTOPIC_H
#define MQTTTOPIC_H

#include "MayBeEmptyString.h"

#include "Object.h"

class MQTTTopic : public Object {
protected:
	MayBeEmptyString topic;	// Topic to look for
	unsigned int qos;		// Associated QoS
	bool hasWildcard;		// True if the topic contains wildcard

public:
	/* Constructor from a file */
	MQTTTopic( const std::string &, std::string &name  );
};

#endif
