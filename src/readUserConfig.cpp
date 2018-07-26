/* Read user configuration directories
 *
 * 25/07/2018 - LF - Switch to C++ (without <filesystem> as not yet supported)
 */
#include <cstdio>

#include "Components.h"
#include "ConfigDir.h"

void readUserConfig( const char *dir, lua_State *L ){
	configDir sd( dir );

	for( configDir::iterator i=sd.begin(); i<sd.end(); i++) 
			puts((*i).c_str());
}
