/* Helpers.c
 * 	Helper functions
 */
#include <cstring>
#include <cstdarg>
#include <cstdio>

#include <libSelene.h>
#include "Components.h"

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

	char tmsg[1024];	/* No simple way here to know the message size */
	vsnprintf(tmsg, sizeof(tmsg), msg, args);

	slc_log( l, tmsg );

	va_end(args);
}

const char *fileextention( const char *fch ){
	return strrchr(fch, '.');
}

