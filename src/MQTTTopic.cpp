#include "MQTTTopic.h"
#include "Config.h"
#include "Helpers.h"

#include <iostream>
#include <fstream>
#include <regex>

#include <cstring>
#include <cassert>

MQTTTopic::MQTTTopic(const std::string &fch, std::string &where, std::string &name) : Object(fch, where, name), alreadydefault(false){

	/*
	 * Reading file's content
	 */

	std::stringstream buffer;
	std::ifstream file;
	file.exceptions ( std::ios::eofbit | std::ios::failbit );
	try {
		std::ifstream file(fch);
		std::streampos pos;

		bool nameused = false;	// if so, the name can't be changed anymore

		/*
		 * Reading header (Majordome's commands)
		 */

		do {
			std::string l;
			pos = file.tellg();

			std::getline( file, l);
			if( l.compare(0, 2, "--") ){	// End of comments
				file.seekg( pos );
				break;
			}

			this->readConfigDirective(l, name, nameused);
		} while(true);

		file.close();
	} catch(const std::ifstream::failure &e){
		if(!file.eof()){
			SelLog->Log('F', "%s : %s", fch.c_str(), strerror(errno) );
			exit(EXIT_FAILURE);
		}
	}
}

void MQTTTopic::readConfigDirective( std::string &l, std::string &name, bool &nameused ){
	MayBeEmptyString arg;

	if( !!(arg = striKWcmp( l, "-->> topic=" ))){
		this->topic = std::regex_replace(arg, std::regex("%ClientID%"), MQTT_ClientID);
		if(verbose)
			SelLog->Log('C', "\t\ttopic : '%s'", this->topic.c_str());
	} else if( !!(arg = striKWcmp( l, "-->> qos=" ))){
		if((this->qos = stoi(arg)) > 2)	// If invalid
			this->qos = 0;
		if(verbose)
			SelLog->Log('C', "\t\tqos : '%d'", this->qos);
	} else if( l == "-->> store" ){
		if(verbose)
			SelLog->Log('C', "\t\tStore in a SelSharedVar");
		this->store = true;
	} else if( l == "-->> numeric" ){
		if(this->alreadydefault){
			SelLog->Log('F',"'default' must be set after 'numeric'");
			exit(EXIT_FAILURE);
		}
		if(verbose)
			SelLog->Log('C', "\t\tStore as a numeric value");
		this->numeric = true;
	} else if( !!(arg = striKWcmp( l, "-->> ttl=" )) ){
		if(this->alreadydefault){
			SelLog->Log('F',"'ttl' can't be set after 'default'");
			exit(EXIT_FAILURE);
		}
		this->ttl = strtoul( arg.c_str(), NULL, 0 );
		if(verbose)
			SelLog->Log('C', "\t\tTTL = %lu", this->ttl);
	} else if( !!(arg = striKWcmp( l, "-->> default=" )) ){
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
		Object::readConfigDirective(l, name, nameused);
}

