/* MQTT topic object
 *
 * 28/07/2018 - LF - First version
 */

#ifndef MQTTTOPIC_H
#define MQTTTOPIC_H

#include "MayBeEmptyString.h"
#include "Event.h"
#include "ObjCollection.h"

class MQTTTopic : virtual public Object, public Event {
	bool alreadydefault;	// true if default has been already used

	virtual void readConfigDirective(std::string &l);

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
	MQTTTopic( const std::string &file, std::string &where );
	
		/* Accessors */
	std::string &getTopic( void ){ return this->topic; };
	const char *getTopicC( void ){ return this->topic.c_str(); };
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

	/* A message arrived */
	void execHandlers(MQTTTopic &, const char *topic, const char *msg);

	/* Create Lua's object */
	static void initLuaInterface( lua_State *L );
};

typedef ObjCollection<MQTTTopic *> TopicCollection;
#endif
