/* Generic class for objects
 *
 * 27/07/2018 - LF - First version
 */

#ifndef OBJECT_H
#define OBJECT_H

#include <string>

class Object {
	bool disabled;
	bool quiet;

protected:
	std::string name;
	std::string where;

public:
	Object() : disabled(false), quiet(false){};

	void enable( void ){ this->disabled = false; };
	void disable( void ){ this->disabled = true; };
	bool isEnabled( void ){ return !this->disabled; };

		// Remove some messages
	void beQuiet( void ){ this->quiet = true; };
	void beNoisy( void ){ this->quiet = false; };
	bool isQuiet( void ){ return this->quiet; };

	std::string &getName( void ){ return this->name; };
	const char *getNameC( void ){ return this->name.c_str(); };
	std::string &getWhere( void ){ return this->where; };
	const char *getWhereC( void ){ return this->where.c_str(); };

	/*
	 * determine the name from the filename
	 * -> fch : filename (optionally including its path)
	 * <- name : extracted name
	 */
	void extrName( const std::string &fch, std::string &name );

};

#endif
