/* Generic class for objects
 *
 * 27/07/2018 - LF - First version
 */

#ifndef OBJECT_H
#define OBJECT_H

#include <string>

class Object {
	bool disabled;

protected:
	std::string name;
	std::string where;

public:
	Object() : disabled(false){};

	void enable( void ){ this->disabled = false; };
	void disable( void ){ this->disabled = true; };
	bool isEnabled( void ){ return this->disabled; };

	std::string &getName( void ){ return this->name; };
	std::string &getWhere( void ){ return this->where; };

	/*
	 * determine the name from the filename
	 * -> fch : filename (optionally including its path)
	 * <- name : extracted name
	 */
	void extrName( const std::string &fch, std::string &name );

};

#endif
