#include "MinMax.h"
#include "Config.h"
#include "Helpers.h"

#include <fstream>

#include <cstring>
#include <cassert>


NamedMinMax::NamedMinMax(const std::string &fch, std::string &where, std::string &name, lua_State *L) : Object(fch, where, name), Handler(fch, where, name){
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

void NamedMinMax::readConfigDirective( std::string &l, std::string &name, bool &nameused ){
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

void NamedMinMax::feedState( lua_State *L ){
	class NamedMinMax **nminmax = (class NamedMinMax **)lua_newuserdata(L, sizeof(class NamedMinMax *));
	assert(nminmax);

	lua_pushstring( L, this->getNameC() );	// Push the name of the tracker
	lua_setglobal( L, "MAJORDOME_NAMEDMINMAX" );

	*nminmax = this;
	luaL_getmetatable(L, "MajordomeNamedMinMax");
	lua_setmetatable(L, -2);
	lua_setglobal( L, "MAJORDOME_Myself" );
}

bool NamedMinMax::execAsync(lua_State *L){
	LuaExec::boolRetCode rc;
	std::string rs("orphaned data collection");
	lua_Number val;

	bool r = this->LuaExec::execSync(L, &rs, &rc, &val);
	switch(rc){
	case LuaExec::boolRetCode::RCfalse :	// data rejected
		if(debug && !this->isQuiet())
			SelLog->Log('D', "['%s'] Data rejected", this->getNameC());
		lua_close(L);
		return r;
	
	case LuaExec::boolRetCode::RCforced :	// value provided
		break;

	default:	// Take the payload as data
		lua_getglobal(L, "MAJORDOME_PAYLOAD");
		if(lua_isnumber(L, -1))
			val = lua_tonumber(L, -1);
		else {
			SelLog->Log('E', "['%s'] can't find MAJORDOME_PAYLOAD as number", this->getNameC());
			lua_close(L);
			return r;
		}
		break;
	}

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

	if(debug && !this->isQuiet())
		SelLog->Log('T', "NamedMinMax ['%s'/'%s'] min:%.0f max:%.0f", this->getNameC(), rs.c_str(), this->min[rs], this->max[rs]);

	lua_close(L);
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
		class NamedMinMax *mm = config.NamedMinMaxList.at( name );
		class NamedMinMax **minmax = (class NamedMinMax **)lua_newuserdata(L, sizeof(class NamedMinMax *));
		assert(minmax);

		*minmax = mm;
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

static int mmm_getContainer(lua_State *L){
	class NamedMinMax *minmax= checkMajordomeNamedMinMax(L);
	lua_pushstring( L, minmax->getWhereC() );
	return 1;
}

static int mmm_getName(lua_State *L){
	class NamedMinMax *minmax= checkMajordomeNamedMinMax(L);
	lua_pushstring( L, minmax->getName().c_str() );
	return 1;
}

static int mmm_isEnabled( lua_State *L ){
	class NamedMinMax *minmax= checkMajordomeNamedMinMax(L);
	lua_pushboolean( L, minmax->isEnabled() );
	return 1;
}

static int mmm_enabled( lua_State *L ){
	class NamedMinMax *minmax= checkMajordomeNamedMinMax(L);
	minmax->enable();
	return 0;
}

static int mmm_disable( lua_State *L ){
	class NamedMinMax *minmax= checkMajordomeNamedMinMax(L);
	minmax->disable();
	return 0;
}

static int mmm_getMin( lua_State *L ){
	class NamedMinMax *minmax= checkMajordomeNamedMinMax(L);
	const char *n = luaL_checkstring(L, 2);
	lua_pushnumber( L, minmax->getMin(n) );
	return 1;
}

static int mmm_getMax( lua_State *L ){
	class NamedMinMax *minmax= checkMajordomeNamedMinMax(L);
	const char *n = luaL_checkstring(L, 2);
	lua_pushnumber( L, minmax->getMax(n) );
	return 1;
}

static int mmm_getAverage( lua_State *L ){
	class NamedMinMax *minmax= checkMajordomeNamedMinMax(L);
	const char *n = luaL_checkstring(L, 2);
	lua_pushnumber( L, minmax->getAverage(n) );
	return 1;
}

static int mmm_getSum( lua_State *L ){
	class NamedMinMax *minmax= checkMajordomeNamedMinMax(L);
	const char *n = luaL_checkstring(L, 2);
	lua_pushnumber( L, minmax->getSum(n) );
	return 1;
}

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

static int mmm_FiguresNames( lua_State *L ){
	/* Instead of using Lua's iterator which will lead to race issue,
	 * We're returning here the list of keys. After all, we are not expecting
	 * to store zillions of keys.
	 */
	class NamedMinMax *minmax= checkMajordomeNamedMinMax(L);

	if(minmax->empty.empty())
		return 0;

	int nbre = 0;
	for(auto & it: minmax->empty){
		nbre++;
		lua_pushstring( L, it.first.c_str() );
	}

	return nbre;
}

static const struct luaL_Reg MajNamedMinMaxM [] = {
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
	{"FiguresNames", mmm_FiguresNames},
	{NULL, NULL}
};

void NamedMinMax::initLuaInterface( lua_State *L ){
	SelLua->objFuncs( L, "MajordomeNamedMinMax", MajNamedMinMaxM );
	SelLua->libCreateOrAddFuncs( L, "MajordomeNamedMinMax", MajNamedMinMaxLib );
}
