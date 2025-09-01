/* Resource.h
 *	Reduce or avoid concurrent access.
 *
 *	29/08/2025 - LF - First version
 */

#ifndef RESOURCE_H
#define RESOURCE_H

#include "Object.h"
#include "ObjCollection.h"

#include <mutex>
#include <condition_variable>

#include <cstdint>

class Resource : virtual public Object {
	uint8_t limit;		// Maximum number of concurrent access
	uint8_t counter;	// Counter of running tasks

	std::mutex mutex;				// Avoid concurrent querying
	std::condition_variable cond;	// Condition

protected:
	virtual bool readConfigDirective(std::string &l);

		// By default, wait until the task can run. Return true when ok
		// if wait == false, return false if no resource available
	bool acquire(bool wait = true);
	void release(void);

public:
	/* Constructor from a file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 */
	Resource( const std::string &file, std::string &where );

	virtual std::string getTri(){ return Resource::trigramme(); }
	static std::string trigramme(){ return "RST_"; }
};
typedef ObjCollection<Resource *> ResourceCollection;


#endif
