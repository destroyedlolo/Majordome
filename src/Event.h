/* Event
 * Master class to objects having actions attached.
 * Used also to impersonate Rendez-Vous.
 */
#ifndef EVENT_H
#define EVENT_H

#include <vector>

#include "Handler.h"
#include "Object.h"

class Event : virtual public Object, public std::vector<Handler *> {
	// The vector contains the collection of handler to launch

public:
	/* Constructor from a file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 */
	Event( const std::string &file, std::string &where, std::string &name  );
	Event(){};

	void addHandler( Handler *h ){ this->push_back(h); }

	/* Handlers execution */
	void launchHandlers(void);

	/* Create Lua's object */
	static void initLuaInterface( lua_State *L );
};

#endif
