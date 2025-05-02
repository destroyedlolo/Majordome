/* Tracker object
 * 
 * 16/03/2019
 */

#ifndef TRACKER_H
#define TRACKER_H

#include "Handler.h"
#include "HandlersExecutor.h"
#include "ObjCollection.h"
#include "LuaTask.h"

class Tracker : public Handler, virtual public HandlersExecutor {	// HandlersExecutor contains tasks to launch when tracker changes to DONE


		/* notifications */
	HandlerVector startingHandlers;	// Handlers to launch when starting the tracker
	HandlerVector stoppingHandlers;	// Handlers to launch when stopping the tracker
	HandlerVector changingHandlers;	// Handlers to launch when the tracker's status is changing

	virtual void readConfigDirective( std::string &l );

public:
	enum _status {
		WAITING,	// Waiting for start time
		CHECKING,	// checking periode (b/w start and stop time)
		DONE,		// the condition has been met during checking periode
	};

private:
	enum _status status;
	std::string statusTopic;
	unsigned int howmany;		// howmany consign
	unsigned int hm_counter;	// actual counter value

	void publishstatus( void );
	void notifyChanged(void);

public:
	/* Constructor from a file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 * -> L : Lua's state
	 */
	Tracker( const std::string &file, std::string &where, lua_State *L );

	virtual void feedState(lua_State *L);
	void feedHandlersState(lua_State *L);

	enum _status getStatus( void ){ return this->status; }
	const char *getStatusC( void );
	unsigned int getCounter(void){ return this->hm_counter; }
	void resetCounter(void){ this->hm_counter = this->howmany; }

	void setStatusTopic( std::string t ){ this->statusTopic = t; }
	bool asStatusTopic( void ){ return !this->statusTopic.empty(); }
	std::string &getStatusTopic( void ){ return this->statusTopic; }

	/* Change tracker status */
	void start( void );
	void stop( void );
	void done( void );

	void addDone( Handler *t ){ this->push_back(t); }
	void addStarted( Handler *t ){ this->startingHandlers.Add(t); }
	void addStopped( Handler *t ){ this->stoppingHandlers.Add(t); }
	void addChanged( Handler *t ){ this->changingHandlers.Add(t); }

		/* Executable */
	virtual bool execAsync(lua_State *L);	// Overloading to handle data acceptation 

	/* Create Lua's object */
	static void initLuaInterface( lua_State *L );

	virtual std::string getTri(){ return "TRK_"; }
};

typedef ObjCollection<Tracker *> TrackerCollection;
typedef ObjVector<Tracker *> TrackerVector;
#endif
