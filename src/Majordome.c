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
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <libgen.h>

#include <stdbool.h>

#define VERSION "0.1"
#define DEFAULT_CONFIGURATION_FILE "/usr/local/etc/Majordome.conf"

	/* Local configuration */
bool verbose = false;
bool configtest = false;

	/* Logging */
void publishLog( char l, const char *msg, ...){
	va_list args;
	va_start(args, msg);

#if 0
	if(verbose || l=='E' || l=='F'){
		char t[ strlen(msg) + 7 ];
		sprintf(t, "*%c* %s\n", l, msg);
		vfprintf((l=='E' || l=='F')? stderr : stdout, t, args);
	}

	if(cfg.client){
		char *sub;
		switch(l){
		case 'F':
			sub = "/Log/Fatal";
			break;
		case 'E':
			sub = "/Log/Error";
			break;
		case 'W':
			sub = "/Log/Warning";
			break;
		default :
			sub = "/Log/Information";
		}

		char tmsg[1024];	/* No simple way here to know the message size */
		char ttopic[ strlen(cfg.ClientID) + strlen(sub) + 1 ];
		sprintf(ttopic, "%s%s", cfg.ClientID, sub);
		vsnprintf(tmsg, sizeof(tmsg), msg, args);

		mqttpublish( cfg.client, ttopic, strlen(tmsg), tmsg, 0);
	}
	va_end(args);
#endif
}

	/*
	 * Helpers
	 */
char *removeLF(char *s){
	size_t l=strlen(s);
	if(l && s[--l] == '\n')
		s[l] = 0;
	return s;
}

char *striKWcmp( char *s, const char *kw ){
/* compare string s against kw
 * Return :
 * 	- remaining string if the keyword matches
 * 	- NULL if the keyword is not found
 */
	size_t klen = strlen(kw);
	if( strncasecmp(s,kw,klen) )
		return NULL;
	else
		return s+klen;
}

char *stradd(char *p, const char *s, bool addspace){
	/* Enlarge string pointed by p to add s
	 * if !p, start a new string
	 *
	 * if addspace == true, the 1st char is
	 * skipped if !p
	 */
	if(!p)
		return(strdup(s + (addspace ? 1:0)));

	size_t asz = strlen(p);
	char *np = (char *)realloc(p, strlen(p) + strlen(s) +1);
	assert(np);
	strcpy( np+asz, s );

	return(np);
}

int main( int ac, char **av){
	const char *conf_file = DEFAULT_CONFIGURATION_FILE;
	int c;

	while((c = getopt(ac, av, "vhf:t")) != EOF) switch(c){
	case 'h':
		fprintf(stderr, "%s (%s)\n"
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
		printf("%s v%s\n", basename(av[0]), VERSION);
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

}
