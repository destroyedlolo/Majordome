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
#include "NamedMinMax.h"

NamedMinMax::NamedMinMax( const std::string &fch, std::string &where, std::string &name, lua_State *L ){
	if(verbose)
		SelLog->Log('L', "\t'%s'", fch.c_str());

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
					SelLog->Log('F', "\t\tName can be changed only before listen, until or waitfor directives");
					exit(EXIT_FAILURE);
				}

				this->name = name = arg;
				if(verbose)
					SelLog->Log('C', "\t\tChanging name to '%s'", name.c_str());
			} else if( !!(arg = striKWcmp( l, "-->> listen=" ))){
				Config::TopicElements::iterator topic;
				if( (topic = config.TopicsList.find(arg)) != config.TopicsList.end()){
					if(verbose)
						SelLog->Log('C', "\t\tAdded to topic '%s'", arg.c_str());
	 				topic->second.addNamedMinMax( this->getName() );
					nameused = true;
				} else {
					SelLog->Log('F', "\t\tTopic '%s' is not (yet ?) defined", arg.c_str());
					exit(EXIT_FAILURE);
				}
			} else if( LuaExec::readConfigDirective(l) )
				nameused = true;
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
}

void NamedMinMax::feedState( lua_State *L, const char *name, const char *topic, const char *payload, bool tracker, const char *trkstatus ){
	try {
		class NamedMinMax &mm = config.NamedMinMaxList.at( this->getNameC() );
		class NamedMinMax **minmax = (class NamedMinMax **)lua_newuserdata(L, sizeof(class NamedMinMax *));
		assert(minmax);

		lua_pushstring( L, name );	// Push the name of the tracker
		lua_setglobal( L, "MAJORDOME_NAMEDMINMAX" );

		*minmax = &mm;
		luaL_getmetatable(L, "MajordomeNamedMinMax");
		lua_setmetatable(L, -2);
		lua_setglobal( L, "MAJORDOME_Myself" );
	} catch( std::out_of_range &e ){	// Not found 
		SelLog->Log('F', "Can't find namedminmax '%s'", this->getNameC() );
		exit(EXIT_FAILURE);
	}

	LuaExec::feedState(L, name, topic, payload, tracker, trkstatus);
}

bool NamedMinMax::exec( const char *name, const char *topic, const char *payload ){
	if( !this->isEnabled() ){
		if(verbose)
			SelLog->Log('T', "NamedMinMax'%s' from '%s' is disabled", this->getNameC(), this->getWhereC() );
		return false;
	}

	std::string rs;
	bool r = this->LuaExec::execSync(name, topic, payload, true, NULL, &rs);

	if(!rs.empty()){
		if(debug)
			SelLog->Log('T', "NamedMinMaxi '%s'[%s] from '%s' is accepting \"%s\"", this->getNameC(), rs.c_str(), this->getWhereC(), payload );

		double val = atof(payload);
		auto it = this->empty.find(rs);

		if(this->empty[rs] || it == this->empty.end()){
			this->empty[rs] = false;
			this->nbre[rs] = 1;
			this->min[rs] = this->max[rs] = this->sum[rs] = val; 
		} else {
			if(val < this->min[rs])
				this->min[rs] = val;
			if(val > this->max[rs])
				this->max[rs] = val;

			this->sum[rs] += val;
			this->nbre[rs]++;
		}

		if(debug)
			SelLog->Log('T', "NamedMinMax '%s'[%s] min:%.0f max:%.0f", this->getNameC(), rs.c_str(), this->min[rs], this->max[rs]);
	}

	return r;
}

	/*****
	 * Lua exposed functions
	 *****/

static class NamedMinMax *checkMajordomeNamedMinMax(lua_State *L){
	class NamedMinMax **r = (class NamedMinMax **)SelLua->testudata(L, 1, "MajordomeNamedMinMax");
	luaL_argcheck(L, r != NULL, 1, "'MajordomeNamedMinMax' expected");
	return *r;
}

static int mmm_find(lua_State *L){
	const char *name = luaL_checkstring(L, 1);

	try {
		class NamedMinMax &mm = config.NamedMinMaxList.at( name );
		class NamedMinMax **minmax = (class NamedMinMax **)lua_newuserdata(L, sizeof(class NamedMinMax *));
		assert(minmax);

		*minmax = &mm;
		luaL_getmetatable(L, "MajordomeNamedMinMax");
		lua_setmetatable(L, -2);

		return 1;
	} catch( std::out_of_range &e ){	// Not found 
		return 0;
	}
}

static const struct luaL_Reg MajNamedMinMaxLib [] = {
	{"find", mmm_find},
	{NULL, NULL}
};

#if 0
static int mmm_getContainer(lua_State *L){
	class MinMax *minmax= checkMajordomeMinMax(L);
	lua_pushstring( L, minmax->getWhereC() );
	return 1;
}

static int mmm_getName(lua_State *L){
	class MinMax *minmax= checkMajordomeMinMax(L);
	lua_pushstring( L, minmax->getName().c_str() );
	return 1;
}

static int mmm_isEnabled( lua_State *L ){
	class MinMax *minmax= checkMajordomeMinMax(L);
	lua_pushboolean( L, minmax->isEnabled() );
	return 1;
}

static int mmm_enabled( lua_State *L ){
	class MinMax *minmax= checkMajordomeMinMax(L);
	minmax->enable();
	return 0;
}

static int mmm_disable( lua_State *L ){
	class MinMax *minmax= checkMajordomeMinMax(L);
	minmax->disable();
	return 0;
}

static int mmm_getMin( lua_State *L ){
	class MinMax *minmax= checkMajordomeMinMax(L);
	lua_pushnumber( L, minmax->getMin() );
	return 1;
}

static int mmm_getMax( lua_State *L ){
	class MinMax *minmax= checkMajordomeMinMax(L);
	lua_pushnumber( L, minmax->getMax() );
	return 1;
}

static int mmm_getAverage( lua_State *L ){
	class MinMax *minmax= checkMajordomeMinMax(L);
	lua_pushnumber( L, minmax->getAverage() );
	return 1;
}

static int mmm_getSum( lua_State *L ){
	class MinMax *minmax= checkMajordomeMinMax(L);
	lua_pushnumber( L, minmax->getSum() );
	return 1;
}

#endif
static int mmm_getSamplesNumber( lua_State *L ){
	class NamedMinMax *minmax= checkMajordomeNamedMinMax(L);
	const char *n = luaL_checkstring(L, 2);
	lua_pushnumber( L, minmax->getSamplesNumber(n) );
	return 1;
}

static int mmm_Clear( lua_State *L ){
	class NamedMinMax *minmax= checkMajordomeNamedMinMax(L);
	const char *n = luaL_checkstring(L, 2);
	minmax->Clear(n);
	return 0;
}

static const struct luaL_Reg MajNamedMinMaxM [] = {
#if 0
	{"getContainer", mmm_getContainer},
 	{"getName", mmm_getName},
	{"isEnabled", mmm_isEnabled},
	{"Enable", mmm_enabled},
	{"Disable", mmm_disable},
	{"getMin", mmm_getMin},
	{"getMax", mmm_getMax},
	{"getAverage", mmm_getAverage},
	{"getSum", mmm_getSum},
#endif
	{"getSamplesNumber", mmm_getSamplesNumber},
	{"Clear", mmm_Clear},
	{"Reset", mmm_Clear},
	{NULL, NULL}
};

void NamedMinMax::initLuaObject( lua_State *L ){
	SelLua->objFuncs( L, "MajordomeNamedMinMax", MajNamedMinMaxM );
	SelLua->libCreateOrAddFuncs( L, "MajordomeNamedMinMax", MajNamedMinMaxLib );
}
