/* Some Toile's helpers */


#include "Toile.h"

#if 0	/* Not needed as included by Config.h */
#include "Renderer.h"
#include "Painting.h"
#include "Decoration.h"
#include "Field.h"
#endif
#include "LCD.h"

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
	{ ".LCD", 0xc0 },
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

	if(ext == ".Renderer"){
		auto tsk = new Renderer( completpath, where, L );
		assert(tsk);

		RendererCollection::iterator prev;
		if((prev = cfg.RendererList.find(tsk->getName())) != cfg.RendererList.end()){
			SelLog->Log('F', "Renderer '%s' is defined multiple times (previous one '%s')", tsk->getName().c_str(), prev->second->getWhere().c_str());
			exit(EXIT_FAILURE);
		} else
			cfg.RendererList.insert( std::make_pair(tsk->getName(), tsk) );

		return true;
	} else if(ext == ".LCD"){
			/* LCD is basically a renderer.
			 * So, but the constructor, the remaining code is the same
			 */
		auto tsk = new LCD( completpath, where, L );
		assert(tsk);
	
		RendererCollection::iterator prev;
		if((prev = cfg.RendererList.find(tsk->getName())) != cfg.RendererList.end()){
			SelLog->Log('F', "Renderer '%s' is defined multiple times (previous one '%s')", tsk->getName().c_str(), prev->second->getWhere().c_str());
			exit(EXIT_FAILURE);
		} else
			cfg.RendererList.insert( std::make_pair(tsk->getName(), tsk) );
		return true;
	} else if(ext == ".Decoration"){
		auto paint = new Decoration( completpath, where, L );
	
		DecorationCollection::iterator prev;
		if((prev = cfg.DecorationList.find(paint->getName())) != cfg.DecorationList.end()){
			SelLog->Log('F', "Decoration '%s' is defined multiple times (previous one '%s')", paint->getName().c_str(), prev->second->getWhere().c_str());
			exit(EXIT_FAILURE);
		} else
			cfg.DecorationList.insert( std::make_pair(paint->getName(), paint) );

		return true;
#if 0 /* TODO painting */
	} else if(ext == ".Painting"){
		auto paint = new Painting( completpath, where, L );
	
		PaintingCollection::iterator prev;
		if((prev = cfg.PaintingList.find(paint->getName())) != cfg.PaintingList.end()){
			SelLog->Log('F', "Painting '%s' is defined multiple times (previous one '%s')", paint->getName().c_str(), prev->second->getWhere().c_str());
			exit(EXIT_FAILURE);
		} else
			cfg.PaintingList.insert( std::make_pair(paint->getName(), paint) );

		return true;
	} else if(ext == ".Field"){
		auto paint = new Field( completpath, where, L );
	
		PaintingCollection::iterator prev;
		if((prev = cfg.PaintingList.find(paint->getName())) != cfg.PaintingList.end()){
			SelLog->Log('F', "Field '%s' is defined multiple times (previous one '%s')", paint->getName().c_str(), prev->second->getWhere().c_str());
			exit(EXIT_FAILURE);
		} else
			cfg.PaintingList.insert( std::make_pair(paint->getName(), paint) );

		return true;
#endif
	}

	return false;
}

bool Toile::execRenderers(){
	for(auto &i: config.RendererList){
		if(!i.second->exec()){
			if(i.second->getFatal())
				return false;
		}

#if 0	/* TODO painting */
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
