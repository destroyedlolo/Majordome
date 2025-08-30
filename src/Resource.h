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
	uint8_t limit;	// Maximum number of concurrent access

protected:
	virtual bool readConfigDirective(std::string &l);

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

#if 0
class Semaphore {
	std::mutex mutex;				// Avoid concurrent connection
	std::condition_variable cond;	// Condition
	uint8_t counter;				// Counter of running tasks

public:
	explicit Semaphore(uint8_t max) : counter(max) {}

	void acquire(){
		std::unique_lock<std::mutex> lock(this->mutex);	// the Mutex will be automatically released when lock will be destroyed, at the end of this block
		this->cond.wait(lock, [this]() { return this->counter > 0; });
        --this->counter;
    }

	void release(){
		{
			// the Mutex will be automatically released when lock will be
			// destroyed, at the end of this block.
			// Here in a sub block as it as to be released before the
			// notifications.
			std::lock_guard<std::mutex> lock(this->mutex);
			++this->counter;
		}
		this->cond.notify_one();
    }

};
#endif

#endif
