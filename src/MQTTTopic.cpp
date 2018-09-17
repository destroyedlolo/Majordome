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

MQTTTopic::MQTTTopic( const std::string &fch, std::string &where, std::string &name ) : qos(0), store(false) {

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

		file.open(fch);
		while( std::getline( file, l) ){
			MayBeEmptyString arg;

			if( !!(arg = striKWcmp( l, "name=" )) ){
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
				if(verbose)
					publishLog('C', "\t\tStore as a numeric value");
				this->numeric = true;
			} else if( !!(arg = striKWcmp( l, "ttl=" )) ){
				this->ttl = strtoul( arg.c_str(), NULL, 0 );
				if(verbose)
					publishLog('C', "\t\tTTL = %lu", this->ttl);
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
	if( this->wildcard )
		publishLog('D', "\t\tHas wildcard");
#endif
}

bool MQTTTopic::match( const char *intopic ){
	if( this->isEnabled() )
		return(!mqtttokcmp(this->getTopic(), intopic));

	return false;
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
	const char *name = luaL_checkstring(L, 1);

	try {
		class MQTTTopic &tpc = config.TopicsList.at( name );
		class MQTTTopic **topic = (class MQTTTopic **)lua_newuserdata(L, sizeof(class MQTTTopic *));
		assert(topic);

		*topic = &tpc;
		luaL_getmetatable(L, "MajordomeMQTTTopic");
		lua_setmetatable(L, -2);

		return 1;
	} catch( std::out_of_range &e ){	// Not found 
		return 0;
	}
}

static const struct luaL_Reg MajTopicLib [] = {
	{"find", mtpc_find},
	{NULL, NULL}
};

static int mtpc_Launch( lua_State *L ){
	class MQTTTopic *topic = checkMajordomeMQTTTopic(L);

	if( topic->isEnabled() )
		topic->execTasks(config, topic->getNameC(), NULL, "fake");
#ifdef DEBUG
	else
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

