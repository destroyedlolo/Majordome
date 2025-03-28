#include "MinMax.h"
#include "Config.h"
#include "Helpers.h"

#include <fstream>

#include <cstring>
#include <cassert>


MinMax::MinMax(const std::string &fch, std::string &where, std::string &name, lua_State *L) : Object(fch, where, name), Handler(fch, where, name), empty(true){
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

		/*
		 * Reading header (Majordome's commands)
		 */

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
}

void MinMax::readConfigDirective( std::string &l, std::string &name, bool &nameused ){
	MayBeEmptyString arg;

	if( !!(arg = striKWcmp( l, "-->> listen=" ))){
		TopicCollection::iterator topic;
		if( (topic = config.TopicsList.find(arg)) != config.TopicsList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tAdded to topic '%s'", arg.c_str());
			topic->second->addHandler( dynamic_cast<Handler *>(this) );
//			nameused = true;
		} else {
			SelLog->Log('F', "\t\tTopic '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else 
		this->LuaExec::readConfigDirective(l, name, nameused);
}

void MinMax::feedState( lua_State *L ){
	class MinMax **minmax = (class MinMax **)lua_newuserdata(L, sizeof(class MinMax *));
	assert(minmax);

	lua_pushstring( L, this->getNameC() );	// Push the name of the tracker
	lua_setglobal( L, "MAJORDOME_MINMAX" );

	*minmax = this;
	luaL_getmetatable(L, "MajordomeMinMax");
	lua_setmetatable(L, -2);
	lua_setglobal( L, "MAJORDOME_Myself" );
}

bool MinMax::execAsync(lua_State *L){
#if 0	// Not needed as already checked within LuaExec::canRun() called by MQTTTopic::execHandlers()
	if( !this->isEnabled() ){
		if(verbose)
			SelLog->Log('T', "MinMax'%s' from '%s' is disabled", this->getNameC(), this->getWhereC() );
		return false;
	}
#endif

	LuaExec::boolRetCode rc;
	lua_Number val;

	bool r = this->LuaExec::execSync(L, &rc, &val);

	if( rc != LuaExec::boolRetCode::RCfalse ){	// data not rejected
		if(isnan(val)){	// data unchanged
			lua_getglobal(L, "MAJORDOME_PAYLOAD");
			if(lua_isnumber(L, -1))
				val = lua_tonumber(L, -1);
			else {
				SelLog->Log('E', "['%s'] can't find MAJORDOME_PAYLOAD as number", this->getNameC());
				lua_close(L);
				return r;
			}
		}
	
		if(this->empty){
			this->empty = false;
			this->nbre = 1;
			this->min = this->max = this->sum = val; 
		} else {
			if(val < this->min)
				this->min = val;
			if(val > this->max)
				this->max = val;

			this->sum += val;
			this->nbre++;
		}

		if(debug)
			SelLog->Log('T', "[MinMax '%s'] accepting %.0f -> min:%.0f max:%.0f", this->getNameC(), val, this->min, this->max);
	} else
		SelLog->Log('E', "[MinMax '%s'] Data rejected", this->getNameC());

	lua_close(L);
	return r;
}

	/*****
	 * Lua exposed functions
	 *****/

static class MinMax *checkMajordomeMinMax(lua_State *L){
	class MinMax **r = (class MinMax **)SelLua->testudata(L, 1, "MajordomeMinMax");
	luaL_argcheck(L, r != NULL, 1, "'MajordomeMinMax' expected");
	return *r;
}

static int mmm_find(lua_State *L){
	const char *name = luaL_checkstring(L, 1);

	try {
		class MinMax *mm = config.MinMaxList.at( name );
		class MinMax **minmax = (class MinMax **)lua_newuserdata(L, sizeof(class MinMax *));
		assert(minmax);

		*minmax = mm;
		luaL_getmetatable(L, "MajordomeMinMax");
		lua_setmetatable(L, -2);

		return 1;
	} catch( std::out_of_range &e ){	// Not found 
		return 0;
	}
}

static const struct luaL_Reg MajMinMaxLib [] = {
	{"find", mmm_find},
	{NULL, NULL}
};

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

static int mmm_getSamplesNumber( lua_State *L ){
	class MinMax *minmax= checkMajordomeMinMax(L);
	lua_pushnumber( L, minmax->getSamplesNumber() );
	return 1;
}

static int mmm_Clear( lua_State *L ){
	class MinMax *minmax= checkMajordomeMinMax(L);
	minmax->Clear();
	return 0;
}

static const struct luaL_Reg MajMinMaxM [] = {
	{"getContainer", mmm_getContainer},
 	{"getName", mmm_getName},
	{"isEnabled", mmm_isEnabled},
	{"Enable", mmm_enabled},
	{"Disable", mmm_disable},
	{"getMin", mmm_getMin},
	{"getMax", mmm_getMax},
	{"getAverage", mmm_getAverage},
	{"getSum", mmm_getSum},
	{"getSamplesNumber", mmm_getSamplesNumber},
	{"Clear", mmm_Clear},
	{"Reset", mmm_Clear},
	{NULL, NULL}
};

void MinMax::initLuaInterface( lua_State *L ){
	SelLua->objFuncs( L, "MajordomeMinMax", MajMinMaxM );
	SelLua->libCreateOrAddFuncs( L, "MajordomeMinMax", MajMinMaxLib );
}
