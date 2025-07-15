/* Helpers.c
 * 	Helper functions
 */
#include "Helpers.h"

#include <cstring>
#include <cstdarg>

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

std::string escapeHTML(std::string data) {
	std::string buffer;
	buffer.reserve(data.size());
	for(size_t pos = 0; pos != data.size(); ++pos) {
		switch(data[pos]) {
		case '&':
			buffer.append("&amp;"); break;
		case '\"':
			buffer.append("&quot;"); break;
		case '\'':
			buffer.append("&apos;"); break;
		case '<':
			buffer.append("&lt;"); break;
		case '>':
			buffer.append("&gt;"); break;
		default:
			buffer.append(&data[pos], 1); break;
		}
	}

	return buffer;
}

