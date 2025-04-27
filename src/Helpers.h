/* Helpers.h
 * 	helpers functions spanned across sources files and global
 *	configuration.
 *
 * 	06/07/2018 - LF - First version
 * 	25/07/2018 - LF - switch to C++
 *	26/04/2025 - LF - Remove MayBeEmptyString
 */

#ifndef HELPERS_H
#define HELPERS_H

#include <MQTTClient.h> /* PAHO library needed */
#include <lua.hpp>	/* Lua's state needed */

#include <string>

	/* ***
	 * Configuration's
	 * ***/
extern bool quiet;
extern bool hideTopicArrival;
extern bool verbose;
extern bool debug;
extern bool trace;
extern bool configtest;


	/* ***
	 * MQTT
	 * ***/

extern MQTTClient MQTT_client;
extern std::string MQTT_ClientID;


	/* ***
	 * Lua
	 * ***/

extern void *luainitfunc;
extern void threadEnvironment(lua_State *);

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
extern const char *striKWcmp( const char *s, const char *kw );
extern std::string striKWcmp( const std::string s, const char *kw );
extern std::string striKWcmp( const std::string s, const std::string &kw );

/* Return the extention of a given file
 * -> filename
 * <- pointer to the last '.' or NULL if not found
 */
extern const char *fileextention( const char *fch );

#endif
