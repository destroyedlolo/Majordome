/* Package configuration handling
 *
 * 27/07/2018 - LF - First version
 */
#include <cstring>

#include <libSelene.h>

#include "Components.h"
#include "SubConfigDir.h"

bool SubConfigDir::accept( const char *fch, string &full ){
	if( SortDir::accept( fch, full ) ){
		const char *ext = fileextention( fch );
		if( !strcmp(ext,".lua") )
			return true;
	}
	return false;
}

SubConfigDir::SubConfigDir(string &where, lua_State *L){
	this->readdircontent( where );

	for( iterator i=this->begin(); i<this->end(); i++){
		string completpath = where + '/' + *i;

		if( *i == "Init.lua" ){
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
		} 
else printf("*d* ignoring %s\n", (*i).c_str() );

	}
}
