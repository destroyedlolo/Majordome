/* Generic class for objects
 *
 * 27/07/2018 - LF - First version
 */

#ifndef OBJECT_H
#define OBJECT_H

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
};

#endif
