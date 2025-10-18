/* Constraint.h
 * 	Interface to resources
 *
 *	01/09/2025 - LF - First version
 */

#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "Resource.h"
#include "Object.h"

class Constraint : virtual public Object {
	Resource *res;
	bool haveToWait;

protected:
	Constraint() : res(NULL) {};

	virtual bool readConfigDirective(std::string &l) override;

public:
	bool acquireResource(void);	// Try to get the resource
	void release(void);
};

#endif
