
/* Timer definition
 *
 * 15/08/2018 - LF - First version
 */

#ifndef TIMER_H
#define TIMER_H

#include <ctime>
#include <sys/time.h>
#include <cerrno>

#include "Event.h"

class Timer : public Event {
	unsigned long every;		// Delay b/w launches
	unsigned long at;			// launch time
	unsigned long min;		// At's minute (act as a flag as well)

	bool immediate;	// Launch at startup
	bool runifover;	// run immediately if the 'At' hour is already passed

	int tfd;		// Timer file descriptor
	int efd;		// Even fd

	pthread_cond_t cond;
	pthread_mutex_t mutex;

public:
	/* Constructor from a file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 */
	Timer( const std::string &file, std::string &where, std::string &name  );
};

#endif
