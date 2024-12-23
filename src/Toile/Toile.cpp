/* Some Toile's helpers */

#include <cstring>

#include "Toile.h"
#include "Renderer.h"
#include "Decoration.h"


/* Determine object weight based on its file extension.
 * Will be merged with SubConfigDir ones (and other modules if any.
 */
#include "../SubConfigDir.h"

static const SubConfigDir::extweight fileext[] = {
	{ ".Renderer", 0xc0 },
/*	{ ".Painting", 0x80 }, */
	{ ".Decoration", 0x60 }
};

uint8_t Toile::objectweight( const char *ext ){
	for(SubConfigDir::extweight i : fileext){
		if(!strcmp(ext, i.ext))
			return i.weight;
	}
	return 0x00;
}

bool Toile::readConfigToile(Config &cfg, std::string &completpath, std::string &where, const char *ext, lua_State *L){
	if( !strcmp(ext,".Renderer") ){
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
	} else if( !strcmp(ext,".Decoration") ){
		std::string name;
		Decoration paint( completpath, where, name, L );
	
		Config::DecorationElements::iterator prev;
		if((prev = cfg.DecorationList.find(name)) != cfg.DecorationList.end()){
			SelLog->Log('F', "Decoration '%s' is defined multiple times (previous one '%s')", name.c_str(), prev->second.getWhere().c_str());
			exit(EXIT_FAILURE);
		} else
			cfg.DecorationList.insert( std::make_pair(name, paint) );
		return true;
	}
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
