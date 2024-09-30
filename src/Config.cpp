/* Read config directories tree 
 *
 * 26/07/2018 - LF - First version
 * 20/05/2024 - LF - Migrate to v4
 */
#include "Config.h"
#include "Selene.h"
#include "Helpers.h"
#include "SubConfigDir.h"

#include <sys/stat.h>
#include <cstring>

bool Config::accept( const char *fch, std::string &full ){
	if( SortDir::accept( fch, full) ){

		struct stat buff;
		if( stat( full.c_str(), &buff )<0 ){
			SelLog->Log('E', "%s : %s", fch, strerror(errno));
			return false;
		}

		return( !!(buff.st_mode & S_IFDIR) );
	} else
		return false;
}

void Config::init(std::string &where, lua_State *L){
	this->configDir = where;
	this->readdircontent(where);

		/* Load packages */
	for( iterator i=this->begin(); i<this->end(); i++){
		if(!quiet)
			SelLog->Log('L', "Loading '%s'", (*i).c_str());

		std::string completpath = where;
		completpath += '/';
		completpath += *i;

		SubConfigDir( *this, completpath, L );
	}
}

void Config::SanityChecks( void ){
	 /* 
	  * Verify topics overlapping
	  */
	for(auto i = TopicsList.begin(); i != TopicsList.end(); i++){
		TopicElements::iterator j = i;
		for(j++; j != TopicsList.end(); j++){
			if( !(i->second.hasWildcard()) && !(j->second.hasWildcard()) ){ // No wildcard
				if( !strcmp( i->second.getTopic(), j->second.getTopic() )){
					SelLog->Log('F', "Same MQTT topic used for topics '%s' from '%s' and '%s' from '%s'",
						i->second.getName().c_str(), 
						i->second.getWhere().c_str(), 
						j->second.getName().c_str(),
						j->second.getWhere().c_str()
					);
					exit(EXIT_FAILURE);
				}
			} else if( i->second.hasWildcard() && j->second.hasWildcard() ){	// Both contain wildcard
			} else if( i->second.hasWildcard() ){
				if( !SelMQTT->mqtttokcmp( i->second.getTopic(), j->second.getTopic() )){
					SelLog->Log('F', "'%s' from '%s' hides '%s' from '%s'",
						i->second.getName().c_str(), 
						i->second.getWhere().c_str(), 
						j->second.getName().c_str(),
						j->second.getWhere().c_str()
					);
					exit(EXIT_FAILURE);
				}
			} else if( j->second.hasWildcard() ){
				if( !SelMQTT->mqtttokcmp( j->second.getTopic(), i->second.getTopic() )){
					SelLog->Log('W', "'%s' from '%s' overlaps '%s' from '%s'",
						i->second.getName().c_str(), 
						i->second.getWhere().c_str(), 
						j->second.getName().c_str(),
						j->second.getWhere().c_str()
					);
				}
			}
		}
	}

		/* Verify that needed tasks exists */
	for(auto i = TasksList.begin(); i != TasksList.end(); i++){
		for(auto j = i->second.needed_task.begin(); j != i->second.needed_task.end(); j++){
			Config::TaskElements::iterator task;
			if( (task = config.TasksList.find(j->c_str())) == config.TasksList.end()){
				SelLog->Log('F', "Task \"%s\" needed by \"%s\" doesn't exist", j->c_str(), i->second.getNameC());
				exit(EXIT_FAILURE);
			}
		}
	}

	for(auto i = TrackersList.begin(); i != TrackersList.end(); i++){
		for(auto j = i->second.needed_task.begin(); j != i->second.needed_task.end(); j++){
			Config::TaskElements::iterator task;
			if( (task = config.TasksList.find(j->c_str())) == config.TasksList.end()){
				SelLog->Log('F', "Task \"%s\" needed by \"%s\" doesn't exist", j->c_str(), i->second.getNameC());
				exit(EXIT_FAILURE);
			}
		}
	}
}

void Config::SubscribeTopics( void ){
	size_t nbre = TopicsList.size();
	const char **topics = new const char * [nbre];
	int *qoss = new int [nbre];

	nbre = 0;
	for(TopicElements::iterator i = TopicsList.begin(); i != TopicsList.end(); i++){
		if( i->second.isEnabled() ){
			topics[nbre] = i->second.getTopic();
			qoss[nbre++] = i->second.getQOS();
		}
	}
#ifdef DEBUG
	if(verbose)
		SelLog->Log('C', "Subscribing to %ld actives topics", nbre);
#endif

	if( nbre ){
		int err;
		if( (err = MQTTClient_subscribeMany(
			MQTT_client,
			nbre, (char* const*)topics, qoss
		)) != MQTTCLIENT_SUCCESS){
			SelLog->Log('E', "Can't subscribe to topics : error %d", err);
			exit(EXIT_FAILURE);
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

void Config::RunStartups( void ){
	for( TaskElements::iterator i = this->TasksList.begin(); i != config.TasksList.end(); i++){
		if( (*i).second.getRunAtStartup() )
			(*i).second.exec(NULL);
	}
}

LuaTask &Config::findTask( std::string &n ) {
	Config::TaskElements::iterator tsk;

	if( (tsk = this->TasksList.find( n )) == this->TasksList.end() )
		throw 1;
	else
		return (*tsk).second;
}

Tracker &Config::findTracker( std::string &n ) {
	Config::TrackerElements::iterator trk;

	if( (trk = this->TrackersList.find( n )) == this->TrackersList.end() )
		throw 1;
	else
		return (*trk).second;
}
