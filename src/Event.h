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

	/* Handlers execution
	 * (runs sequentially)
	 */
	void execHandlers(void);		// fresh State but in the same thread
	void execHandlers(lua_State *);	// same thread, same State

		/* Execute on particular events.
		 * By default, they are doing nothing ... 
		 * They have to be overloaded by objects handling this event.
		 *
		 * The 1st argument is only here to identify the event's kind.
		 *
		 * Note : in the case of MQTTTopic, the handler is not defined here :
		 * the processing is part of msgarrived() and we know which handler to
		 * call.
		 */

	/* Create Lua's object */
	static void initLuaInterface(lua_State *L);
};

#endif
