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

public:
	/* Constructor from a file */
	MQTTTopic( std::string &  );
};

#endif
