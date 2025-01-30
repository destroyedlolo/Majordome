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
#include "Helpers.h"
#include "MayBeEmptyString.h"
#include "Config.h"

#include <fstream>

#include <cstring>		// strerror()
#include <cassert>		// Dirty exits for situations not expected to arrive
#include <csignal>
#include <unistd.h> 	// getopt()
#include <libgen.h>		// basename()

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
Config config;			// Application's configuration

	/* *****
	 * local configuration
	 * (feed with default values)
	 * ******/
std::string MQTT_ClientID;
static std::string UserConfigRoot("/usr/local/etc/Majordome");	// Where to find user configuration
static std::string Broker_URL("tcp://localhost:1883");	// Broker to contact

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
			} else if(!!(arg = striKWcmp( l, "UserConfiguration=" )) || !!(arg = striKWcmp( l, "ApplicationDirectory=" ))){
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


	/* *****
	 * Threading
	 * ******/

pthread_attr_t thread_attr;

/* Set the Lua environment of each newly created threads
 * (including the main one)
 */

void threadEnvironment(lua_State *L){
	lua_pushnumber( L, VERSION );	/* Expose version to lua side */
	lua_setglobal( L, "MAJORDOME_VERSION" );

	lua_pushstring( L, COPYRIGHT );	/* Expose copyright to lua side */
	lua_setglobal( L, "MAJORDOME_COPYRIGHT" );

#ifdef TOILE
	lua_pushnumber( L, TOILEVERSION );	/* Expose version to lua side */
	lua_setglobal( L, "TOILE_VERSION" );
#endif

	lua_pushstring( L, MQTT_ClientID.c_str() );	/* Expose ClientID to lua side */
	lua_setglobal( L, "MAJORDOME_ClientID" );

	lua_pushstring( L, config.getConfigDir().c_str() );
	lua_setglobal( L, "MAJORDOME_CONFIGURATION_DIRECTORY" );

	if(verbose){
		lua_pushinteger( L, 1 );
		lua_setglobal( L, "MAJORDOME_VERBOSE" );
	}

#if DEBUG
	if(debug){
		lua_pushinteger( L, 1 );
		lua_setglobal( L, "MAJORDOME_DEBUG" );
	}
#endif

	SelLua->ApplyStartupFunc(L);	// Creates metas
	SelMQTT->createExternallyManaged(L, MQTT_client);	// Push object on the task
	lua_setglobal( L, "MQTTBroker" );	// expose it as the broker
}


	/* ***
	 * Majordome's Lua own API
	 */

static int mjd_letsgo(lua_State *L){
	SelLog->Log('D', "Late dependencies building");
	
	SelLua->lateBuildingDependancies(L);
	SelLua->ApplyStartupFunc(L);

	SelLog->Log('D', "Let's go ...");

	return 0;
}

static const struct luaL_Reg MajordomeLib [] = {
	{"LetsGo", mjd_letsgo},
	{NULL, NULL}
};

static void initMajordomeObject( lua_State *L ){
	SelLua->libCreateOrAddFuncs( L, "Majordome", MajordomeLib );
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
	if(verbose && !hideTopicArrival)
		SelLog->Log('T', "Receiving '%s'", topic);

		// Convert the payload to a string
	char cpayload[msg->payloadlen + 1];
	memcpy(cpayload, msg->payload, msg->payloadlen);
	cpayload[msg->payloadlen] = 0;

/* TODO message handling */
	for(auto &i : config.TopicsList){
		if(i.second->isEnabled()){
			if(i.second->match( topic )){
#ifdef DEBUG
				if( debug && !i.second->isQuiet() ){
					if(hideTopicArrival)
						SelLog->Log('D', "'%s' accepted by topic '%s'", topic, i.second->getNameC() );
					else
						SelLog->Log('D', "Accepted by topic '%s'", i.second->getNameC() );
				}
#endif

				if( i.second->toBeStored() ){	// Store it in a SharedVar
					if( i.second->isNumeric() ){
						try {
							double val = std::stod( cpayload );
							SelSharedVar->setNumber( i.second->getNameC(), val, i.second->getTTL() );
						} catch( ... ){
							SelLog->Log('E', "Topic '%s' is expecting a number : no convertion done ", i.second->getNameC() );
							SelSharedVar->clear( i.second->getNameC() );
						}
					} else
						SelSharedVar->setString( i.second->getNameC(), cpayload, i.second->getTTL() );
				}
			}

			i.second->execHandlers(*i.second, topic, cpayload);
		}
	}

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

	/* *****
	 * Main loop
	 * ******/

void quit(int){
	exit(EXIT_SUCCESS);
}

void bye(void){
	/* ToDo */
//	config.RunShutdowns();
}

int main(int ac, char **av){
	initSelene();							// Load Séléné modules
	SelLog->configure(NULL, LOG_STDOUT);	// Early logging to STDOUT before broker initialisation

		/* Read arguments */
	char c;
	const char *conf_file = DEFAULT_CONFIGURATION_FILE;

	while((c = getopt(ac, av, "qvVdhrf:t")) != (char)EOF) switch(c){
	case 'h':
		fprintf(stderr, "%s (%.04f%s)\n"
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
			basename(av[0]), VERSION,
#ifdef TOILE
#define STRIFY_HELPER(s) #s
#define STRIFY(x) STRIFY_HELPER(x)
			" - Toile " STRIFY(TOILEVERSION),
#else
			"",
#endif
			COPYRIGHT, DEFAULT_CONFIGURATION_FILE
		);
		exit(EXIT_FAILURE);
		break;
	case 't':
		configtest = true;
	case 'v':
#ifdef TOILE
		SelLog->Log('I', "%s v%.04f %s", basename(av[0]), VERSION, " - Toile v" STRIFY(TOILEVERSION));
#else
		SelLog->Log('I', "%s v%.04f", basename(av[0]), VERSION);
#endif
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

	if(!trace)
		SelLog->ignoreList("T");	// Disabling trace logging

	read_configuration(conf_file);

		/* **
		 * Initial technical objects
		 * **/

		// Creates threads as detached in order to save
		// some resources when quiting
	assert(!pthread_attr_init (&thread_attr));
	assert(!pthread_attr_setdetachstate (&thread_attr, PTHREAD_CREATE_DETACHED));

		/* **
		 * Connecting to the broker
		 * **/
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
	SelLog->initMQTT(MQTT_client, MQTT_ClientID.c_str());	// Initialize MQTT logging

		/* **
		 * Reading user configuration 
		 * ***/
	if(!quiet)
		SelLog->Log('I', "Application code for %s %f ...", basename(av[0]), VERSION);

	threadEnvironment(SelLua->getLuaState());
	config.init(UserConfigRoot, SelLua->getLuaState());	// Read user's configuration files
	config.SanityChecks();

	if(configtest){
		SelLog->Log('E', "Testing only the configuration ... leaving.");
		exit(EXIT_FAILURE);
	}

		/* **
		 * Add Majordom's own objects to slave thread
		 * ***/
	SelLua->AddStartupFunc(initMajordomeObject);
	SelLua->AddStartupFunc(LuaTask::initLuaInterface);
	SelLua->AddStartupFunc(Event::initLuaInterface);
	SelLua->AddStartupFunc(MQTTTopic::initLuaInterface);
	SelLua->AddStartupFunc(Timer::initLuaInterface);
	SelLua->AddStartupFunc(Timer::initLuaInterface);
	SelLua->AddStartupFunc(MinMax::initLuaInterface);

		/* **
		 * After this point, we're running application's code
		 * **/

	if(!quiet)
		SelLog->Log('I', "Application starting ...");

	config.RunStartups();	// Run startup functions
	config.SubscribeTopics();	// MQTT : activate topics receiving
	config.LaunchTimers();	// Launch slave timers
	config.RunImmediates();	// Run immediate & overdue timers tasks

		/* Shutdown's */
	signal(SIGINT,quit);
	signal(SIGUSR1,quit);
	atexit(bye);

#ifdef TOILE
	Toile::RefreshRenderers();
#endif

	pause();	// Waiting for events, nothing else to do
}
