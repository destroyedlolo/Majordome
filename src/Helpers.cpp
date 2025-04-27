/* Helpers.c
 * 	Helper functions
 */
#include "Helpers.h"

#include <cstring>
#include <cstdarg>
#include <cstdio>

char *removeLF(char *s){
	size_t l=strlen(s);
	if(l && s[--l] == '\n')
		s[l] = 0;
	return s;
}

const char *striKWcmp(const char *s, const char *kw){
	size_t klen = strlen(kw);
	if( strncasecmp(s,kw,klen) )
		return NULL;
	else
		return s+klen;
}

std::string striKWcmp(const std::string s, const char *kw){
	size_t l = strlen( kw );

	if(!s.compare(0, l, kw))
		return std::string( s.substr(l) );
	return std::string();
}

std::string striKWcmp(const std::string &s, const std::string &kw ){
	if(!s.compare(0, kw.size(), kw))
		return std::string( s.substr(kw.size()) );
	return std::string();
}

const char *fileextention( const char *fch ){
	return strrchr(fch, '.');
}

