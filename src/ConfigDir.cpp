/* Read config directories tree 
 *
 * 26/07/2018 - LF - First version
 */

#include <sys/stat.h>
#include <errno.h>
#include <cstring>

#include "Components.h"
#include "ConfigDir.h"

bool configDir::accept( const char *fch, const char *dir ){
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

