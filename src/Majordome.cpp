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

}
