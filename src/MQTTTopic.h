/* MQTT topic object
 *
 * 28/07/2018 - LF - First version
 */

#ifndef MQTTTOPIC_H
#define MQTTTOPIC_H

#include <string>

#include "Object.h"

class MQTTTopic : public Object {
	std::string topic;	// Topic to look for
	int qos;		// Associated QoS

public:
	/* Constructor from a file */
	MQTTTopic( std::string &  );
};

#endif
