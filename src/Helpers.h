/* Helpers.h
 * 	helpers functions spanned across sources files.
 *
 * 	06/07/2018 - LF - First version
 * 	25/07/2018 - LF - switch to C++
 */

#ifndef HELPERS_H
#define HELPERS_H

#include <MQTTClient.h> /* PAHO library needed */
#include <lua.h>	/* Lua's state needed */

#include "MayBeEmptyString.h"

	/****
	 * Configuration's
	 ****/
extern bool verbose;
extern bool debug;
extern bool configtest;
extern MQTTClient MQTT_client;
extern const char *ClientID;

extern void *luainitfunc;

	/******
	 * technical objects
	 *******/
extern pthread_attr_t thread_attr;

	/****
	 * Helpers
	 ****/

/* Removes the last character if it's an "\n" */
extern char *removeLF(char *);

/* compares string s against kw keywords
 * Return :
 * 	- remaining string if the keyword matches
 * 	- NULL if the keyword is not found
 */
extern char *striKWcmp( char *s, const char *kw );
extern MayBeEmptyString striKWcmp( std::string s, const char *kw );
extern MayBeEmptyString striKWcmp( std::string s, std::string &kw );

/* Log messages (in printf() format) into both
 * stdout/err and in topics.
 * 
 * -> l : level of the message [F]atal, [E]rror, [W]arning or info
 * 	msg : the message to log + arguments
 */
extern void publishLog( char l, const char *msg, ...);

/* Return the extention of a given file
 * -> filename
 * <- pointer to the last '.' or NULL if not found
 */
extern const char *fileextention( const char *fch );

#endif
