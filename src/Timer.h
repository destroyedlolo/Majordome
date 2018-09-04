/* Timer definition
 *
 * Note :
 * Enable / Disable - only control if tasks have to be launched or not
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
	unsigned short at;			// launch time
	unsigned short min;			// At's minute 

	bool immediate;	// Launch at startup
	bool runifover;	// run immediately if the 'At' hour is already passed

	pthread_t thread;
	pthread_cond_t cond;
	pthread_mutex_t mutex;

	enum Commands {
	NOW,	// Launch tasks now
	CHANGE,	// Change consigns
	RESET,	// Reset the timer (only useful if set in "Every" mode)
	} cmd;

	static void *threadedslave(void *);

public:
	/* Constructor from a file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 */
	Timer( const std::string &file, std::string &where, std::string &name  );

	/* The timer is handled through a dedicated thread ... a pointer
	 * to this object has to be passed to the newly created thread
	 * BUT as object are copied into maps, the pointer is only valid after
	 * the said insertion.
	 *
	 */
	void launchThread( void );

	/* Launch tasks associated to this trigger */
	void execTasks( void );

	/* Tell if this timer is passed and has runifover
	 *	Return false if it's an 'every' timer
	 */
	bool isOver( void );

	/*
	 * Accessors
	 */
	unsigned long getEvery( void ){ return this->every; };
	bool getImmediate( void ){ return this->immediate; };
	bool getRunIfOver( void ){ return this->runifover; };
};

#endif
