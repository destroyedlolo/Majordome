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
#ifdef DBASE
#	ifdef PGSQL
	{ ".pgsql", 0xc0 },
#	endif
	{ ".feed", 0x70 },
	{ ".namedfeed", 0x70 },
	{ ".archiving", 0x70 },
#endif
	{ ".topic", 0xc0 },
	{ ".timer", 0xc0 },
	{ ".rendezvous", 0xc0 },
	{ ".minmax", 0x80 },
	{ ".namedminmax", 0x80 },
	{ ".tracker", 0x80 },
	{ ".shutdown", 0x50 },
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
	
			TaskCollection::iterator prev;
			if((prev = cfg.TasksList.find(name)) != cfg.TasksList.end()){
				SelLog->Log('F', "Task '%s' is defined multiple times (previous one '%s')", name.c_str(), prev->second->getWhere().c_str());
				exit(EXIT_FAILURE);
			} else
				cfg.TasksList.insert( std::make_pair(name, tsk) );
		} else if(ext == ".shutdown"){
			std::string name;
			auto tsk = new Shutdown( completpath, where, name, L );
			assert(tsk);
	
			ShutdownCollection::iterator prev;
			if((prev = cfg.ShutdownsList.find(name)) != cfg.ShutdownsList.end()){
				SelLog->Log('F', "Shutdown '%s' is defined multiple times (previous one '%s')", name.c_str(), prev->second->getWhere().c_str());
				exit(EXIT_FAILURE);
			} else
				cfg.ShutdownsList.insert( std::make_pair(name, tsk) );
		} else if(ext == ".topic"){
			std::string name;
			auto tpc = new MQTTTopic( completpath, where, name );

			TopicCollection::iterator prev;
			if((prev = cfg.TopicsList.find(name)) != cfg.TopicsList.end()){
				SelLog->Log('F', "Topic '%s' is defined multiple times (previous one '%s')", name.c_str(), prev->second->getWhereC());
				exit(EXIT_FAILURE);
			} else
				cfg.TopicsList.insert( std::make_pair(name, tpc) );
		} else if(ext == ".timer" ){
			std::string name;
			auto tmr = new Timer( completpath, where, name );

			TimerCollection::iterator p;
			if((p = cfg.TimersList.find(name)) != cfg.TimersList.end()){
				SelLog->Log('F', "Timer '%s' is defined multiple times (previous one '%s')", name.c_str(), p->second->getWhereC());
				exit(EXIT_FAILURE);
			} else
				cfg.TimersList.insert( std::make_pair(name, tmr) ).first;
		} else if(ext == ".rendezvous"){
			std::string name;
			auto evt = new Event( completpath, where, name );

			EventCollection::iterator prev;
			if((prev = cfg.EventsList.find(name)) != cfg.EventsList.end()){
				SelLog->Log('F', "Event '%s' is defined multiple times (previous one '%s')", name.c_str(), prev->second->getWhereC());
				exit(EXIT_FAILURE);
			} else
				cfg.EventsList.insert( std::make_pair(name, evt) );
		} else if(ext == ".tracker"){
			std::string name;
			auto trk = new Tracker( completpath, where, name, L );
	
			TrackerCollection::iterator prev;
			if((prev = cfg.TrackersList.find(name)) != cfg.TrackersList.end()){
				SelLog->Log('F', "Tracker '%s' is defined multiple times (previous one '%s')", name.c_str(), prev->second->getWhereC());
				exit(EXIT_FAILURE);
			} else
				cfg.TrackersList.insert( std::make_pair(name, trk) );
		} else if(ext == ".minmax"){
			std::string name;
			auto trk = new MinMax(completpath, where, name, L );
	
			MinMaxCollection::iterator prev;
			if((prev = cfg.MinMaxList.find(name)) != cfg.MinMaxList.end()){
				SelLog->Log('F', "MinMax '%s' is defined multiple times (previous one '%s')", name.c_str(), prev->second->getWhere().c_str());
				exit(EXIT_FAILURE);
			} else
				cfg.MinMaxList.insert( std::make_pair(name, trk) );
		} else if(ext == ".namedminmax"){
			std::string name;
			auto trk = new NamedMinMax(completpath, where, name, L );
	
			NamedMinMaxCollection::iterator prev;
			if((prev = cfg.NamedMinMaxList.find(name)) != cfg.NamedMinMaxList.end()){
				SelLog->Log('F', "NamedMinMax '%s' is defined multiple times (previous one '%s')", name.c_str(), prev->second->getWhere().c_str());
				exit(EXIT_FAILURE);
			} else
				cfg.NamedMinMaxList.insert( std::make_pair(name, trk) );
#ifdef DBASE
#	ifdef PGSQL
		} else if(ext == ".pgsql"){
			std::string name;
			auto pg = new pgSQL( completpath, where, name );

			pgSQLCollection::iterator prev;
			if((prev = cfg.pgSQLsList.find(name)) != cfg.pgSQLsList.end()){
				SelLog->Log('F', "pgsql '%s' is defined multiple times (previous one '%s')", name.c_str(), prev->second->getWhereC());
				exit(EXIT_FAILURE);
			} else
				cfg.pgSQLsList.insert( std::make_pair(name, pg) );
#	endif
		} else if(ext == ".feed"){
			std::string name;
			auto f = new Feed( completpath, where, name, L );

			FeedCollection::iterator prev;
			if((prev = cfg.FeedsList.find(name)) != cfg.FeedsList.end()){
				SelLog->Log('F', "Feed '%s' is defined multiple times (previous one '%s')", name.c_str(), prev->second->getWhereC());
				exit(EXIT_FAILURE);
			} else
				cfg.FeedsList.insert( std::make_pair(name, f) );
		} else if(ext == ".namedfeed"){
			std::string name;
			auto f = new NamedFeed( completpath, where, name, L );

			NamedFeedCollection::iterator prev;
			if((prev = cfg.NamedFeedsList.find(name)) != cfg.NamedFeedsList.end()){
				SelLog->Log('F', "NamedFeed '%s' is defined multiple times (previous one '%s')", name.c_str(), prev->second->getWhereC());
				exit(EXIT_FAILURE);
			} else
				cfg.NamedFeedsList.insert( std::make_pair(name, f) );
		} else if(ext == ".archiving"){
			std::string name;
			auto f = new Archiving( completpath, where, name, L );

			ArchivingCollection::iterator prev;
			if((prev = cfg.ArchivingsList.find(name)) != cfg.ArchivingsList.end()){
				SelLog->Log('F', "Archiving '%s' is defined multiple times (previous one '%s')", name.c_str(), prev->second->getWhereC());
				exit(EXIT_FAILURE);
			} else
				cfg.ArchivingsList.insert( std::make_pair(name, f) );
#endif
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
