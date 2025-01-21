/* Common interface to all modules
 *
 * 21/01/2025 - LF - First version
 */

#ifndef MODULEINTERFACE_H
#define MODULEINTERFACE_H

#include "Object.h"

class lua_State;

class ModuleInterface : virtual public Object {

	/* Read directives applicable to the given object.
	 *
	 * -> l : directive line to parse
	 * -> nameused : is the name already used ?
	 */
	virtual void readConfigDirective( std::string &l, bool &nameused ) = 0;

	/* A method can't be both virtual and static ... but this method
	 * is mandatory to declare Lua Interface.
	 */
#if 0
	virtual static void initLuaInterface( lua_State *L ) = 0;
#endif
};

#endif
