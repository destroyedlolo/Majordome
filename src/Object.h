/* Generic class for objects
 *
 * 27/07/2018 - LF - First version
 */

#ifndef OBJECT_H
#define OBJECT_H

class Object {
	bool disabled;

public:
	Object() : disabled(false){};

	void enable( void ){ this->disabled = false; };
	void disable( void ){ this->disabled = true; };
	bool isEnabled( void ){ return this->disabled; };
};

#endif
