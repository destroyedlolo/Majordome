/* Helpers.c
 * 	Helper functions
 */
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "MQTT_tools.h"
#include "Helpers.h"

char *removeLF(char *s){
	size_t l=strlen(s);
	if(l && s[--l] == '\n')
		s[l] = 0;
	return s;
}

char *striKWcmp( char *s, const char *kw ){
	size_t klen = strlen(kw);
	if( strncasecmp(s,kw,klen) )
		return NULL;
	else
		return s+klen;
}

void publishLog( char l, const char *msg, ...){
	va_list args;
	va_start(args, msg);

	if(verbose || l=='E' || l=='F'){
		char t[ strlen(msg) + 7 ];
		sprintf(t, "*%c* %s\n", l, msg);
		vfprintf((l=='E' || l=='F')? stderr : stdout, t, args);
	}

	if(MQTT_client){
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
		char ttopic[ strlen(ClientID) + strlen(sub) + 1 ];
		sprintf(ttopic, "%s%s",ClientID, sub);
		vsnprintf(tmsg, sizeof(tmsg), msg, args);

		mqttpublish( MQTT_client, ttopic, strlen(tmsg), tmsg, 0);
	}
	va_end(args);
}

