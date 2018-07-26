/* Read user configuration directories
 *
 * 25/07/2018 - LF - Switch to C++ (without <filesystem> as not yet supported)
 */
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <sys/stat.h>

#include <unistd.h>	// getcwd()
#include <errno.h>

#include "Components.h"
#include "SortDir.h"

class configDir : virtual public SortDir {
protected :
	virtual bool accept( const char *fch, const char *dir ){
		if( SortDir::accept( fch, dir ) ){
			string p( dir );
			p += '/';
			p += fch;

			struct stat buff;
			if( stat( p.c_str(), &buff )<0 ){
				publishLog('E', "%s : %s", fch, strerror(errno));
				return false;
			}

			return( !!(buff.st_mode & S_IFDIR) );
		} else
			return false;
	}

public:
	configDir(const char *where) { this->readdircontent( where ); };
};

void readUserConfig( const char *dir, lua_State *L ){
	configDir sd( dir );

	for( configDir::iterator i=sd.begin(); i<sd.end(); i++) 
			puts((*i).c_str());
}
