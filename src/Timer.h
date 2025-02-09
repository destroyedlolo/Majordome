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
#include "ObjCollection.h"
#include "Tracker.h"

class Timer : public Event, virtual public HandlersExecutor {	// Event contains tasks to launch

	uint32_t every;		// Delay b/w launches
	uint16_t at;			// launch time
	uint16_t min;			// At's minute 

	bool immediate;	// Launch at startup
	bool runifover;	// run immediately if the 'At' hour is already passed

	pthread_t thread;
	static void *threadedslave(void *);

	TrackerVector startTrackers;
	TrackerVector stopTrackers;

public:
	/* Constructor from a file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 */
	Timer( const std::string &file, std::string &where, std::string &name  );
	virtual ~Timer() = default;	// Just to make this class polymorphic

	void readConfigDirective( std::string &l, std::string &name, bool &nameused );

	/* Timers are handled through a dedicated thread ...
	 * A pointer to this object is passed to it.
	 */
	void launchThread( void );

	/*
	 * Executes slaves' handlers
	 */

	void execHandlers(void);
	void feedHandlersState(lua_State *L);

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

	bool isOver( void );	// Tell if this absolute timer is passed and has runifover
	bool inEveryMode( void ){ return( !!this->every ); }

		/* Lua's communication
		 * As timers are running in separate threads an IPC kind of
		 * channel has to be used.
		 */

public:
	enum Commands {
	LAUNCH,	// Launch tasks now
	RESET,	// Reset the timer without launching tasks
	};

private:

	/* IPC signaling */
	pthread_cond_t cond;
	pthread_mutex_t mutex;

	enum Commands cmd;

public:
		// Avoid race condition (for multi-fields value, like "at")
	void lock( void );
	void unlock( void );

	void sendCommand( enum Commands );

	/* Trackers */
	void addStartTracker( Tracker *t ){ this->startTrackers.Add(t); }
	void addStopTracker( Tracker *t ){ this->stopTrackers.Add(t); }

	/* Create Lua's object */
	static void initLuaInterface(lua_State *L);
};

typedef ObjCollection<Timer *> TimerCollection;

#endif
