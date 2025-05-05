#include "MQTTTopic.h"
#include "Config.h"
#include "Helpers.h"

#include <iostream>
#include <fstream>
#include <regex>

#include <cstring>
#include <cassert>

MQTTTopic::MQTTTopic(const std::string &fch, std::string &where) : Object(fch, where), alreadydefault(false), qos(0), wildcard(false), store(false), numeric(false), ttl(0){

	this->loadConfigurationFile(fch, where);

	/*
	 * Sanity checks
	 */

	if( this->topic.empty() ){
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

	if(d2)
		fd2 << this->getFullId() << ".class: Topic" << std::endl;
}

void MQTTTopic::readConfigDirective(std::string &l){
	std::string arg;

	if(!(arg = striKWcmp( l, "-->> topic=" )).empty()){
		this->topic = std::regex_replace(arg, std::regex("%ClientID%"), MQTT_ClientID);
		if(verbose)
			SelLog->Log('C', "\t\ttopic : '%s'", this->topic.c_str());
	} else if(!(arg = striKWcmp( l, "-->> qos=" )).empty()){
		if((this->qos = stoi(arg)) > 2)	// If invalid
			this->qos = 0;
		if(verbose)
			SelLog->Log('C', "\t\tqos : '%d'", this->qos);
	} else if(l == "-->> store"){
		if(verbose)
			SelLog->Log('C', "\t\tStore in a SelSharedVar");
		this->store = true;
	} else if(l == "-->> numeric"){
		if(this->alreadydefault){
			SelLog->Log('F',"'default' must be set after 'numeric'");
			exit(EXIT_FAILURE);
		}
		if(verbose)
			SelLog->Log('C', "\t\tStore as a numeric value");
		this->numeric = true;
	} else if(!(arg = striKWcmp( l, "-->> ttl=" )).empty()){
		if(this->alreadydefault){
			SelLog->Log('F',"'ttl' can't be set after 'default'");
			exit(EXIT_FAILURE);
		}
		this->ttl = strtoul( arg.c_str(), NULL, 0 );
		if(verbose)
			SelLog->Log('C', "\t\tTTL = %lu", this->ttl);
	} else if(!(arg = striKWcmp( l, "-->> default=" )).empty()){
		this->alreadydefault = true;
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
	} else 
		this->Object::readConfigDirective(l);
}

bool MQTTTopic::match( const char *intopic ){
	if( this->isEnabled() )
		return(!SelMQTT->mqtttokcmp(this->getTopicC(), intopic));

	return false;
}

void MQTTTopic::execHandlers(MQTTTopic &, const char *topic, const char *payload){
#ifdef DEBUG
	if(debug && !this->isQuiet() && !hideTopicArrival)
		SelLog->Log('D', "[%s] MQTTTopic::execHandlers() : %d handler(s) to run", this->getNameC(), this->size());
#endif

	for(auto &i : *this){
		lua_State *L = i->prepareExec();

		if(L){
			lua_pushstring( L, this->getNameC() );	// Push the name
			lua_setglobal( L, "MAJORDOME_TOPIC_NAME" );

			if(topic){	// Otherwise, launched by the Lua API
				lua_pushstring( L, topic );	// Push the topic
				lua_setglobal( L, "MAJORDOME_TOPIC" );
				lua_pushstring( L, payload );	// and its payload
				lua_setglobal( L, "MAJORDOME_PAYLOAD" );
			}

				/* Notez-bien : this function may be overloaded and, 
				 * the handler can be called SYNCHRONOUSLY if needed.
				 * (it's the case of MinMax).
				 * In such case, Lua state need to be freed
				 */
			i->execAsync(L);
		}
	}
}

	/*****
	 * Lua exposed functions
	 *****/
static class MQTTTopic *checkMajordomeMQTTTopic(lua_State *L){
	class MQTTTopic **r = (class MQTTTopic **)SelLua->testudata(L, 1, "MajordomeMQTTTopic");
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
		class MQTTTopic *tpc = config.TopicsList.at( name );
		class MQTTTopic **topic = (class MQTTTopic **)lua_newuserdata(L, sizeof(class MQTTTopic *));
		assert(topic);

		*topic = tpc;
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
		SelMQTT->mqttpublish( MQTT_client, topic->getTopicC(), strlen(val), (void *)val, retain );
	else if(verbose)
		SelLog->Log('I', "'%s' is disabled : sending ignored", topic->getTopic());

	return 0;
}

static int mtpc_getTopic( lua_State *L ){
	class MQTTTopic *topic = checkMajordomeMQTTTopic(L);
	lua_pushstring( L, topic->getTopicC() );
	return 1;
}

static int mtpc_getVal( lua_State *L ){
	class MQTTTopic *topic = checkMajordomeMQTTTopic(L);

#ifdef DEBUG
	if( !topic->toBeStored() && debug )
		SelLog->Log('E', "Topic %s is not \"stored\" : getVal() will not works", topic->getNameC() );
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

static int mtpc_Launch( lua_State *L ){
	class MQTTTopic *topic = checkMajordomeMQTTTopic(L);

	if( topic->isEnabled() )
		topic->execHandlers(*topic, NULL, NULL);
#ifdef DEBUG
	else if( debug )
		SelLog->Log('D', "Topic %s is disabled : no tasks launched", topic->getNameC() );
#endif	
	return 0;
}

static const struct luaL_Reg MajTopicM [] = {
	{"Publish", mtpc_Publish},
	{"getTopic", mtpc_getTopic},
	{"getVal", mtpc_getVal},
	{"getContainer", mtpc_getContainer},
	{"getName", mtpc_getName},
	{"isEnabled", mtpc_isEnabled},
	{"Enable", mtpc_enabled},
	{"Disable", mtpc_disable},
	{"Launch", mtpc_Launch},
	{NULL, NULL}
};

void MQTTTopic::initLuaInterface( lua_State *L ){
	SelLua->objFuncs( L, "MajordomeMQTTTopic", MajTopicM );
	SelLua->libCreateOrAddFuncs( L, "MajordomeMQTTTopic", MajTopicLib );
}

