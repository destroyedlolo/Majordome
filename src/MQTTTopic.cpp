#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cassert>

extern "C" {
    #include "lualib.h"
    #include "lauxlib.h"
};

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
		publishLog('L', "\t'%s'", fch.c_str());

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
					publishLog('F',"'name' can't be used after 'default'");
					exit(EXIT_FAILURE);
				}
				this->name = name = arg;
				if(verbose)
					publishLog('C', "\t\tChanging name to '%s'", name.c_str());
			} else if( !!(arg = striKWcmp( l, "topic=" )) ){
				this->topic = arg;
				if(verbose)
					publishLog('C', "\t\ttopic : '%s'", this->topic.c_str());
			} else if( !!(arg = striKWcmp( l, "qos=" ))){
				if((this->qos = stoi(arg)) > 2)	// If invalid
					this->qos = 0;
				if(verbose)
					publishLog('C', "\t\tqos : '%d'", this->qos);
			} else if( l == "store" ){
				if(verbose)
					publishLog('C', "\t\tStore in a SelShared");
				this->store = true;
			} else if( l == "numeric" ){
				if(alreadydefault){
					publishLog('F',"'default' must be set after 'numeric'");
					exit(EXIT_FAILURE);
				}
				if(verbose)
					publishLog('C', "\t\tStore as a numeric value");
				this->numeric = true;
			} else if( !!(arg = striKWcmp( l, "ttl=" )) ){
				if(alreadydefault){
					publishLog('F',"'ttl' can't be set after 'default'");
					exit(EXIT_FAILURE);
				}
				this->ttl = strtoul( arg.c_str(), NULL, 0 );
				if(verbose)
					publishLog('C', "\t\tTTL = %lu", this->ttl);
			} else if( !!(arg = striKWcmp( l, "default=" )) ){
				alreadydefault = true;
				if( !this->toBeStored() )
					publishLog('E',"Default value is only useful for a stored topic");
				else {
					if( this->numeric ){
						try {
							double val = std::stod( arg );
							soc_setn( this->getNameC(), val, this->ttl );

							if(verbose)
								publishLog('C', "\t\tdefault = %lf", val);
						} catch( ... ){
							publishLog('F', "Topic '%s' is expecting a number : no convertion done ", this->getNameC() );
							exit(EXIT_FAILURE);
						}
					} else {
						soc_sets( this->getNameC(), arg.c_str(), this->ttl );
						if(verbose)
							publishLog('C', "\t\tdefault = \"%s\"", arg.c_str());
					}
				}
			} else if( l == "quiet" ){
				if(verbose)
					publishLog('C', "\t\tBe quiet");
				this->beQuiet();
			} else if( l == "disabled" ){
				if(verbose)
					publishLog('C', "\t\tDisabled");
				this->disable();
			}
#if 0
else publishLog('D', "Ignore '%s'", l.c_str());
#endif
		}
	} catch(const std::ifstream::failure &e){
		if(!file.eof()){
			publishLog('F', "%s : %s", fch.c_str(), strerror(errno) );
			exit(EXIT_FAILURE);
		}
	} catch(const std::invalid_argument &e){
		publishLog('F', "%s : invalid argument", fch.c_str() );
		exit(EXIT_FAILURE);
	}

	file.close();

	/*
	 * Sanity checks
	 */

	if( !this->topic ){
		publishLog('F', "%s : No topic defined", fch.c_str() );
		exit(EXIT_FAILURE);
	}
	this->wildcard =
		this->topic.find('#') != std::string::npos ||
		this->topic.find('+') != std::string::npos;

#ifdef DEBUG
	if( this->wildcard && debug )
		publishLog('D', "\t\tHas wildcard");
#endif
}

bool MQTTTopic::match( const char *intopic ){
	if( this->isEnabled() )
		return(!mqtttokcmp(this->getTopic(), intopic));

	return false;
}

bool MQTTTopic::enable( void ){
	int err;
	if( (err = MQTTClient_subscribe(
		MQTT_client, 
		this->getTopic(),
		this->getQOS()
	)) != MQTTCLIENT_SUCCESS){
		publishLog('E', "Can't subscribe to '%s' : error %d", this->getTopic(), err);
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
		publishLog('E', "Can't unsubscribe to '%s' : error %d", this->getTopic(), err);
		return false;
	}
	this->Event::disable();
	return true;
}

void MQTTTopic::execTrackers( Config &cfg, const char *trig_name, const char *topic, const char *payload ){
#ifdef DEBUG
	if(debug && !this->isQuiet())
		publishLog('D', "execTrackers() : %d to run", this->trackers.size() );
#endif

	for( StringVector::iterator trk = this->trackers.begin(); trk != this->trackers.end(); trk++){
		try {
			Tracker &tracker = cfg.findTracker( *trk );
			tracker.exec( trig_name, topic, payload );
		} catch (...) {
			publishLog('F', "Internal error : can't find tracker \"%s\"", (*trk).c_str() );
			exit(EXIT_FAILURE);
		}
	}
}

void MQTTTopic::execTrackers( Config &cfg, const char *timer_name ){
#ifdef DEBUG
	if(debug && !this->isQuiet())
		publishLog('D', "execTrackers() : %d to run", this->trackers.size() );
#endif

	for( StringVector::iterator trk = this->trackers.begin(); trk != this->trackers.end(); trk++){
		try {
			Tracker &tracker = cfg.findTracker( *trk );
			tracker.exec( timer_name );
		} catch (...) {
			publishLog('F', "Internal error : can't find tracker \"%s\"", (*trk).c_str() );
			exit(EXIT_FAILURE);
		}
	}
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
		mqttpublish( MQTT_client, topic->getTopic(), strlen(val), (void *)val, retain );

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
		publishLog('D', "Topic %s is not \"stored\" : getVal() will not works", topic->getNameC() );
#endif

	struct SharedVarContent var;
	soc_get( topic->getNameC(), &var );

	switch( var.type ){
	case SOT_NUMBER:
		lua_pushnumber( L, var.val.num );
		soc_free( &var );
		return 1;
	case SOT_STRING:
	case SOT_XSTRING:
		lua_pushstring( L, var.val.str );
		soc_free( &var );
		return 1;
	default:
		soc_free( &var );
		return 0;
	}
}

static int mtpc_Launch( lua_State *L ){
	class MQTTTopic *topic = checkMajordomeMQTTTopic(L);

	if( topic->isEnabled() )
		topic->execTasks(config, topic->getNameC(), NULL, "fake");
#ifdef DEBUG
	else if( debug )
		publishLog('D', "Topic %s is disabled : no tasks launched", topic->getNameC() );
#endif	
	return 0;
}

static int mtpc_getContainer( lua_State *L ){
	class MQTTTopic *topic = checkMajordomeMQTTTopic(L);
	lua_pushstring( L, topic->getWhereC() );
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
	{"isEnabled", mtpc_isEnabled},
	{"Enable", mtpc_enabled},
	{"Disable", mtpc_disable},
	{NULL, NULL}
};

int MQTTTopic::initLuaObject( lua_State *L ){
	libSel_objFuncs( L, "MajordomeMQTTTopic", MajTopicM );
	libSel_libFuncs( L, "MajordomeMQTTTopic", MajTopicLib );

	return 1;
}

