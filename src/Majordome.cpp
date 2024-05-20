/*
 * Majordome
 * 	An event based Automation System
 *
 * Copyright 2018-24 Laurent Faillie
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

#include "Selene.h"
#include "Helpers.h"
#include "Version.h"
#include "Config.h"

#include <iostream>
#include <fstream>
#include <string>

#include <cstdio>
#include <cstdlib>
#include <cstring>		// strerror()
#include <cassert>

#include <unistd.h> 	// getopt()
#include <errno.h>
#include <libgen.h>		// basename()

using namespace std;

#define DEFAULT_CONFIGURATION_FILE "/usr/local/etc/Majordome.conf"

	/*****
	 * global configuration
	 *****/
bool verbose = false;
bool hideTopicArrival = false;	// Silence topics arrival
bool debug = false;
bool quiet = false;
bool trace = false;
bool configtest = false;

Config config;

	/******
	 * local configuration
	 * (feed with default values)
	 *******/
string MQTT_ClientID;
static string UserConfigRoot("/usr/local/etc/Majordome");	// Where to find user configuration
static string Broker_URL("tcp://localhost:1883");	// Broker to contact

static void read_configuration(const char *fch){
	std::ifstream file;
	file.exceptions ( std::ios::eofbit | std::ios::failbit ); // No need to check failbit

	try {
		std::string l;

		file.open(fch);
		while(std::getline( file, l)){
			MayBeEmptyString arg;

			if( l[0]=='#' )
				continue;
			else if(!!(arg = striKWcmp( l, "Broker_URL=" ))){
				Broker_URL = arg.c_str();
				if(verbose)
					SelLog->Log('C', "Broker_URL : '%s'", Broker_URL.c_str());
			} else if(!!(arg = striKWcmp( l, "ClientID=" ))){
				MQTT_ClientID = arg.c_str();
				if(verbose)
					SelLog->Log('C', "Client ID : '%s'", MQTT_ClientID.c_str());
			} else if(!!(arg = striKWcmp( l, "UserConfiguration=" ))){
				UserConfigRoot = arg.c_str();
				if(verbose)
					SelLog->Log('C', "User configuration directory : '%s'", UserConfigRoot.c_str());
			} 
		}
	} catch(const std::ifstream::failure &e){
		if(!file.eof()){
			SelLog->Log('F', "%s : %s", fch, strerror(errno));
			exit(EXIT_FAILURE);
		}
	}

	file.close();

	if(MQTT_ClientID.empty()){
		char h[HOST_NAME_MAX];
		if(gethostname( h, HOST_NAME_MAX )){
			SelLog->Log('F', "gethostname() : %s", strerror(errno));
			exit(EXIT_FAILURE);
		}
		char l[strlen(h) + 20];
		sprintf(l, "Majordome-%s-%u", h, getpid());
		MQTT_ClientID = l;
		if(verbose)
			SelLog->Log('C', "MQTT Client ID : '%s' (computed)", MQTT_ClientID.c_str());
	}
}

	/******
	 * technical objects
	 *******/
pthread_attr_t thread_attr;

	/******
	 * MQTT's
	 *******/

MQTTClient MQTT_client;

/* handle message arrival and call associated function.
 * NOTE : up to now, only textual topics & messages are
 * correctly handled (lengths are simply ignored)
 *
 * Take in account ONLY the first enabled topic matching 
 * the receiving one, even if all tasks are disabled or
 * already running.
 */
static int msgarrived(void *actx, char *topic, int tlen, MQTTClient_message *msg){
	if(verbose && !hideTopicArrival)
		SelLog->Log('T', "Receiving '%s'", topic);

		// Convert the payload to a string
	char cpayload[msg->payloadlen + 1];
	memcpy(cpayload, msg->payload, msg->payloadlen);
	cpayload[msg->payloadlen] = 0;

	MQTTClient_freeMessage(&msg);
	MQTTClient_free(topic);
	return 1;
}

static void connlost(void *ctx, char *cause){
	SelLog->Log('F', "Broker connection lost due to %s", cause);
	exit(EXIT_FAILURE);
}

static void brkcleaning(void){	/* Clean broker stuffs */
	MQTTClient_disconnect(MQTT_client, 10000);	/* 10s for the grace period */
	MQTTClient_destroy(&MQTT_client);
}

	/******
	 * Main loop
	 *******/
int main(int ac, char **av){
	initSelene();							// Load Séléné modules
	SelLog->configure(NULL, LOG_STDOUT);	// Early logging to STDOUT before broker initialisation

		/* Read arguments */
	char c;
	const char *conf_file = DEFAULT_CONFIGURATION_FILE;

	while((c = getopt(ac, av, "qvVdhrf:t")) != EOF) switch(c){
	case 'h':
		fprintf(stderr, "%s (%.04f)\n"
			"A lightweight event based Automation System\n"
			"%s\n"
			"Known options are :\n"
			"\t-h : this online help\n"
			"\t-q : be quiet (remove all messages but script generated one)\n"
			"\t-v : enable verbose messages (overwrite -q)\n"
			"\t-r : enable trace messages\n"
			"\t-V : silence topic arrival\n"
#ifdef DEBUG
			"\t-d : enable debug messages (overwrite -q)\n"
#endif
			"\t-f<file> : read <file> for configuration\n"
			"\t\t(default is '%s')\n"
			"\t-t : test configuration file and exit\n",
			basename(av[0]), VERSION, COPYRIGHT, DEFAULT_CONFIGURATION_FILE
		);
		exit(EXIT_FAILURE);
		break;
	case 't':
		configtest = true;
	case 'v':
		SelLog->Log('I', "%s v%.04f", basename(av[0]), VERSION);
		verbose = true;
		quiet = false;
		break;
	case 'r':
		trace = true;
		break;
	case 'V':
		hideTopicArrival = true;
		break;
	case 'q':
		if(!verbose)
			quiet = true;
		break;
#ifdef DEBUG
	case 'd':
		debug = true;
		quiet = false;
		break;
#endif
	case 'f':
		conf_file = optarg;
		break;
	default:
		SelLog->Log('F', "Unknown option '%c'\n%s -h\n\tfor some help\n", c, av[0]);
		exit(EXIT_FAILURE);
	}
	read_configuration(conf_file);

		/***
		 * Initial technical objects
		 ***/

		// Creates threads as detached in order to save
		// some resources when quiting
	assert(!pthread_attr_init (&thread_attr));
	assert(!pthread_attr_setdetachstate (&thread_attr, PTHREAD_CREATE_DETACHED));

		/***
		 * Connecting to the broker
		 ***/
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
	conn_opts.reliable = 0;	/* Asynchronous sending */
	int err;

	if( (err = MQTTClient_create( &MQTT_client, Broker_URL.c_str(), MQTT_ClientID.c_str(), MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS ){
		SelLog->Log('F', "Can't connect to broker due to error %d", err);
		exit(EXIT_FAILURE);
	}

	MQTTClient_setCallbacks( MQTT_client, NULL, connlost, msgarrived, NULL);

	switch( err = MQTTClient_connect( MQTT_client, &conn_opts) ){
	case MQTTCLIENT_SUCCESS : 
		break;
	case 1 : SelLog->Log('F', "Unable to connect : Unacceptable protocol version");
		exit(EXIT_FAILURE);
	case 2 : SelLog->Log('F', "Unable to connect : Identifier rejected");
		exit(EXIT_FAILURE);
	case 3 : SelLog->Log('F', "Unable to connect : Server unavailable");
		exit(EXIT_FAILURE);
	case 4 : SelLog->Log('F', "Unable to connect : Bad user name or password");
		exit(EXIT_FAILURE);
	case 5 : SelLog->Log('F', "Unable to connect : Not authorized");
		exit(EXIT_FAILURE);
	default :
		if( !MQTTClient_isConnected( MQTT_client ) ){
			SelLog->Log('F', "Unable to connect (unknown error : %d)", err);
			exit(EXIT_FAILURE);
		} else
			SelLog->Log('W', "Connected but got an unknown error : %d)", err);
	}
	atexit(brkcleaning);

	if(!quiet)
		SelLog->Log('I', "Starting %s %f ...", basename(av[0]), VERSION);

		/***
		 * Reading user configuration 
		 ****/
	config.init(UserConfigRoot.c_str(), SelLua->getLuaState());	// Read user's configuration files

	if(configtest){
		SelLog->Log('E', "Testing only the configuration ... leaving.");
		exit(EXIT_FAILURE);
	}

	if(!quiet)
		SelLog->Log('I', "Let's go ...");
}
