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

#include "StringVector.h"
#include "Event.h"

 
class Timer : public Event {	// Event contains tasks to launch

	unsigned long every;		// Delay b/w launches
	unsigned short at;			// launch time
	unsigned short min;			// At's minute 

	bool immediate;	// Launch at startup
	bool runifover;	// run immediately if the 'At' hour is already passed

	pthread_t thread;
	pthread_cond_t cond;
	pthread_mutex_t mutex;

	StringVector startTrackers;
	StringVector stopTrackers;

public:
	enum Commands {
	LAUNCH,	// Launch tasks now
	RESET,	// Reset the timer without launching tasks
	};

private :
	enum Commands cmd;

	static void *threadedslave(void *);

public:
	/* Constructor from a file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 */
	Timer( const std::string &file, std::string &where, std::string &name  );

	void addStartTracker( std::string t ){ this->startTrackers.Add(t); }
	void addStopTracker( std::string t ){ this->stopTrackers.Add(t); }

	/* The timer is handled through a dedicated thread ... a pointer
	 * to this object has to be passed to the newly created thread
	 * BUT as object are copied into maps, the pointer is only valid after
	 * the said insertion.
	 */
	void launchThread( void );

	/* Launch tasks associated to this trigger 
	 * Disable/Enable trackers as well
	 */
	void execTasks( void );

	/* Tell if this timer is passed and has runifover
	 *	Return false if it's an 'every' timer
	 */
	bool isOver( void );

	/* Tell if this timer is in 'every' mode */
	bool inEveryMode( void );

	/*
	 * (un)Lock timer data to avoid race condition
	 */
	void lock( void );
	void unlock( void );

	/*
	 *	Send a command to the timer
	 */
	void sendCommand( enum Commands );

	/*
	 * Accessors
	 */
	unsigned long getEvery( void ){ return this->every; }
	void setEvery( unsigned long v ){ this->every = v; }

	unsigned short getAt( void ){ return this->at; }
	unsigned short getMin( void ){ return this->min; }
	void setAt( unsigned short v ){ this->at = v; }
	void setMin( unsigned short v ){ this->min = v; }

	bool getImmediate( void ){ return this->immediate; }
	bool getRunIfOver( void ){ return this->runifover; }

	/* Create Lua's object */
	static void initLuaObject( lua_State *L );
};

#endif
