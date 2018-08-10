/* Package configuration handling
 *
 * 27/07/2018 - LF - First version
 * 10/08/2018 - LF - Force loading order
 */
#include <cstring>
#include <algorithm>

#include <libSelene.h>

#include "Components.h"
#include "SubConfigDir.h"

bool SubConfigDir::accept( const char *fch, std::string &full ){
	if( SortDir::accept( fch, full ) ){
		const char *ext = fileextention( fch );
		if( !strcmp(ext,".lua") ||
			!strcmp(ext,".topic")
		)
			return true;
	}
	return false;
}

SubConfigDir::SubConfigDir( Config &cfg, std::string &where, lua_State *L){
	this->readdircontent( where );

	for( iterator i=this->begin(); i<this->end(); i++){
		std::string completpath = where + '/' + *i;
		const char *ext = fileextention( (*i).c_str() );

		if( *i == "Init.lua" ){
#ifdef DEBUG
	publishLog('L', "\t'Init.lua'");
#endif
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
		} else if( !strcmp(ext,".lua") ){
			std::string name;
			LuaTask tsk( cfg, completpath, where, name, L );
	
			Config::TaskElements::iterator prev;
			if((prev = cfg.TasksList.find(name)) != cfg.TasksList.end()){
				publishLog('F', "Task '%s' is defined multiple times (previous one '%s')", name.c_str(), prev->second.getWhere().c_str());
				exit(EXIT_FAILURE);
			} else
				cfg.TasksList.insert( std::make_pair(name, tsk) );
		}
#ifdef DEBUG
else printf("*d* ignoring %s (ext '%s')\n", (*i).c_str(), ext );
#endif
	}
}

void SubConfigDir::sort( void ){
	std::sort(entries.begin(), entries.end(), 
		[](std::string const &a, std::string const &b) -> bool {
			const char *exta = fileextention( a.c_str() );
			const char *extb = fileextention( b.c_str() );
			int diff = strcmp(extb, exta); // as .topic > .lua but has to be loaded before

			if(!diff)
			    return a < b;
			else
				return diff<0;
		}
	);	
}
