/* MayBeEmptyString
 *
 * Extend string to allow emptyness
 * It's only a workaround : emptiness is set only by the constructor or
 * explicitly.
 * string's operator ARE NOT TOOK IN ACCOUNT.
 */
#ifndef MAYBEEMPTYSTRING_H
#define MAYBEEMPTYSTRING_H

#include <string>

class MayBeEmptyString : public std::string {
	bool empty;

public:
	MayBeEmptyString() : empty(true) {}
	MayBeEmptyString( std::string s ) : empty(false) { std::string::operator=(s); }
	MayBeEmptyString( const char *s ) : empty(false) { std::string::operator=(s); }

	bool operator! (){ return this->empty; }
	bool isEmpty(void){ return this->empty; }
	void Empty(void){ this->empty = true; }
	void Set(void){ this->empty = false; }
};

#endif
