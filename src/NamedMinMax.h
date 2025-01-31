/* NamedMinMax object
 *
 * 27/10/2024 Creation
 */

#ifndef NMINMAX_H
#define NMINMAX_H

#include "Event.h"
#include "LuaExec.h"

#include <unordered_map>
#include <math.h>

class NamedMinMax : virtual public Handler {
public:	// Made public only to let Lua enumerates
	std::unordered_map<std::string, bool> empty;		// No value yet
	std::unordered_map<std::string, lua_Number> min,max;
	std::unordered_map<std::string, size_t> nbre;	// Number of handled values
	std::unordered_map<std::string, lua_Number> sum;

private:
	virtual void readConfigDirective( std::string &l, std::string &name, bool &nameused );
	virtual void feedState(lua_State *L);

public:
	/* Constructor from a file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 * -> L : Lua's state
	 */
	NamedMinMax( const std::string &file, std::string &where, std::string &name, lua_State *L );

		/* Executable */
	virtual bool execAsync(lua_State *L);	// Overloading to handle MinMax data feeding

		/* Accessor */
	lua_Number getMin(const char *n){ return(this->empty[n] ? 0 : this->min[n]); }
	lua_Number getMax(const char *n){ return(this->empty[n] ? 0 : this->max[n]); }
	lua_Number getAverage(const char *n){ return(this->empty[n] ? INFINITY : this->sum[n]/this->nbre[n]); }
	lua_Number getSum(const char *n){ return(this->empty[n] ? 0 : this->sum[n]); }
	lua_Number getSamplesNumber(const char *n){ return(this->empty[n] ? 0 : this->nbre[n]); }

	bool isEmpty(const char *n){ return this->empty[n]; }
	void Clear(const char *n){ this->empty[n] = true; }

	/* Create Lua's object */
	static void initLuaInterface( lua_State *L );
};

#endif
