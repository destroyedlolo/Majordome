/* Read config directories tree 
 *
 * 26/07/2018 - LF - First version
 */

#include <sys/stat.h>
#include <errno.h>
#include <cstring>

#include <libSelene.h>

#include "Components.h"
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

Config::Config(const char *where, lua_State *L){
	this->readdircontent( where );

	for( iterator i=this->begin(); i<this->end(); i++){
		publishLog('L', "Loading '%s'", (*i).c_str());

		std::string completpath = where;
		completpath += '/';
		completpath += *i;

		SubConfigDir( *this, completpath, L );
	}
	this->SanityChecks();
}

void Config::SanityChecks( void ){
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
