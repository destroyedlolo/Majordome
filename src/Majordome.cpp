/*
 * Majordome
 * 	An event based Automation System
 *
 * Copyright 2018-25 Laurent Faillie
 *
 * 		Majordome is covered by
 *		Creative Commons Attribution-NonCommercial 3.0 License
 *      (http://creativecommons.org/licenses/by-nc/3.0/) 
 *      Consequently, you're free to use if for personal or non-profit usage,
 *      professional or commercial usage REQUIRES a commercial licence.
 *  
 *      Majordome is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include "Version.h"
#include "Selene.h"
#include "MayBeEmptyString.h"


#define DEFAULT_CONFIGURATION_FILE "/usr/local/etc/Majordome.conf"

	/* ****
	 * global runtime configuration
	 * ****/
bool quiet = false;		// Avoid unnecessary messages
bool verbose = false;	// Add verbosity
bool trace = false;		// Trace internals like messages arrival
bool debug = false;		// Add debugging code (mostly messages)
bool hideTopicArrival = false;	// Silence topics arrival

bool configtest = false;	// Only validate the configuration, then exit

	/* *****
	 * local configuration
	 * (feed with default values)
	 * ******/
std::string MQTT_ClientID;
static std::string UserConfigRoot("/usr/local/etc/Majordome");	// Where to find user configuration
static std::string Broker_URL("tcp://localhost:1883");	// Broker to contact

	/* *****
	 * Main loop
	 * ******/
int main(int ac, char **av){
	initSelene();							// Load Séléné modules
	SelLog->configure(NULL, LOG_STDOUT);	// Early logging to STDOUT before broker initialisation
}
