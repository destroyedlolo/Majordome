/* MQTT topic object
 *
 * 28/07/2018 - LF - First version
 */

#ifndef MQTTTOPIC_H
#define MQTTTOPIC_H

#include "MayBeEmptyString.h"
#include "Event.h"

class MQTTTopic : public Event {
	// LuaTasks to launch are stored in the Event

public :
	StringVector trackers;	// Trackers to launch at message arrival
	StringVector minmaxes;	// MinMax to launch at message arrival

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

	bool enable( void );
	bool disable( void );

	/* check if an incoming topic matches this one
	 * -> intopic : incoming topic
	 * <- matching (bool) or not (false)
	 */
	bool match( const char *intopic );

		/* Have a look on *Task() in Event.h for some documentation */
	void addTracker( std::string t ){ this->trackers.Add(t); }
	void execTrackers( Config &, const char *name, const char *topic, const char *payload );
	void execTrackers( Config &, const char *name );

	void addMinMax( std::string t ){ this->minmaxes.Add(t); }
	void execMinMax( Config &, const char *name, const char *topic, const char *payload );

	/* Create Lua's object */
	static void initLuaObject( lua_State *L );
};

#endif
