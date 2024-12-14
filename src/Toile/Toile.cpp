/* Some Toile's helpers */

#include <cstring>

#include "Toile.h"
#include "Renderer.h"


/* Determine object weight based on its file extension.
 * Will be merged with SubConfigDir ones (and other modules if any.
 */
#include "../SubConfigDir.h"

static const SubConfigDir::extweight fileext[] = {
	{ ".renderer", 0xc0 }
};

uint8_t Toile::objectweight( const char *ext ){
	for(SubConfigDir::extweight i : fileext){
		if(!strcmp(ext, i.ext))
			return i.weight;
	}
	return 0x00;
}

bool Toile::readConfigToile(Config &cfg, std::string &completpath, std::string &where, const char *ext, lua_State *L){
	if( !strcmp(ext,".renderer") ){
		std::string name;
if(debug) puts("*D*** Toile::readConfigToile()");
		Renderer paint( completpath, where, name, L );
	
		Config::RendererElements::iterator prev;
		if((prev = cfg.RendererList.find(name)) != cfg.RendererList.end()){
			SelLog->Log('F', "Renderer '%s' is defined multiple times (previous one '%s')", name.c_str(), prev->second.getWhere().c_str());
			exit(EXIT_FAILURE);
		} else
			cfg.RendererList.insert( std::make_pair(name, paint) );
if(debug) puts("*D**F Toile::readConfigToile() true");
		return true;
	}
if(debug) puts("*D**F Toile::readConfigToile() false");
	return false;
}

bool Toile::execRenderer(){
	for(auto &i: config.RendererList){
		if(!i.second.exec()){
			if(i.second.getFatal())
				return false;
		}
	}

	return true;
}
