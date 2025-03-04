/* Some Toile's helpers */


#include "Toile.h"

#if 0	/* TODO */
#include "Renderer.h"
#include "Painting.h"
#include "Decoration.h"
#include "Field.h"
#endif

#include "../Helpers.h"

#include <iostream>
#include <cstring>
#include <cassert>

#include "../Config.h"
#include "../SubConfigDir.h"

/* Determine object weight based on its file extension.
 * Will be merged with SubConfigDir ones (and other modules if any.
 */
static const SubConfigDir::extweight fileext[] = {
	{ ".Renderer", 0xc0 },
	{ ".Painting", 0x80 },
	{ ".Field", 0x80 },
	{ ".Decoration", 0x60 }
};

uint8_t Toile::objectweight( const char *ext ){
	for(SubConfigDir::extweight i : fileext){
		if(!strcmp(ext, i.ext))
			return i.weight;
	}
	return 0x00;
}

bool Toile::readConfigToile(Config &cfg, std::string &completpath, std::string &where, std::string &ext, lua_State *L){
/*
	if(debug)
		SelLog->Log('D', "Toile::readConfigToile()");
*/

	if(ext == ".Renderer"){
		std::string name;
		auto tsk = new Renderer( completpath, where, name, L );
		assert(tsk);
	
		RendererCollection::iterator prev;
		if((prev = cfg.RendererList.find(name)) != cfg.RendererList.end()){
			SelLog->Log('F', "Renderer '%s' is defined multiple times (previous one '%s')", name.c_str(), prev->second->getWhere().c_str());
			exit(EXIT_FAILURE);
		} else
			cfg.RendererList.insert( std::make_pair(name, tsk) );

		return true;
#if 0 /* TODO */
	} else if(ext == ".Decoration"){
		if(debug) puts("*D** Toile::readConfigToile() - Decoration");

		std::string name;
		auto paint = new Decoration( completpath, where, name, L );
	
		Config::DecorationElements::iterator prev;
		if((prev = cfg.DecorationList.find(name)) != cfg.DecorationList.end()){
			SelLog->Log('F', "Decoration '%s' is defined multiple times (previous one '%s')", name.c_str(), prev->second->getWhere().c_str());
			exit(EXIT_FAILURE);
		} else
			cfg.DecorationList.insert( std::make_pair(name, paint) );

		if(debug) puts("*D**F Toile::readConfigToile() - Decoration - true");
		return true;
	} else if(ext == ".Painting"){
		if(debug) puts("*D**F Toile::readConfigToile() - Painting");

		std::string name;
		auto paint = new Painting( completpath, where, name, L );
	
		Config::PaintingElements::iterator prev;
		if((prev = cfg.PaintingList.find(name)) != cfg.PaintingList.end()){
			SelLog->Log('F', "Painting '%s' is defined multiple times (previous one '%s')", name.c_str(), prev->second->getWhere().c_str());
			exit(EXIT_FAILURE);
		} else
			cfg.PaintingList.insert( std::make_pair(name, paint) );

		if(debug) puts("*D**F Toile::readConfigToile() - Painting - true");
		return true;
	} else if(ext == ".Field"){
		if(debug) puts("*D**F Toile::readConfigToile() - Field");

		std::string name;
		auto paint = new Field( completpath, where, name, L );
	
		Config::PaintingElements::iterator prev;
		if((prev = cfg.PaintingList.find(name)) != cfg.PaintingList.end()){
			SelLog->Log('F', "Field '%s' is defined multiple times (previous one '%s')", name.c_str(), prev->second->getWhere().c_str());
			exit(EXIT_FAILURE);
		} else
			cfg.PaintingList.insert( std::make_pair(name, paint) );

		if(debug) puts("*D**F Toile::readConfigToile() - Field - true");
		return true;
#endif
	}

/*
	if(debug)
		SelLog->Log('D', "Toile::readConfigToile() false");
*/
	return false;
}

bool Toile::execRenderers(){
	for(auto &i: config.RendererList){
		if(!i.second->exec()){
			if(i.second->getFatal())
				return false;
		}

#if 0	/* TODO Painting */
			// Initialize subsurfaces
		for(auto &paint: i.second->PaintingList)
			paint->exec();
#endif
	}

	return true;
}

void Toile::RefreshRenderers(){
	for(auto &r: config.RendererList)
		r.second->refreshAll();
}
