#include <iostream>
#include <fstream>
#include <sstream>	// stringstream

#include <cstring>
#include <cassert>

extern "C" {
    #include "lualib.h"
    #include "lauxlib.h"
};

#include "Config.h"
#include "Helpers.h"
#include "Tracker.h"

Tracker::Tracker( Config &cfg, const std::string &fch, std::string &where, std::string &name, lua_State *L ):status(_status::WAITING){
	if(verbose)
		publishLog('L', "\t'%s'", fch.c_str());

	this->extrName( fch, name );
	this->name = name;
	this->where = where;

	/*
	 * Reading file's content
	 */

	std::stringstream buffer;
	std::ifstream file;
	file.exceptions ( std::ios::eofbit | std::ios::failbit );
	try {
		std::ifstream file(fch);
		std::streampos pos;

		bool nameused = false;	// if so, the name can't be changed anymore

		do {
			std::string l;
			pos = file.tellg();

			std::getline( file, l);
			if( l.compare(0, 2, "--") ){	// End of comments
				file.seekg( pos );
				break;
			}

			MayBeEmptyString arg;
			if( !!(arg = striKWcmp( l, "-->> name=" ))){
				if( nameused ){
					publishLog('F', "\t\tName can be changed only before listen, until or waitfor directives");
					exit(EXIT_FAILURE);
				}

				this->name = name = arg;
				if(verbose)
					publishLog('C', "\t\tChanging name to '%s'", name.c_str());

			} else if( !!(arg = striKWcmp( l, "-->> listen=" ))){
				Config::TopicElements::iterator topic;
				if( (topic = cfg.TopicsList.find(arg)) != cfg.TopicsList.end()){
					if(verbose)
						publishLog('C', "\t\tAdded to topic '%s'", arg.c_str());
	 				topic->second.addTracker( this->getName() );
					nameused = true;
				} else {
					publishLog('F', "\t\tTopic '%s' is not (yet ?) defined", arg.c_str());
					exit(EXIT_FAILURE);
				}
			} else if( !!(arg = striKWcmp( l, "-->> start=" ))){
				Config::TimerElements::iterator timer;
				if( (timer = cfg.TimersList.find(arg)) != cfg.TimersList.end()){
					if(verbose)
						publishLog('C', "\t\tStart timer '%s'", arg.c_str());
	 				timer->second.addStartTracker( this->getName() );
					nameused = true;
				} else {
					publishLog('F', "\t\ttimer '%s' is not (yet ?) defined", arg.c_str());
					exit(EXIT_FAILURE);
				}
			} else if( !!(arg = striKWcmp( l, "-->> stop=" ))){
				Config::TimerElements::iterator timer;
				if( (timer = cfg.TimersList.find(arg)) != cfg.TimersList.end()){
					if(verbose)
						publishLog('C', "\t\tStop timer '%s'", arg.c_str());
	 				timer->second.addStopTracker( this->getName() );
					nameused = true;
				} else {
					publishLog('F', "\t\ttimer '%s' is not (yet ?) defined", arg.c_str());
					exit(EXIT_FAILURE);
				}
			} else if( l == "-->> activated" ){
				if(verbose)
					publishLog('C', "\t\tActivated at startup");
				this->status = _status::CHECKING;
			} else if( l == "-->> disabled" ){
				if(verbose)
					publishLog('C', "\t\tDisabled");
				this->disable();
			}
#if 0
else printf("Ignore '%s'\n", l.c_str());
#endif
		} while(true);

		/*
		 * Reading the remaining of the script and keep it as 
		 * an Lua's script
		 */

		buffer << file.rdbuf();
		file.close();
	} catch(const std::ifstream::failure &e){
		if(!file.eof()){
			publishLog('F', "%s : %s", fch.c_str(), strerror(errno) );
			exit(EXIT_FAILURE);
		}
	}

	if( !this->LoadFunc( L, buffer, this->name.c_str() ))
		exit(EXIT_FAILURE);
}

bool Tracker::exec( const char *name, const char *topic, const char *payload ){
	if( !this->isEnabled() || this->status != _status::CHECKING ){
		if(verbose)
			publishLog('I', "Tracker '%s' from '%s' is disabled or inactive", this->getNameC(), this->getWhereC() );
		return false;
	}

	return this->LuaExec::execSync(name, topic, payload, true);
}

void Tracker::start( void ){
	publishLog('I', "Tracker '%s' is checking", this->getNameC() );
	this->status = _status::CHECKING; 
}

void Tracker::stop( void ){
/*AF: lancer ici toutes les stoppingTasks */
	publishLog('I', "Tracker '%s' is waiting", this->getNameC() );
	this->status = _status::WAITING;
}

void Tracker::done( void ){
	if( this->isEnabled() && this->getStatus() == _status::CHECKING )
		this->execTasks(config, this->getNameC(), NULL, NULL, true);
	publishLog('I', "Tracker '%s' is done", this->getNameC() );
	this->status = _status::DONE;
}