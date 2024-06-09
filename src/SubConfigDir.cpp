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

/* Determine object weight based on its file extension 
 * (reverse ordered by weight)
 */
static const char * fileext[] = {
	".topic",
	".timer",
	".rendezvous",
	".tracker",
	".lua",
	".md"
};

static int objectweight( const char *ext ){
	for(unsigned int i = 0; i<sizeof(fileext)/sizeof(const char *); i++){
		if(!strcmp(ext, fileext[i]))
			return (int)i;
	}
	return -1;
}

bool SubConfigDir::accept( const char *fch, std::string &full ){
	if(SortDir::accept( fch, full )){
		const char *ext = fileextention( fch );
		bool res = false;

		if(ext)
			res = (objectweight(ext) != -1 );

		if(!res)
			SelLog->Log('E', "'%s' is rejected", fch);

		return( res );
	}
	return false;
}

SubConfigDir::SubConfigDir(Config &cfg, std::string &where, lua_State *L){
	this->readdircontent(where);

	for( iterator i=this->begin(); i<this->end(); i++){
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
		}
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

			return(va < vb);
		}
	);
}
