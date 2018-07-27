/* Package configuration handling
 *
 * 27/07/2018 - LF - First version
 */
#include <cstring>

#include "Components.h"
#include "SubConfigDir.h"

bool SubConfigDir::accept( const char *fch, const char *dir ){
	if( SortDir::accept( fch, dir ) ){
		const char *ext = fileextention( fch );
		if( !strcmp(ext,".lua") )
			return true;
	}
	return false;
}


