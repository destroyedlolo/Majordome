/* MinMax object
 *
 * 26/10/2024 Creation
 */

#ifndef MINMAX_H
#define MINMAX_H

#include "Event.h"
#include "LuaExec.h"

#include <math.h>

class MinMax : public Event, public LuaExec {
	bool empty;		// No value yet
	lua_Number min,max;
	size_t nbre;	// Number of handled values
	lua_Number sum;

public:
	/* Constructor from a file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 * -> L : Lua's state
	 */
	MinMax( const std::string &file, std::string &where, std::string &name, lua_State *L );

	/* Launch lua script if applicable
	 * -> name : name of the topic/timer that triggers this task
	 * -> topic : the topic itself
	 * <- true if it has been launched, false otherwise
	 */
	bool exec( const char *name, const char *topic=NULL, const char *payload=NULL );

		/* Create Lua's object */
	static void initLuaObject( lua_State *L );

		/* Accessor */
	lua_Number getMin(){ return(this->empty ? 0 : this->min); }
	lua_Number getMax(){ return(this->empty ? 0 : this->max); }
	lua_Number getAverage(){ return(this->empty ? INFINITY : this->sum/this->nbre); }
	lua_Number getSum(){ return(this->empty ? 0 : this->sum); }
	lua_Number getSamplesNumber(){return(this->empty ? 0 :  this->nbre); }

	bool isEmpty(){ return this->empty; }
	void Clear(){ this->empty = true; }

	/* Overloading of LuaExec's in order to initialise Myself object */
	virtual void feedState( lua_State *L, const char *name, const char *topic=NULL, const char *payload=NULL, bool tracker=false, const char *trkstatus=NULL );

};

#endif
