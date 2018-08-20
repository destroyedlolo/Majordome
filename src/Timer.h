
/* Timer definition
 *
 * 15/08/2018 - LF - First version
 */

#ifndef TIMER_H
#define TIMER_H

#include "Event.h"

class Timer : public Event {
	int sample;		// Delay b/w launches
	int at;			// launch time
	int min;		// At's minute (act as a flag as well)

	bool immediate;	// Launch at startup
	bool runifover;	// run immediately if the 'At' hour is already passed

public:
	/* Constructor from a file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 */
	Timer( const std::string &file, std::string &where, std::string &name  );
};

#endif
