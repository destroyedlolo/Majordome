/* MinMax object
 *
 * 27/10/2024 Creation
 */

#ifndef NMINMAX_H
#define NMINMAX_H

#include "Event.h"
#include "LuaExec.h"
#include "MinMax.h"

#include <unordered_map>
#include <math.h>

class NamedMinMax : public Event, public LuaExec {
public:	// Made public only to let Lua enumerates
	std::unordered_map<std::string, bool> empty;		// No value yet
	std::unordered_map<std::string, lua_Number> min,max;
	std::unordered_map<std::string, size_t> nbre;	// Number of handled values
	std::unordered_map<std::string, lua_Number> sum;

public:
	/* Constructor from a file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 * -> L : Lua's state
	 */
	NamedMinMax( const std::string &file, std::string &where, std::string &name, lua_State *L );

	/* Launch lua script if applicable
	 * -> name : name of the topic/timer that triggers this task
	 * -> topic : the topic itself
	 * <- true if it has been launched, false otherwise
	 */
	bool exec( const char *name, const char *topic=NULL, const char *payload=NULL );

		/* Create Lua's object */
	static void initLuaObject( lua_State *L );

		/* Accessor */
	lua_Number getMin(const char *n){ return this->min[n]; }
	lua_Number getMax(const char *n){ return this->max[n]; }
	lua_Number getAverage(const char *n){ return(this->empty[n] ? INFINITY : this->sum[n]/this->nbre[n]); }
	lua_Number getSum(const char *n){ return this->sum[n]; }
	lua_Number getSamplesNumber(const char *n){ return this->nbre[n]; }

	bool isEmpty(const char *n){ return this->empty[n]; }
	void Clear(const char *n){ this->empty[n] = true; }

	/* Overloading of LuaExec's in order to initialise Myself object */
	virtual void feedState( lua_State *L, const char *name, const char *topic=NULL, const char *payload=NULL, bool tracker=false, const char *trkstatus=NULL );

};

#endif
