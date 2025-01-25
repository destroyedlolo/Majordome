/* Read config directories tree 
 *
 * 26/07/2018 - LF - First version
 * 20/05/2024 - LF - Migrate to v4
 * 20/01/2025 - LF - Migrate to v6
 */

#include "Config.h"
#include "Selene.h"
#include "Helpers.h"
#include "SubConfigDir.h"

#include <sys/stat.h>
#include <cstring>	// strerror()

	// Reject non-directory
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
	for(auto i=this->begin(); i<this->end(); i++){
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
		TopicCollection::iterator j = i;
		for(j++; j != TopicsList.end(); j++){
			if( !(i->second->hasWildcard()) && !(j->second->hasWildcard()) ){ // No wildcard
				if( i->second->getTopic() == j->second->getTopic() ){
					SelLog->Log('F', "Same MQTT topic used for topics '%s' from '%s' and '%s' from '%s'",
						i->second->getName().c_str(), 
						i->second->getWhere().c_str(), 
						j->second->getName().c_str(),
						j->second->getWhere().c_str()
					);
					exit(EXIT_FAILURE);
				}
			} else if( i->second->hasWildcard() && j->second->hasWildcard() ){	// Both contain wildcard
			} else if( i->second->hasWildcard() ){
				if( !SelMQTT->mqtttokcmp( i->second->getTopicC(), j->second->getTopicC() )){
					SelLog->Log('F', "'%s' from '%s' hides '%s' from '%s'",
						i->second->getName().c_str(), 
						i->second->getWhere().c_str(), 
						j->second->getName().c_str(),
						j->second->getWhere().c_str()
					);
					exit(EXIT_FAILURE);
				}
			} else if( j->second->hasWildcard() ){
				if( !SelMQTT->mqtttokcmp( j->second->getTopicC(), i->second->getTopicC() )){
					SelLog->Log('W', "'%s' from '%s' overlaps '%s' from '%s'",
						i->second->getName().c_str(), 
						i->second->getWhere().c_str(), 
						j->second->getName().c_str(),
						j->second->getWhere().c_str()
					);
				}
			}
		}
	}

/* TODO */
}

void Config::RunStartups( void ){
	for(auto &i : this->TasksList){
		if( i.second->getRunAtStartup() )
			i.second->exec();
	}
}

