/* Configuration handling
 *
 * 26/07/2018 - LF - First version
 * 27/07/2018 - LF - handle all Configuration aspects
 */
#ifndef CONFIG_H
#define CONFIG_H

#include <lua.h>	/* Lua's state needed */

#include "SortDir.h"

class Config : virtual public SortDir {
protected :
	virtual bool accept( const char *fch, const char *dir );

public:
	Config(const char *where, lua_State *L);
};

#endif
