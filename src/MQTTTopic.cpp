#include <iostream>
#include <fstream>
#include <cstring>

#include "Components.h"
#include "MQTTTopic.h"

MQTTTopic::MQTTTopic( const std::string &fch, std::string &name ) : qos(0) {

	/*
	 * determine the name from the filename
	 */

	name = fch;
	const size_t last_slash_idx = name.find_last_of("/");	// Filename only
	if(std::string::npos != last_slash_idx)
		name.erase(0, last_slash_idx + 1);

	const size_t period_idx = name.rfind('.');	// Remove extension if present.
	if (std::string::npos != period_idx)
		name.erase(period_idx);


	/*
	 * Reading file's content
	 */

#ifdef DEBUG
	publishLog('L', "\t'%s'", fch.c_str());
#endif

	std::ifstream file;
	file.exceptions ( std::ios::eofbit | std::ios::failbit ); // No need to check failbit
	try {
		std::string l;

		file.open(fch);
		while( std::getline( file, l) ){
			MayBeEmptyString arg;

			if( !!(arg = striKWcmp( l, "name=" ))){
				name = arg;
#ifdef DEBUG
				publishLog('D', "\t\tChanging name to '%s'", name.c_str());
#endif
			} else if( !!(arg = striKWcmp( l, "topic=" ))){
				this->topic = arg;
#ifdef DEBUG
				publishLog('D', "\t\ttopic : '%s'", this->topic.c_str());
#endif
			} else if( !!(arg = striKWcmp( l, "qos=" ))){
				if((this->qos = stoi(arg)) > 2)	// If invalid
					this->qos = 0;
#ifdef DEBUG
				publishLog('D', "\t\tqos : '%d'", this->qos);
#endif
			} else if( l == "disabled" ){
#ifdef DEBUG
				publishLog('D', "\t\tDisabled");
#endif
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
	this->hasWildcard =
		this->topic.find('#') != std::string::npos ||
		this->topic.find('+') != std::string::npos;

#ifdef DEBUG
	if( this->hasWildcard )
		publishLog('D', "\t\tHas Wildcard");
#endif
}
