/* Components.h
 * 	functions spanned across sources files.
 *
 * 	06/07/2018 - LF - First version
 */

#ifndef COMPONENTS_H
#include <stdbool.h>

	/****
	 * Configuration's
	 ****/
#define MAXLINE 1024	/* Maximum length of a line to be read */
extern bool verbose;
extern MQTTClient MQTT_client;
extern const char *ClientID;


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

/* Log messages (in printf() format) into both
 * stdout/err and in topics.
 * 
 * -> l : level of the message [F]atal, [E]rror, [W]arning or info
 * 	msg : the message to log + arguments
 */
extern void publishLog( char l, const char *msg, ...);

#endif
