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

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>

#include <unistd.h> 	// getopt()
#include <errno.h>

#if 0
#include <libgen.h>
#include <stdbool.h>
#endif

#include <libSelene.h>

#include "Components.h"

#define VERSION 0.02
#define DEFAULT_CONFIGURATION_FILE "/usr/local/etc/Majordome.conf"

using namespace std;

	/*****
	 * global configuration
	 *****/

bool verbose = false;
const char *MQTT_ClientID = NULL;	/* MQTT client id : must be unique among a broker's clients */
void *luainitfunc;

	/******
	 * local configuration
	 *******/

static bool configtest = false;
static const char *UserConfigRoot = "/usr/local/etc/Majordome";	/* Where to find user configuration */
static const char *Broker_URL = "tcp://localhost:1883";		/* Broker's URL */

/* Note : C++ stream exception handling is really brain damaged.
 * well, such simple task is done in the old C way !
 * (will see latter if it can switch to a C++ without headaches)
 */
static void read_configuration( const char *fch){
	FILE *f;
	char l[MAXLINE];
	char *arg;

	if(verbose)
		printf("\nReading configuration file '%s'\n---------------------------\n", fch);

	if(!(f=fopen(fch, "r"))){
		publishLog('F', "%s : %s", fch, strerror( errno ));
		exit(EXIT_FAILURE);
	}

	while(fgets(l, MAXLINE, f)){
		if(*l == '#' || *l == '\n')
			continue;
	
		if((arg = striKWcmp(l,"Broker_URL="))){
			assert(( Broker_URL = strdup( removeLF(arg) ) ));
			if(verbose)
				printf("\tBroker_URL : '%s'\n", Broker_URL);
		} else if((arg = striKWcmp(l,"ClientID="))){
			assert(( MQTT_ClientID = strdup( removeLF(arg) ) ));
			if(verbose)
				printf("\tMQTT Client ID : '%s'\n", MQTT_ClientID);
		} else if((arg = striKWcmp(l,"UserConfiguration="))){
			assert(( UserConfigRoot = strdup( removeLF(arg) ) ));
			if(verbose)
				printf("\tUser configuration directory : '%s'\n", UserConfigRoot);
		}
	}
	fclose(f);

	if(verbose)
		puts("");

	if(!MQTT_ClientID){
		char h[HOST_NAME_MAX];
		if(gethostname( h, HOST_NAME_MAX )){
			publishLog('F', "gethostname() : %s", strerror( errno ));
			exit(EXIT_FAILURE);
		}
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

static int msgarrived(void *actx, char *topic, int tlen, MQTTClient_message *msg){
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

	switch( MQTTClient_connect( MQTT_client, &conn_opts) ){
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
		publishLog('F', "Unable to connect");
		exit(EXIT_FAILURE);
	}
	atexit(brkcleaning);

		/***
		 * Lua's stuffs
		 ***/
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	initSeleneLibrary(L);

	slc_initMQTT( MQTT_client, MQTT_ClientID );	// Initialize MQTT logging
	semc_initializeSeleMQTT( &MQTT_client, MQTT_ClientID );	// Initialize SeleMQTT

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
	readUserConfig( UserConfigRoot, L );
}

