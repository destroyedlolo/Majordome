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
	bool wildcard;			// True if the topic contains wildcard
	bool store;				// Keep values in a SelShared variable
	bool numeric;			// it's a numeric value
	unsigned long int ttl;	// time to live

public:
	/* Constructor from a file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 */
	MQTTTopic( const std::string &file, std::string &where, std::string &name  );

	const char *getTopic( void ){ return this->topic.c_str(); };
	unsigned int getQOS( void ){ return this->qos; };
	bool hasWildcard( void ){ return this->wildcard; };
	bool toBeStored( void ){ return this->store; };
	bool isNumeric( void ){ return this->numeric; };
	unsigned long int getTTL( void ){ return this->ttl; };

	/* check if an incoming topic matches this one
	 * -> intopic : incoming topic
	 * <- matching (bool) or not (false)
	 */
	bool match( const char *intopic );


	/* Create Lua's object */
	static int initLuaObject( lua_State *L );
};

#endif
