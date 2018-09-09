/* Read config directories tree 
 *
 * 26/07/2018 - LF - First version
 */

#include <sys/stat.h>
#include <errno.h>
#include <cstring>

#include <libSelene.h>

#include "Helpers.h"
#include "Config.h"
#include "SubConfigDir.h"

bool Config::accept( const char *fch, std::string &full ){
	if( SortDir::accept( fch, full) ){

		struct stat buff;
		if( stat( full.c_str(), &buff )<0 ){
			publishLog('E', "%s : %s", fch, strerror(errno));
			return false;
		}

		return( !!(buff.st_mode & S_IFDIR) );
	} else
		return false;
}

void Config::init(const char *where, lua_State *L){
	this->readdircontent( where );

		/* Load packages */
	for( iterator i=this->begin(); i<this->end(); i++){
		publishLog('L', "Loading '%s'", (*i).c_str());

		std::string completpath = where;
		completpath += '/';
		completpath += *i;

		SubConfigDir( *this, completpath, L );
	}
}

void Config::SanityChecks( void ){

		/* May be removed future version if only dynamic topics are allowed */
	if(TopicsList.empty()){
		publishLog('F', "There is not topic defined");
		exit(EXIT_FAILURE);
	}

	 /* 
	  * Verify topics overlapping
	  */
	for(TopicElements::iterator i = TopicsList.begin(); i != TopicsList.end(); i++){
		TopicElements::iterator j = i;
		for(j++; j != TopicsList.end(); j++){
			if( !(i->second.hasWildcard()) && !(j->second.hasWildcard()) ){ // No wildcard
				publishLog('F', "Same MQTT topic used for topics '%s' from '%s' and '%s' from '%s'",
					i->second.getName().c_str(), 
					i->second.getWhere().c_str(), 
					j->second.getName().c_str(),
					j->second.getWhere().c_str()
				);
				exit(EXIT_FAILURE);
			} else if( i->second.hasWildcard() && j->second.hasWildcard() ){	// Both contain wildcard
			} else if( i->second.hasWildcard() ){
				if( !mqtttokcmp( i->second.getTopic(), j->second.getTopic() )){
					publishLog('F', "'%s' from '%s' hides '%s' from '%s'",
						i->second.getName().c_str(), 
						i->second.getWhere().c_str(), 
						j->second.getName().c_str(),
						j->second.getWhere().c_str()
					);
					exit(EXIT_FAILURE);
				}
			} else if( j->second.hasWildcard() ){
				if( !mqtttokcmp( j->second.getTopic(), i->second.getTopic() )){
					publishLog('W', "'%s' from '%s' overlaps '%s' from '%s'",
						i->second.getName().c_str(), 
						i->second.getWhere().c_str(), 
						j->second.getName().c_str(),
						j->second.getWhere().c_str()
					);
				}
			}
		}
	}
}

void Config::SubscribeTopics( void ){
	for(TopicElements::iterator i = TopicsList.begin(); i != TopicsList.end(); i++){
		if( i->second.isEnabled() ){
			int err;
			if( (err = MQTTClient_subscribe(
				MQTT_client, 
				i->second.getTopic(),
				i->second.getQOS()
			)) != MQTTCLIENT_SUCCESS){
				publishLog('D', "Can't subscribe to '%s' : error %d", i->second.getTopic(), err);
				exit(EXIT_FAILURE);
			}
		}
	}
}

void Config::LaunchTimers( void ){
	for( TimerElements::iterator i = this->TimersList.begin(); i != config.TimersList.end(); i++)
		(*i).second.launchThread();
}

void Config::RunImmediates( void ){
	for( TimerElements::iterator i = this->TimersList.begin(); i != config.TimersList.end(); i++){
		if( (*i).second.getImmediate() || (*i).second.isOver() )
			(*i).second.execTasks();
	}
}

LuaTask &Config::findTask( std::string &n ) {
	Config::TaskElements::iterator tsk;

	if( (tsk = this->TasksList.find( n )) == this->TasksList.end() )
		throw 1;
	else
		return (*tsk).second;
}
