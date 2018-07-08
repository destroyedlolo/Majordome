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
 *	05/07/2018	- LF - Start of development
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <libgen.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>

#include <MQTTClient.h> /* PAHO library needed */ 

#include "Helpers.h"

#define VERSION 0.01
#define DEFAULT_CONFIGURATION_FILE "/usr/local/etc/Majordome.conf"

	/* global configuration */
bool verbose = false;
const char *UserConfigRoot = "/usr/local/etc/Majordome";	/* Where to find user configuration */

	/* local configuration */
static bool configtest = false;
static const char *Broker = "tcp://localhost:1883";		/* Broker's URL */
const char *ClientID = NULL;	/* MQTT client id : must be unique among a broker's clients */

static void read_configuration( const char *fch){ FILE *f; char l[MAXLINE];
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
	
		if((arg = striKWcmp(l,"Broker="))){
			assert(( Broker = strdup( removeLF(arg) ) ));
			if(verbose)
				printf("Broker : '%s'\n", Broker);
		} else if((arg = striKWcmp(l,"ClientID="))){
			assert(( ClientID = strdup( removeLF(arg) ) ));
			if(verbose)
				printf("MQTT Client ID : '%s'\n", ClientID);
		} else if((arg = striKWcmp(l,"UserConfiguration="))){
			assert(( UserConfigRoot = strdup( removeLF(arg) ) ));
			if(verbose)
				printf("User configuration directory : '%s'\n", UserConfigRoot);
		}
	}
	fclose(f);

	if(!ClientID){
		char h[HOST_NAME_MAX];
		if(gethostname( h, HOST_NAME_MAX )){
			publishLog('F', "gethostname() : %s", strerror( errno ));
			exit(EXIT_FAILURE);
		}
		sprintf(l, "Majordome-%s-%u", h, getpid());
		assert(( ClientID = strdup( l ) ));
		if(verbose)
			printf("MQTT Client ID : '%s' (computed)\n", ClientID);
	}
}

	/* MQTT's */
MQTTClient MQTT_client;

static int msgarrived(void *actx, char *topic, int tlen, MQTTClient_message *msg){
	MQTTClient_freeMessage(&msg);
	MQTTClient_free(topic);
	return 1;
}

static void connlost(void *ctx, char *cause){
	publishLog('W', "Broker connection lost due to %s", cause);
	exit(EXIT_FAILURE);
}

static void brkcleaning(void){	/* Clean broker stuffs */
	MQTTClient_disconnect(MQTT_client, 10000);	/* 10s for the grace period */
	MQTTClient_destroy(&MQTT_client);
}

int main( int ac, char **av){
	const char *conf_file = DEFAULT_CONFIGURATION_FILE;
	int c;

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
		publishLog('W', "Testing only the configuration ... leaving.");
		exit(EXIT_FAILURE);
	}

		/* Connecting to the broker */
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
	conn_opts.reliable = 0;	/* Asynchronous sending */

	MQTTClient_create( &MQTT_client, Broker, ClientID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
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

}
