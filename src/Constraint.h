/* Constraint.h
 * 	Interface to resources
 *
 *	01/09/2025 - LF - First version
 */

#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "Resource.h"

class Constraint {
	Resource *res;
	bool haveToWait;

protected:
	Constraint() : res(NULL) {};

	virtual bool readConfigDirective(std::string &l);

public:
	bool tryToAcquireResource(void);	// False if the resource if no resources available
	 bool waitForResource(void);	// Wait until the resource is available
	void release(void);
};

#endif
