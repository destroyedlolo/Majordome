/* MinMax object
 *
 * 26/10/2024 Creation
 */

#ifndef MINMAX_H
#define MINMAX_H

#include "Handler.h"
#include "ObjCollection.h"

#include <math.h>

class MinMax : public Handler {
	bool empty;		// No value yet
	lua_Number min,max;
	size_t nbre;	// Number of handled values
	lua_Number sum;

	virtual void readConfigDirective( std::string &l, std::string &name, bool &nameused );
	virtual void feedState(lua_State *L);

public:
	/* Constructor from a file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 * -> L : Lua's state
	 */
	MinMax( const std::string &file, std::string &where, std::string &name, lua_State *L );

		/* Accessors */
	lua_Number getMin(){ return(this->empty ? 0 : this->min); }
	lua_Number getMax(){ return(this->empty ? 0 : this->max); }
	lua_Number getAverage(){ return(this->empty ? INFINITY : this->sum/this->nbre); }
	lua_Number getSum(){ return(this->empty ? 0 : this->sum); }
	lua_Number getSamplesNumber(){return(this->empty ? 0 :  this->nbre); }

	bool isEmpty(){ return this->empty; }
	void Clear(){ this->empty = true; }

		/* Executable */
	virtual bool execAsync(lua_State *L);	// Overloading to handle MinMax data feeding

	/* Create Lua's object */
	static void initLuaInterface( lua_State *L );
};

typedef ObjCollection<MinMax *> MinMaxCollection;

#endif
