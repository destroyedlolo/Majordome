/*
 * Majordome
 * 	An event based Automation System
 *
 * Copyright 2018 Laurent Faillie
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
 *	05/07/2018 - LF - Start of development
 *	25/07/2018 - LF - Switch to C++ (when it's useful)
 */

#include <iostream>
#include <fstream>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>

#include <unistd.h> 	// getopt()
#include <errno.h>

#include <libSelene.h>

#include "Components.h"
#include "Config.h"

#define VERSION 0.0201
#define DEFAULT_CONFIGURATION_FILE "/usr/local/etc/Majordome.conf"

using namespace std;

	/*****
	 * global configuration
	 *****/

bool verbose = false;
const char *MQTT_ClientID = NULL;	/* MQTT client id : must be unique among a broker's clients */
void *luainitfunc;

Config config;

	/******
	 * local configuration
	 *******/

static bool configtest = false;
static const char *UserConfigRoot = "/usr/local/etc/Majordome";	/* Where to find user configuration */
static const char *Broker_URL = "tcp://localhost:1883";		/* Broker's URL */

static void read_configuration( const char *fch){
	std::ifstream file;
	file.exceptions ( std::ios::eofbit | std::ios::failbit ); // No need to check failbit
	try {
		std::string l;

		file.open(fch);
		while( std::getline( file, l) ){
			MayBeEmptyString arg;

			if( l[0]=='#' )
				continue;
			else if( !!(arg = striKWcmp( l, "Broker_URL=" ))){
				assert(( Broker_URL = strdup( arg.c_str() ) ));
				if(verbose)
					printf("\tBroker_URL : '%s'\n", Broker_URL);
			} else if( !!(arg = striKWcmp( l, "ClientID=" ))){
				assert(( MQTT_ClientID = strdup( arg.c_str() ) ));
				if(verbose)
					printf("\tClient ID : '%s'\n", MQTT_ClientID);
			} else if( !!(arg = striKWcmp( l, "UserConfiguration=" ))){
				assert(( UserConfigRoot = strdup( arg.c_str() ) ));
				if(verbose)
					printf("\tUser configuration directory : '%s'\n", UserConfigRoot);
			} 
		}
	} catch(const std::ifstream::failure &e){
		if(!file.eof()){
			publishLog('F', "%s : %s", fch, strerror(errno) );
			exit(EXIT_FAILURE);
		}
	}

	file.close();

	if(verbose)
		puts("");

	if(!MQTT_ClientID){
		char h[HOST_NAME_MAX];
		if(gethostname( h, HOST_NAME_MAX )){
			publishLog('F', "gethostname() : %s", strerror( errno ));
			exit(EXIT_FAILURE);
		}
		char l[ strlen(h) + 20 ];
		sprintf(l, "Majordome-%s-%u", h, getpid());
		assert(( MQTT_ClientID = strdup( l ) ));
		if(verbose)
			printf("MQTT Client ID : '%s' (computed)\n", MQTT_ClientID);
	}
}

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
#ifdef DEBUG
	publishLog('D', "Receiving '%s'", topic);
#endif

	for(Config::TopicElements::iterator i = config.TopicsList.begin(); i != config.TopicsList.end(); i++){
		if( i->second.match( topic ) ){
#ifdef DEBUG
			publishLog('D', "Accepted by '%s'", i->second.getNameC() );
#endif
			break;
		}
	}

	MQTTClient_freeMessage(&msg);
	MQTTClient_free(topic);
	return 1;
}

static void connlost(void *ctx, char *cause){
	publishLog('W', "Broker_URL connection lost due to %s", cause);
	exit(EXIT_FAILURE);
}

static void brkcleaning(void){	/* Clean broker stuffs */
	MQTTClient_disconnect(MQTT_client, 10000);	/* 10s for the grace period */
	MQTTClient_destroy(&MQTT_client);
}



	/******
	 * Main loop
	 *******/
static int setGlobalVar( lua_State *L ){
	lua_pushnumber( L, VERSION );	/* Expose version to lua side */
	lua_setglobal( L, "MAJORDOME_VERSION" );

	lua_pushstring( L, MQTT_ClientID );	/* Expose ClientID to lua side */
	lua_setglobal( L, "MAJORDOME_ClientID" );

	return 0;
}

int main(int ac, char **av){
	const char *conf_file = DEFAULT_CONFIGURATION_FILE;
	int c;

	slc_init( NULL, LOG_STDOUT );	/* Early logging to STDOUT before broker initialisation*/

	while((c = getopt(ac, av, "vhf:t")) != EOF) switch(c){
	case 'h':
		fprintf(stderr, "%s (%.04f)\n"
			"A lightweight event based Automation System\n"
			"Known options are :\n"
			"\t-h : this online help\n"
			"\t-v : enable verbose messages\n"
			"\t-f<file> : read <file> for configuration\n"
			"\t\t(default is '%s')\n"
			"\t-t : test configuration file and exit\n",
			basename(av[0]), VERSION, DEFAULT_CONFIGURATION_FILE
		);
		exit(EXIT_FAILURE);
		break;
	case 't':
		configtest = true;
		printf("%s v%.04f\n", basename(av[0]), VERSION);
	case 'v':
		verbose = true;
		break;
	case 'f':
		conf_file = optarg;
		break;
	default:
		publishLog('F', "Unknown option '%c'\n%s -h\n\tfor some help\n", c, av[0]);
		exit(EXIT_FAILURE);
	}
	read_configuration( conf_file );

	if(configtest){
		publishLog('E', "Testing only the configuration ... leaving.");
		exit(EXIT_FAILURE);
	}

		/***
		 * Connecting to the broker
		 ***/
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
	conn_opts.reliable = 0;	/* Asynchronous sending */
	int err;

	if( (err = MQTTClient_create( &MQTT_client, Broker_URL, MQTT_ClientID, MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS ){
		fprintf(stderr, "*F* can't connect to broker due to error %d\n", err);
		exit(EXIT_FAILURE);
	}

	MQTTClient_setCallbacks( MQTT_client, NULL, connlost, msgarrived, NULL);

	switch( err = MQTTClient_connect( MQTT_client, &conn_opts) ){
	case MQTTCLIENT_SUCCESS : 
		break;
	case 1 : publishLog('F', "Unable to connect : Unacceptable protocol version");
		exit(EXIT_FAILURE);
	case 2 : publishLog('F', "Unable to connect : Identifier rejected");
		exit(EXIT_FAILURE);
	case 3 : publishLog('F', "Unable to connect : Server unavailable");
		exit(EXIT_FAILURE);
	case 4 : publishLog('F', "Unable to connect : Bad user name or password");
		exit(EXIT_FAILURE);
	case 5 : publishLog('F', "Unable to connect : Not authorized");
		exit(EXIT_FAILURE);
	default :
		if( !MQTTClient_isConnected( MQTT_client ) ){
			publishLog('F', "Unable to connect (unknown error : %d)", err);
			exit(EXIT_FAILURE);
		} else
			publishLog('W', "Connected but got an unknown error : %d)", err);
	}
	atexit(brkcleaning);

		/***
		 * Lua's stuffs
		 ***/
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	initSeleneLibrary(L);

	slc_initMQTT( MQTT_client, MQTT_ClientID );	// Initialize MQTT logging
	semc_initializeSeleMQTT( MQTT_client, MQTT_ClientID );	// Initialize SeleMQTT

	luainitfunc = libSel_AddStartupFunc( NULL, setGlobalVar );
	luainitfunc = libSel_AddStartupFunc( luainitfunc, initSelShared );
	luainitfunc = libSel_AddStartupFunc( luainitfunc, initSelLog );
	luainitfunc = libSel_AddStartupFunc( luainitfunc, initSeleMQTT );

	libSel_ApplyStartupFunc( luainitfunc, L );

	if(verbose)
		publishLog('I', "Starting %s %f ...", basename(av[0]), VERSION);

		/***
		 * Reading user configuration 
		 ****/
	config.init( UserConfigRoot, L );

	pause();	// Waiting for events, nothing else to do
}

