#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <regex>

extern "C" {
    #include "lualib.h"
    #include "lauxlib.h"
};

#include "Selene.h"
#include "Helpers.h"
#include "MQTTTopic.h"
#include "Config.h"

MQTTTopic::MQTTTopic( const std::string &fch, std::string &where, std::string &name ) : qos(0), store(false), numeric(false), ttl(0) {

	this->extrName( fch, name );
	this->name = name;
	this->where = where;

	/*
	 * Reading file's content
	 */
	if(verbose)
		SelLog->Log('L', "\t'%s'", fch.c_str());

	std::ifstream file;
	file.exceptions ( std::ios::eofbit | std::ios::failbit ); // No need to check failbit
	try {
		std::string l;
		bool alreadydefault = false;

		file.open(fch);
		while( std::getline( file, l) ){
			MayBeEmptyString arg;

			if( !!(arg = striKWcmp( l, "name=" )) ){
				if(alreadydefault){
					SelLog->Log('F',"'name' can't be used after 'default'");
					exit(EXIT_FAILURE);
				}
				this->name = name = arg;
				if(verbose)
					SelLog->Log('C', "\t\tChanging name to '%s'", name.c_str());
			} else if( !!(arg = striKWcmp( l, "topic=" )) ){
				this->topic = std::regex_replace(arg, std::regex("%ClientID%"), MQTT_ClientID);
				if(verbose)
					SelLog->Log('C', "\t\ttopic : '%s'", this->topic.c_str());
			} else if( !!(arg = striKWcmp( l, "qos=" ))){
				if((this->qos = stoi(arg)) > 2)	// If invalid
					this->qos = 0;
				if(verbose)
					SelLog->Log('C', "\t\tqos : '%d'", this->qos);
			} else if( l == "store" ){
				if(verbose)
					SelLog->Log('C', "\t\tStore in a SelShared");
				this->store = true;
			} else if( l == "numeric" ){
				if(alreadydefault){
					SelLog->Log('F',"'default' must be set after 'numeric'");
					exit(EXIT_FAILURE);
				}
				if(verbose)
					SelLog->Log('C', "\t\tStore as a numeric value");
				this->numeric = true;
			} else if( !!(arg = striKWcmp( l, "ttl=" )) ){
				if(alreadydefault){
					SelLog->Log('F',"'ttl' can't be set after 'default'");
					exit(EXIT_FAILURE);
				}
				this->ttl = strtoul( arg.c_str(), NULL, 0 );
				if(verbose)
					SelLog->Log('C', "\t\tTTL = %lu", this->ttl);
			} else if( !!(arg = striKWcmp( l, "default=" )) ){
				alreadydefault = true;
				if( !this->toBeStored() )
					SelLog->Log('E',"Default value is only useful for a stored topic");
				else {
					if( this->numeric ){
						try {
							double val = std::stod( arg );
							SelSharedVar->setNumber( this->getNameC(), val, this->ttl );

							if(verbose)
								SelLog->Log('C', "\t\tdefault = %lf", val);
						} catch( ... ){
							SelLog->Log('F', "Topic '%s' is expecting a number : no convertion done ", this->getNameC() );
							exit(EXIT_FAILURE);
						}
					} else {
						SelSharedVar->setString( this->getNameC(), arg.c_str(), this->ttl );
						if(verbose)
							SelLog->Log('C', "\t\tdefault = \"%s\"", arg.c_str());
					}
				}
			} else if( l == "quiet" ){
				if(verbose)
					SelLog->Log('C', "\t\tBe quiet");
				this->beQuiet();
			} else if( l == "disabled" ){
				if(verbose)
					SelLog->Log('C', "\t\tDisabled");
				this->disable();
			}
#if 0
else SelLog->Log('D', "Ignore '%s'", l.c_str());
#endif
		}
	} catch(const std::ifstream::failure &e){
		if(!file.eof()){
			SelLog->Log('F', "%s : %s", fch.c_str(), strerror(errno) );
			exit(EXIT_FAILURE);
		}
	} catch(const std::invalid_argument &e){
		SelLog->Log('F', "%s : invalid argument", fch.c_str() );
		exit(EXIT_FAILURE);
	}

	file.close();

	/*
	 * Sanity checks
	 */

	if( !this->topic ){
		SelLog->Log('F', "%s : No topic defined", fch.c_str() );
		exit(EXIT_FAILURE);
	}
	this->wildcard =
		this->topic.find('#') != std::string::npos ||
		this->topic.find('+') != std::string::npos;

#ifdef DEBUG
	if( this->wildcard && debug )
		SelLog->Log('D', "\t\tHas wildcard");
#endif
}

bool MQTTTopic::match( const char *intopic ){
	if( this->isEnabled() )
		return(!SelMQTT->mqtttokcmp(this->getTopic(), intopic));

	return false;
}

bool MQTTTopic::enable( void ){
	int err;
	if( (err = MQTTClient_subscribe(
		MQTT_client, 
		this->getTopic(),
		this->getQOS()
	)) != MQTTCLIENT_SUCCESS){
		SelLog->Log('E', "Can't subscribe to '%s' : error %d", this->getTopic(), err);
		return false;
	}

	this->Event::enable();
	return true;
}

bool MQTTTopic::disable( void ){
	int err;
	if( (err = MQTTClient_unsubscribe(
		MQTT_client, 
		this->getTopic()
	)) != MQTTCLIENT_SUCCESS){
		SelLog->Log('E', "Can't unsubscribe to '%s' : error %d", this->getTopic(), err);
		return false;
	}
	this->Event::disable();
	return true;
}

void MQTTTopic::execTrackers( Config &cfg, const char *trig_name, const char *topic, const char *payload ){
#ifdef DEBUG
	if(debug && !this->isQuiet())
		SelLog->Log('D', "execTrackers() : %d to run", this->trackers.size() );
#endif

#if 0	/* AF Tracker */
	for( StringVector::iterator trk = this->trackers.begin(); trk != this->trackers.end(); trk++){
		try {
			Tracker &tracker = cfg.findTracker( *trk );
			tracker.exec( trig_name, topic, payload );
		} catch (...) {
			SelLog->Log('F', "Internal error : can't find tracker \"%s\"", (*trk).c_str() );
			exit(EXIT_FAILURE);
		}
	}
#endif
}

void MQTTTopic::execTrackers( Config &cfg, const char *timer_name ){
#ifdef DEBUG
	if(debug && !this->isQuiet())
		SelLog->Log('D', "execTrackers() : %d to run", this->trackers.size() );
#endif

#if 0	/* AF Tracker */
	for( StringVector::iterator trk = this->trackers.begin(); trk != this->trackers.end(); trk++){
		try {
			Tracker &tracker = cfg.findTracker( *trk );
			tracker.exec( timer_name );
		} catch (...) {
			SelLog->Log('F', "Internal error : can't find tracker \"%s\"", (*trk).c_str() );
			exit(EXIT_FAILURE);
		}
	}
#endif
}


	/*****
	 * Lua exposed functions
	 *****/
static class MQTTTopic *checkMajordomeMQTTTopic(lua_State *L){
	class MQTTTopic **r = (class MQTTTopic **)luaL_testudata(L, 1, "MajordomeMQTTTopic");
	luaL_argcheck(L, r != NULL, 1, "'MajordomeMQTTTopic' expected");
	return *r;
}

static int mtpc_find(lua_State *L){
/* Find a topic
 * 1 : topic to find
 * 2: true if it has to fail if not found
 */
	const char *name = luaL_checkstring(L, 1);
	int tofail=  lua_toboolean(L, 2);

	try {
		class MQTTTopic &tpc = config.TopicsList.at( name );
		class MQTTTopic **topic = (class MQTTTopic **)lua_newuserdata(L, sizeof(class MQTTTopic *));
		assert(topic);

		*topic = &tpc;
		luaL_getmetatable(L, "MajordomeMQTTTopic");
		lua_setmetatable(L, -2);

		return 1;
	} catch( std::out_of_range &e ){	// Not found
		if( tofail )
			return luaL_error( L, "Can't find \"%s\" topic", name );
		return 0;
	}
}

static const struct luaL_Reg MajTopicLib [] = {
	{"find", mtpc_find},
	{NULL, NULL}
};

static int mtpc_Publish(lua_State *L){
/* Publish to a topic
 *	2 : value
 *	3 : retain
 */
	class MQTTTopic *topic = checkMajordomeMQTTTopic(L);
	const char *val = luaL_checkstring(L, 2);
	int retain =  lua_toboolean(L, 3);

	if( topic->isEnabled() )
		SelMQTT->mqttpublish( MQTT_client, topic->getTopic(), strlen(val), (void *)val, retain );
	else if(verbose)
		SelLog->Log('I', "'%s' is disabled : sending ignored", topic->getTopic());

	return 0;
}

static int mtpc_getTopic( lua_State *L ){
	class MQTTTopic *topic = checkMajordomeMQTTTopic(L);
	lua_pushstring( L, topic->getTopic() );
	return 1;
}

static int mtpc_getVal( lua_State *L ){
	class MQTTTopic *topic = checkMajordomeMQTTTopic(L);

#ifdef DEBUG
	if( !topic->toBeStored() && debug )
		SelLog->Log('D', "Topic %s is not \"stored\" : getVal() will not works", topic->getNameC() );
#endif

	enum SharedObjType type;
	union SelSharedVarContent var = SelSharedVar->getValue( topic->getNameC(), &type, false );

	switch( type ){
	case SOT_NUMBER:
		lua_pushnumber( L, var.num );
		return 1;
	case SOT_STRING:
	case SOT_XSTRING:
		lua_pushstring( L, var.str );
		return 1;
	default:
		return 0;
	}
}

static int mtpc_Launch( lua_State *L ){
	class MQTTTopic *topic = checkMajordomeMQTTTopic(L);

	if( topic->isEnabled() )
		topic->execTasks(config, topic->getNameC(), NULL, "fake");
#ifdef DEBUG
	else if( debug )
		SelLog->Log('D', "Topic %s is disabled : no tasks launched", topic->getNameC() );
#endif	
	return 0;
}

static int mtpc_getContainer( lua_State *L ){
	class MQTTTopic *topic = checkMajordomeMQTTTopic(L);
	lua_pushstring( L, topic->getWhereC() );
	return 1;
}

static int mtpc_getName( lua_State *L ){
	class MQTTTopic *topic = checkMajordomeMQTTTopic(L);
	lua_pushstring( L, topic->getName().c_str() );
	return 1;
}

static int mtpc_enabled( lua_State *L ){
	class MQTTTopic *topic = checkMajordomeMQTTTopic(L);
	topic->enable();
	return 0;
}

static int mtpc_disable( lua_State *L ){
	class MQTTTopic *topic = checkMajordomeMQTTTopic(L);
	topic->disable();
	return 0;
}

static int mtpc_isEnabled( lua_State *L ){
	class MQTTTopic *topic = checkMajordomeMQTTTopic(L);
	lua_pushboolean( L, topic->isEnabled() );
	return 1;
}

static const struct luaL_Reg MajTopicM [] = {
	{"Publish", mtpc_Publish},
	{"getTopic", mtpc_getTopic},
	{"getVal", mtpc_getVal},
	{"Launch", mtpc_Launch},
	{"getContainer", mtpc_getContainer},
	{"getName", mtpc_getName},
	{"isEnabled", mtpc_isEnabled},
	{"Enable", mtpc_enabled},
	{"Disable", mtpc_disable},
	{NULL, NULL}
};

int MQTTTopic::initLuaObject( lua_State *L ){
	SelLua->objFuncs( L, "MajordomeMQTTTopic", MajTopicM );
	SelLua->libCreateOrAddFuncs( L, "MajordomeMQTTTopic", MajTopicLib );

	return 1;
}

