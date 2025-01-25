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
#include <cstring>
#include <cassert>

/* Determine object weight based on its file extension 
 * Some space are left for modules extensions (like Toile's)
 */
static const SubConfigDir::extweight fileext[] = {
	{ ".topic", 0xc0 },
	{ ".timer", 0xc0 },
	{ ".rendezvous", 0xc0 },
#if 0
	{ ".tracker", 0x80 },
	{ ".minmax", 0x80 },
	{ ".namedminmax", 0x80 },
	{ ".shutdown", 0x50 },
#endif
	{ ".lua", 0x40 },
	{ ".md", 0x01 }	// ignored, documentation only
};

static uint8_t objectweight( const char *ext ){
	uint8_t ret = 0x00;
	for(SubConfigDir::extweight i : fileext){
		if(!strcmp(ext, i.ext))
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

		if(ext == ".md")	// Ignore documentation
			;
		else if( *i == "Init.lua" ){
			if(configtest){
				SelLog->Log('T', "\t'Init.lua' (Not launched : test mode)");
				continue;
			}

			if(verbose)
				SelLog->Log('L', "\t'Init.lua'");
				
			lua_pushstring(L, where.c_str());
			lua_setglobal(L, "SELENE_SCRIPT_DIR");
			lua_pushstring(L, (*i).c_str());
			lua_setglobal(L, "SELENE_SCRIPT_NAME");

			int err = luaL_loadfile(L, completpath.c_str()) || lua_pcall(L, 0, 0, 0);
			if(err){
				SelLog->Log('F', "%s : %s", completpath.c_str(), lua_tostring(L, -1));
				lua_pop(L, 1);  /* pop error message from the stack */
				exit(EXIT_FAILURE);
			}
		} else if(ext == ".lua"){
			std::string name;
			auto tsk = new LuaTask( completpath, where, name, L );
			assert(tsk);
	
			Config::TaskCollection::iterator prev;
			if((prev = cfg.TasksList.find(name)) != cfg.TasksList.end()){
				SelLog->Log('F', "Task '%s' is defined multiple times (previous one '%s')", name.c_str(), prev->second->getWhere().c_str());
				exit(EXIT_FAILURE);
			} else
				cfg.TasksList.insert( std::make_pair(name, tsk) );
		} else if(ext == ".topic"){
			std::string name;
			auto tpc = new MQTTTopic( completpath, where, name );

			Config::TopicCollection::iterator prev;
			if((prev = cfg.TopicsList.find(name)) != cfg.TopicsList.end()){
				SelLog->Log('F', "Topic '%s' is defined multiple times (previous one '%s')", name.c_str(), prev->second->getWhereC());
				exit(EXIT_FAILURE);
			} else
				cfg.TopicsList.insert( std::make_pair(name, tpc) );
		} else if(ext == ".timer" ){
			std::string name;
			auto tmr = new Timer( completpath, where, name );

			Config::TimerCollection::iterator p;
			if((p = cfg.TimersList.find(name)) != cfg.TimersList.end()){
				SelLog->Log('F', "Timer '%s' is defined multiple times (previous one '%s')", name.c_str(), p->second->getWhereC());
				exit(EXIT_FAILURE);
			} else
				cfg.TimersList.insert( std::make_pair(name, tmr) ).first;
		} else if(ext == ".rendezvous"){
			std::string name;
			auto evt = new Event( completpath, where, name );

			Config::EventCollection::iterator prev;
			if((prev = cfg.EventsList.find(name)) != cfg.EventsList.end()){
				SelLog->Log('F', "Event '%s' is defined multiple times (previous one '%s')", name.c_str(), prev->second->getWhereC());
				exit(EXIT_FAILURE);
			} else
				cfg.EventsList.insert( std::make_pair(name, evt) );
#	ifdef DEBUG
		} else 
			if(debug)
				SelLog->Log('D', "Ignoring %s (ext '%s')", (*i).c_str(), ext.c_str() );
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
