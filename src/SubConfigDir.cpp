/* Package configuration handling
 *
 * 27/07/2018 - LF - First version
 */
#include <cstring>

#include "Components.h"
#include "SubConfigDir.h"

bool SubConfigDir::accept( const char *fch, string &full ){
	if( SortDir::accept( fch, full ) ){
		const char *ext = fileextention( fch );
		if( !strcmp(ext,".lua") )
			return true;
	}
	return false;
}

SubConfigDir::SubConfigDir(const char *where, lua_State *L){
}
