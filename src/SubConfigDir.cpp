/* Package configuration handling
 *
 * 27/07/2018 - LF - First version
 * 10/08/2018 - LF - Force loading order
 * 16/03/2019 - LF - Add .tracker
 * 20/05/2024 - LF - Migrate to v4
 */
#include <cstring>
#include <algorithm>

#include "Selene.h"
#include "Helpers.h"
#include "SubConfigDir.h"

#ifdef TOILE
#	include "Toile/Toile.h"
#endif

/* Determine object weight based on its file extension 
 * Some space are left for modules extensions (like Toile's)
 */
static const SubConfigDir::extweight fileext[] = {
	{ ".topic", 0xc0 },
	{ ".timer", 0xc0 },
	{ ".rendezvous", 0xc0 },
	{ ".tracker", 0x80 },
	{ ".minmax", 0x80 },
	{ ".namedminmax", 0x80 },
	{ ".lua", 0x40 },
	{ ".shutdown", 0x40 },
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

	return 0x00;
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
		const char *ext = fileextention((*i).c_str());

		if(!strcmp(ext,".md"))	// Ignore documentation
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
		} else if( !strcmp(ext,".timer") ){
			std::string name;
			Timer tmr( completpath, where, name );

			Config::TimerElements::iterator p;
			if((p = cfg.TimersList.find(name)) != cfg.TimersList.end()){
				SelLog->Log('F', "Timer '%s' is defined multiple times (previous one '%s')", name.c_str(), p->second.getWhere().c_str());
				exit(EXIT_FAILURE);
			} else
				cfg.TimersList.insert( std::make_pair(name, tmr) ).first;
		} else if( !strcmp(ext,".lua") ){
			std::string name;
			LuaTask tsk( completpath, where, name, L );
	
			Config::TaskElements::iterator prev;
			if((prev = cfg.TasksList.find(name)) != cfg.TasksList.end()){
				SelLog->Log('F', "Task '%s' is defined multiple times (previous one '%s')", name.c_str(), prev->second.getWhere().c_str());
				exit(EXIT_FAILURE);
			} else
				cfg.TasksList.insert( std::make_pair(name, tsk) );
		} else if( !strcmp(ext,".topic") ){
			std::string name;
			MQTTTopic tpc( completpath, where, name );

			Config::TopicElements::iterator prev;
			if((prev = cfg.TopicsList.find(name)) != cfg.TopicsList.end()){
				SelLog->Log('F', "Topic '%s' is defined multiple times (previous one '%s')", name.c_str(), prev->second.getWhere().c_str());
				exit(EXIT_FAILURE);
			} else
				cfg.TopicsList.insert( std::make_pair(name, tpc) );
		} else if( !strcmp(ext,".rendezvous") ){
			std::string name;
			Event evt( completpath, where, name );

			Config::EventElements::iterator prev;
			if((prev = cfg.EventsList.find(name)) != cfg.EventsList.end()){
				SelLog->Log('F', "Event '%s' is defined multiple times (previous one '%s')", name.c_str(), prev->second.getWhere().c_str());
				exit(EXIT_FAILURE);
			} else
				cfg.EventsList.insert( std::make_pair(name, evt) );
		} else if( !strcmp(ext,".tracker") ){
			std::string name;
			Tracker trk( completpath, where, name, L );
	
			Config::TrackerElements::iterator prev;
			if((prev = cfg.TrackersList.find(name)) != cfg.TrackersList.end()){
				SelLog->Log('F', "Tracker '%s' is defined multiple times (previous one '%s')", name.c_str(), prev->second.getWhere().c_str());
				exit(EXIT_FAILURE);
			} else
				cfg.TrackersList.insert( std::make_pair(name, trk) );
		} else if( !strcmp(ext,".minmax") ){
			std::string name;
			MinMax trk( completpath, where, name, L );
	
			Config::MinMaxElements::iterator prev;
			if((prev = cfg.MinMaxList.find(name)) != cfg.MinMaxList.end()){
				SelLog->Log('F', "MinMax '%s' is defined multiple times (previous one '%s')", name.c_str(), prev->second.getWhere().c_str());
				exit(EXIT_FAILURE);
			} else
				cfg.MinMaxList.insert( std::make_pair(name, trk) );
		} else if( !strcmp(ext,".namedminmax") ){
			std::string name;
			NamedMinMax trk( completpath, where, name, L );

			Config::NamedMinMaxElements::iterator prev;
			if((prev = cfg.NamedMinMaxList.find(name)) != cfg.NamedMinMaxList.end()){
				SelLog->Log('F', "NamedMinMax '%s' is defined multiple times (previous one '%s')", name.c_str(), prev->second.getWhere().c_str());
				exit(EXIT_FAILURE);
			} else
				cfg.NamedMinMaxList.insert( std::make_pair(name, trk) );
		}
#	ifdef TOILE
		else if(Toile::readConfigToile(cfg, completpath, where, ext, L))
			;
#	endif
#	ifdef DEBUG
		else 
			if(debug)
				SelLog->Log('D', "Ignoring %s (ext '%s')", (*i).c_str(), ext );
#	endif
	}
}

void SubConfigDir::sort( void ){
	std::sort(entries.begin(), entries.end(), 
		[](std::string const &a, std::string const &b) -> bool {
			int va = objectweight( fileextention( a.c_str() ));
			int vb = objectweight( fileextention( b.c_str() ));

			return(va > vb);
		}
	);
}
