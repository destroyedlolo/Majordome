/* Some Toile's helpers */

#include <cstring>

#include "Toile.h"

/* Determine object weight based on its file extension.
 * Will be merged with SubConfigDir ones (and other modules if any.
 */
#include "../SubConfigDir.h"

static const SubConfigDir::extweight fileext[] = {
	{ ".painting", 0xc0 }
};

uint8_t Toile::objectweight( const char *ext ){
	for(SubConfigDir::extweight i : fileext){
		if(!strcmp(ext, i.ext))
			return i.weight;
	}
	return 0x00;
}
