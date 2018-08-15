#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

#include "Helpers.h"
#include "MQTTTopic.h"

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
