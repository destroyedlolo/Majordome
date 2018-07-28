/* Read config directories tree 
 *
 * 26/07/2018 - LF - First version
 */

#include <sys/stat.h>
#include <errno.h>
#include <cstring>

#include "Components.h"
#include "Config.h"
#include "SubConfigDir.h"

bool Config::accept( const char *fch, string &full ){
	if( SortDir::accept( fch, full) ){

		struct stat buff;
		if( stat( full.c_str(), &buff )<0 ){
			publishLog('E', "%s : %s", fch, strerror(errno));
			return false;
		}

		return( !!(buff.st_mode & S_IFDIR) );
	} else
		return false;
}

Config::Config(const char *where, lua_State *L){
	this->readdircontent( where );

	for( iterator i=this->begin(); i<this->end(); i++){
		publishLog('L', "Loading '%s'", (*i).c_str());

		string completpath = where;
		completpath += '/';
		completpath += *i;

		SubConfigDir( *this, completpath, L );
	}
}
