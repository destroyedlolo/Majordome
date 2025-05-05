/* Add a data to table with a Name field
 */
#ifndef NAMEDFEED_H
#define NAMEDFEED_H

#include "Feed.h"

class NamedFeed : virtual public Feed {
	virtual void feedState(lua_State *L);

		/* Executable */
	virtual bool execAsync(lua_State *L);	// Overloading to handle data acceptation 

public:
	NamedFeed(const std::string &, std::string &, lua_State *);


	/* Create Lua's object */
	static void initLuaInterface( lua_State *L );

	virtual std::string getTri(){ return NamedFeed::trigramme(); }
	static std::string trigramme(){ return "NFD_"; }
};

typedef ObjCollection<NamedFeed *> NamedFeedCollection;
#endif
