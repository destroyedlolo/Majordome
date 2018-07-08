/* Helpers.h
 * 	Helpers functions
 *
 * 	06/07/2018 - LF - First version
 */

#ifndef HELPERS_H
#include <stdbool.h>

#define MAXLINE 1024	/* Maximum length of a line to be read */
extern bool verbose;
extern MQTTClient MQTT_client;
extern const char *ClientID;

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
