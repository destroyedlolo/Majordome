/* Timer definition
 *
 * Note :
 * Enable / Disable - only control if handlers have to be launched or not
 *
 * 15/08/2018 - LF - First version
 */

#ifndef TIMER_H
#define TIMER_H

#include "Event.h"

class Timer : public Event {	// Event contains tasks to launch

	uint32_t every;		// Delay b/w launches
	uint16_t at;			// launch time
	uint16_t min;			// At's minute 

	bool immediate;	// Launch at startup
	bool runifover;	// run immediately if the 'At' hour is already passed

	pthread_t thread;
	pthread_cond_t cond;
	pthread_mutex_t mutex;

public:
	enum Commands {
	LAUNCH,	// Launch tasks now
	RESET,	// Reset the timer without launching tasks
	};

private :
	enum Commands cmd;

public:
	/* Constructor from a file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 */
	Timer( const std::string &file, std::string &where, std::string &name  );

	void readConfigDirective( std::string &l, std::string &name, bool &nameused );
};

#endif
