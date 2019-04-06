/* Package configuration handling
 *
 * 27/07/2018 - LF - First version
 * 10/08/2018 - LF - Force loading order
 * 16/03/2019 - LF - Add .tracker
 */
#include <cstring>
#include <algorithm>

#include <libSelene.h>

#include "Helpers.h"
#include "SubConfigDir.h"
#include "Timer.h"

/* Determine object weight based on its file extension */
static const char * fileext[] = {
	".topic",
	".timer",
	".rendezvous",
	".tracker",
	".lua"
};

static int objectweight( const char *ext ){
	for( unsigned int i = 0; i<sizeof(fileext)/sizeof(const char *); i++){
		if(!strcmp(ext, fileext[i]))
			return (int)i;
	}
	return -1;
}

bool SubConfigDir::accept( const char *fch, std::string &full ){
	if( SortDir::accept( fch, full ) ){
		const char *ext = fileextention( fch );
		return( objectweight(ext) != -1 );
	}
	return false;
}

SubConfigDir::SubConfigDir( Config &cfg, std::string &where, lua_State *L){
	this->readdircontent( where );

	for( iterator i=this->begin(); i<this->end(); i++){
		std::string completpath = where + '/' + *i;
		const char *ext = fileextention( (*i).c_str() );

		if( *i == "Init.lua" ){
			if(configtest){
				publishLog('T', "\t'Init.lua' (Not launched : test mode)");
				continue;
			}

			if(verbose)
				publishLog('L', "\t'Init.lua'");
				
			lua_pushstring(L, where.c_str() );
			lua_setglobal(L, "SELENE_SCRIPT_DIR");
			lua_pushstring(L, (*i).c_str() );
			lua_setglobal(L, "SELENE_SCRIPT_NAME");

			int err = luaL_loadfile(L, completpath.c_str()) || lua_pcall(L, 0, 0, 0);
			if(err){
				publishLog('F', "%s : %s", completpath.c_str(), lua_tostring(L, -1));
				lua_pop(L, 1);  /* pop error message from the stack */
				exit(EXIT_FAILURE);
			}
		} else if( !strcmp(ext,".topic") ){
			std::string name;
			MQTTTopic tpc( completpath, where, name );

			Config::TopicElements::iterator prev;
			if((prev = cfg.TopicsList.find(name)) != cfg.TopicsList.end()){
				publishLog('F', "Topic '%s' is defined multiple times (previous one '%s')", name.c_str(), prev->second.getWhere().c_str());
				exit(EXIT_FAILURE);
			} else {
				cfg.TopicsList.insert( std::make_pair(name, tpc) );
			}
		} else if( !strcmp(ext,".rendezvous") ){
			std::string name;
			Event evt( completpath, where, name );

			Config::EventElements::iterator prev;
			if((prev = cfg.EventsList.find(name)) != cfg.EventsList.end()){
				publishLog('F', "Event '%s' is defined multiple times (previous one '%s')", name.c_str(), prev->second.getWhere().c_str());
				exit(EXIT_FAILURE);
			} else {
				cfg.EventsList.insert( std::make_pair(name, evt) );
			}
		} else if( !strcmp(ext,".timer") ){
			std::string name;
			Timer tmr( completpath, where, name );

			Config::TimerElements::iterator p;
			if((p = cfg.TimersList.find(name)) != cfg.TimersList.end()){
				publishLog('F', "Timer '%s' is defined multiple times (previous one '%s')", name.c_str(), p->second.getWhere().c_str());
				exit(EXIT_FAILURE);
			} else
				cfg.TimersList.insert( std::make_pair(name, tmr) ).first;
		} else if( !strcmp(ext,".lua") ){
			std::string name;
			LuaTask tsk( cfg, completpath, where, name, L );
	
			Config::TaskElements::iterator prev;
			if((prev = cfg.TasksList.find(name)) != cfg.TasksList.end()){
				publishLog('F', "Task '%s' is defined multiple times (previous one '%s')", name.c_str(), prev->second.getWhere().c_str());
				exit(EXIT_FAILURE);
			} else
				cfg.TasksList.insert( std::make_pair(name, tsk) );
		} else if( !strcmp(ext,".tracker") ){
			std::string name;
			Tracker trk( cfg, completpath, where, name, L );
	
			Config::TrackerElements::iterator prev;
			if((prev = cfg.TrackerList.find(name)) != cfg.TrackerList.end()){
				publishLog('F', "Tracker '%s' is defined multiple times (previous one '%s')", name.c_str(), prev->second.getWhere().c_str());
				exit(EXIT_FAILURE);
			} else
				cfg.TrackerList.insert( std::make_pair(name, trk) );
		}
#ifdef DEBUG
else 
	if( debug )
		printf("*d* ignoring %s (ext '%s')\n", (*i).c_str(), ext );
#endif
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
