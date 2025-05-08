/* Field to display some information
 *
 *	11/01/2025 - First version
 */

#include "Field.h"
#include "../Config.h"

#include <iostream>
#include <fstream>
#include <sstream>	// stringstream

#include <cstring>
#include <cassert>

Field::Field( const std::string &fch, std::string &where, lua_State *L ) : Object(fch, where), Painting(fch, where), Handler(fch, where){
	this->loadConfigurationFile(fch, where, L);

	if(d2)
		fd2 << this->getFullId() << ".class: Field" << std::endl;


		/* ***
		 * Sanity checks
		 * ***/
	if(!this->parentR && !this->parentP){
		SelLog->Log('F', "[Painting \"%s\"] No parent defined", this->name.c_str());
		exit(EXIT_FAILURE);
	}

	if(!this->geometry.h){
		this->geometry.h = 1;
		if(verbose)
			SelLog->Log('C', "\t\tHeight defaulted to : 1");
	}
}

void Field::readConfigDirective( std::string &l ){
	std::string arg;
	if(!(arg = striKWcmp( l, "-->> Sample=" )).empty()){
		this->geometry.w = arg.length();
		if(verbose)
			SelLog->Log('C', "\t\tWidth guessed to : %u", this->geometry.w);
	} else if(this->Painting::readConfigDirectiveOnly(l))
		;
	else if(this->readConfigDirectiveData(l))
		;
	else if(this->readConfigDirectiveNoData(l))
		;
	else
		this->LuaExec::readConfigDirective(l);
}

void Field::feedState( lua_State *L ){
	class Field **f = (class Field **)lua_newuserdata(L, sizeof(class Field *));
	assert(f);

	lua_pushstring( L, this->getNameC() );	// Push the name of the tracker
	lua_setglobal( L, "MAJORDOME_FIELD" );

	*f = this;
	luaL_getmetatable(L, "MajordomeField");
	lua_setmetatable(L, -2);
	lua_setglobal( L, "MAJORDOME_Myself" );
}

void Field::update(std::string &rs, lua_Number &rn){
	this->refresh();		// Refesh background

	if(!rs.empty())
		this->getSurface()->cb->WriteString(this->getSurface(), rs.c_str());
	else if(!isnan(rn)){
		std::string t = std::to_string(rn);
		this->getSurface()->cb->WriteString(this->getSurface(), t.c_str());
	}
	this->refreshChild();	// Refresh forground
}

bool Field::execAsync(lua_State *L){
	LuaExec::boolRetCode rc;
	lua_Number val;
	std::string s;

	bool r = this->LuaExec::execSync(L, &rc, &val, &s);

	if( rc != LuaExec::boolRetCode::RCfalse ){	// data not rejected
		if(isnan(val) && s.empty()){	// data unchanged
			lua_getglobal(L, "MAJORDOME_PAYLOAD");
			if(lua_isnumber(L, -1))
				val = lua_tonumber(L, -1);
			else if(lua_isstring(L, -1))
				s = lua_tostring(L, -1);
			else {
				SelLog->Log('E', "['%s'] can't find MAJORDOME_PAYLOAD as number", this->getNameC());
				lua_close(L);
				return r;
			}
		}
	
		if(debug)
			SelLog->Log('T', "[Field '%s'] accepting %.0f", this->getNameC(), val);

		this->update(s, val);
	} else
		SelLog->Log('D', "[Field '%s'] Data rejected", this->getNameC());

	lua_close(L);
	return r;
}
