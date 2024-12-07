/* Some Toile's helpers */

#include <cstring>

#include "Toile.h"
#include "Painting.h"


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

bool Toile::readConfigToile(Config &cfg, std::string &completpath, std::string &where, const char *ext, lua_State *L){
	return false;
}
