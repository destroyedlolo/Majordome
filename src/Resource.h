/* Resource.h
 *	Reduce or avoid concurrent access.
 *
 *	29/08/2025 - LF - First version
 */

#ifndef RESOURCE_H
#define RESOURCE_H

#include <mutex>
#include <condition_variable>

#include <cstdint>

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
