/* Read config directories tree 
 *
 * 26/07/2018 - LF - First version
 * 20/05/2024 - LF - Migrate to v4
 */
#include "Config.h"
#include "Selene.h"
#include "Helpers.h"
#include "SubConfigDir.h"

#include <sys/stat.h>
#include <cstring>

bool Config::accept( const char *fch, std::string &full ){
	if( SortDir::accept( fch, full) ){

		struct stat buff;
		if( stat( full.c_str(), &buff )<0 ){
			SelLog->Log('E', "%s : %s", fch, strerror(errno));
			return false;
		}

		return( !!(buff.st_mode & S_IFDIR) );
	} else
		return false;
}

void Config::init(std::string &where, lua_State *L){
	this->readdircontent(where);

		/* Load packages */
	for( iterator i=this->begin(); i<this->end(); i++){
		if(!quiet)
			SelLog->Log('L', "Loading '%s'", (*i).c_str());

		std::string completpath = where;
		completpath += '/';
		completpath += *i;

		SubConfigDir( *this, completpath, L );
	}
}

LuaTask &Config::findTask( std::string &n ) {
	Config::TaskElements::iterator tsk;

	if( (tsk = this->TasksList.find( n )) == this->TasksList.end() )
		throw 1;
	else
		return (*tsk).second;
}

