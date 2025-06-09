/* MinMax with a string array as key
 *
 * 08/06/2025 Creation
 *
 *	See https://stackoverflow.com/questions/42701688/using-an-unordered-map-with-arrays-as-keys
 */

#ifndef MKMINMAX_H
#define MKMINMAX_H

#include "Handler.h"
#include "ObjCollection.h"

#include <unordered_map>
#include <math.h>

struct MKuMapH {
	std::size_t operator()(const std::vector<std::string> &) const;
};

class MultiKeysMinMax : public Handler {
	uint8_t nk;

public:
	std::unordered_map<std::vector<std::string>, bool, MKuMapH> empty;
	std::unordered_map<std::vector<std::string>, lua_Number, MKuMapH> min,max;
	std::unordered_map<std::vector<std::string>, size_t, MKuMapH> nbre;	// Number of handled values
	std::unordered_map<std::vector<std::string>, lua_Number, MKuMapH> sum;

private:
	virtual void readConfigDirective( std::string &l );
	virtual void feedState(lua_State *L);

	bool keysSanityCheck(const std::vector<std::string> &arg){ return(arg.size() == this->nk); }

public:
	/* Constructor from a file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 * -> L : Lua's state
	 */
	MultiKeysMinMax( const std::string &file, std::string &where, lua_State *L );

		/* Executable */
	virtual bool execAsync(lua_State *L);	// Overloading to handle MinMax data feeding
	void readKeys(lua_State *, int , std::vector<std::string> &);

		/* Accessors */
	uint8_t getNumberOfKeys(){ return this->nk; }

	std::unordered_map<std::vector<std::string>, bool, MKuMapH> &getEmptyList() { return this->empty; }
	lua_Number getMin(const std::vector<std::string> n){ return(this->empty[n] ? 0 : this->min[n]); }
	lua_Number getMax(const std::vector<std::string> n){ return(this->empty[n] ? 0 : this->max[n]); }
	lua_Number getAverage(const std::vector<std::string> n){ return(this->empty[n] ? INFINITY : this->sum[n]/this->nbre[n]); }
	lua_Number getSum(const std::vector<std::string> n){ return(this->empty[n] ? 0 : this->sum[n]); }
	lua_Number getSamplesNumber(const std::vector<std::string> n){ return(this->empty[n] ? 0 : this->nbre[n]); }

	bool isEmpty(const std::vector<std::string> &n){ return this->empty[n]; }
	void Clear(const std::vector<std::string> &n){ if(this->keysSanityCheck(n)) this->empty[n] = true; }

	void push(const std::vector<std::string> &, lua_Number);	// Push a new data

	/* Create Lua's object */
	static void initLuaInterface( lua_State *L );

	virtual std::string getTri(){ return MultiKeysMinMax::trigramme(); }
	static std::string trigramme(){ return "MKM_"; }

#if DEBUG
	void dump();
#endif
};

typedef ObjCollection<MultiKeysMinMax *> MultiKeysMinMaxCollection;
#endif
