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

Field::Field( const std::string &fch, std::string &where, std::string &name, lua_State *L ) : Object(fch, where, name), Painting(fch, where, name), Handler(fch, where, name){
#if DEBUG
	if(verbose)
		SelLog->Log('D', "\t\tid : (%p)", this);
#endif

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

			this->readConfigDirective(l, name, nameused);
		} while(true);

		/*
		 * Reading the remaining of the script and keep it as 
		 * an Lua's script
		 */

		buffer << file.rdbuf();
		file.close();
	} catch(const std::ifstream::failure &e){
		if(!file.eof()){
			SelLog->Log('F', "%s : %s", fch.c_str(), strerror(errno) );
			exit(EXIT_FAILURE);
		}
	}


	if( !this->LoadFunc( L, buffer, this->name.c_str() ))
		exit(EXIT_FAILURE);

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

void Field::readConfigDirective( std::string &l, std::string &name, bool &nameused ){
	MayBeEmptyString arg;
	if(!!(arg = striKWcmp( l, "-->> Sample=" ))){
		this->geometry.w = arg.length();
		if(verbose)
			SelLog->Log('C', "\t\tWidth guessed to : %u", this->geometry.w);
	} else if( !!(arg = striKWcmp( l, "-->> listen=" ))){
		TopicCollection::iterator topic;
		if( (topic = config.TopicsList.find(arg)) != config.TopicsList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tAdded to topic '%s'", arg.c_str());
			topic->second->addHandler( dynamic_cast<Handler *>(this) );
		} else {
			SelLog->Log('F', "\t\tTopic '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else Painting::readConfigDirective(l, name, nameused);
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
		if(isnan(val)){	// data unchanged
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
		SelLog->Log('E', "[Field '%s'] Data rejected", this->getNameC());

	lua_close(L);
	return r;
}
