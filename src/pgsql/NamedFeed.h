/* Add a data to table with a Name field
 */
#ifndef NAMEDFEED_H
#define NAMEDFEED_H

#include "Feed.h"

class NamedFeed : public Feed {
	virtual void feedState(lua_State *L);

		/* Executable */
	virtual bool execAsync(lua_State *L);	// Overloading to handle data acceptation 

public:
	virtual ~NamedFeed(){};

	NamedFeed(const std::string &fch, std::string &where, std::string &name, lua_State *L);

	/* Create Lua's object */
	static void initLuaInterface( lua_State *L );
};

typedef ObjCollection<NamedFeed *> NamedFeedCollection;
#endif
