/* Package configuration handling
 *
 * 27/07/2018 - LF - First version
 * 10/08/2018 - LF - Force loading order
 * 16/03/2019 - LF - Add .tracker
 * 20/05/2024 - LF - Migrate to v4
 * 20/01/2025 - LF - Migrate to v6
 */

#include "Selene.h"
#include "Helpers.h"
#include "SubConfigDir.h"
#ifdef TOILE
#	include "Toile/Toile.h"
#endif
#include "Config.h"

#include <algorithm>

/* Determine object weight based on its file extension 
 * Some space are left for modules extensions (like Toile's)
 */
static const SubConfigDir::extweight fileext[] = {
#if 0
	{ ".topic", 0xc0 },
	{ ".timer", 0xc0 },
	{ ".rendezvous", 0xc0 },
	{ ".tracker", 0x80 },
	{ ".minmax", 0x80 },
	{ ".namedminmax", 0x80 },
	{ ".shutdown", 0x50 },
	{ ".lua", 0x40 },
#endif
	{ ".md", 0x01 }	// ignored, documentation only
};

static uint8_t objectweight( const char *ext ){
	uint8_t ret = 0x00;
	for(SubConfigDir::extweight i : fileext){
		if(ext == i.ext)
			return i.weight;
	}

#if TOILE
	if((ret = Toile::objectweight(ext)))
		return ret;
#endif

	return ret;
}

bool SubConfigDir::accept( const char *fch, std::string &full ){
	if(SortDir::accept( fch, full )){
		const char *ext = fileextention( fch );
		bool res = false;

		if(ext)
			res = objectweight(ext);

		if(!res)
			SelLog->Log('E', "'%s' is rejected", fch);

		return( res );
	}
	return false;
}

SubConfigDir::SubConfigDir(Config &cfg, std::string &where, lua_State *L){
	this->readdircontent(where);

	for(auto i=this->begin(); i<this->end(); i++){
		std::string completpath = where + '/' + *i;
		std::string ext = fileextention((*i).c_str());

		if(ext == ".md"){	// Ignore documentation
			;
#	ifdef DEBUG
		} else 
			if(debug)
				SelLog->Log('D', "Ignoring %s (ext '%s')", (*i).c_str(), ext );
#	endif
	}
}

void SubConfigDir::sort( void ){
	std::sort(this->begin(), this->end(), 
		[](std::string const &a, std::string const &b) -> bool {
			int va = objectweight( fileextention( a.c_str() ));
			int vb = objectweight( fileextention( b.c_str() ));

			return(va > vb);
		}
	);
}
