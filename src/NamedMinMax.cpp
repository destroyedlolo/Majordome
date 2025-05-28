#include "MinMax.h"
#include "Config.h"
#include "Helpers.h"

#include <fstream>

#include <cstring>
#include <cassert>


NamedMinMax::NamedMinMax(const std::string &fch, std::string &where, lua_State *L) : Object(fch, where), Handler(fch, where){
	this->loadConfigurationFile(fch, where,L);

	if(d2)
		fd2 << this->getFullId() << ".class: NamedMinMax" << std::endl;
}

void NamedMinMax::readConfigDirective( std::string &l ){
	std::string arg;

	if(this->readConfigDirectiveData(l))
		;
	else 
		this->LuaExec::readConfigDirective(l);
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

void NamedMinMax::push(std::string rs,lua_Number val){
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
		SelLog->Log('T', "NamedMinMax ['%s'/'%s'] min:%.0f max:%.0f", this->getNameC(), rs, this->min[rs], this->max[rs]);
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

	this->push(rs, val);

	lua_close(L);
	return r;
}

#if DEBUG
void NamedMinMax::dump(){
	for(auto & it: this->empty){	// Iterating against keys
		std::cout << "\n" << it.first << std::endl << "-------"  << std::endl;
		std::cout << "Number of samples : " << this->getSamplesNumber(it.first) << std::endl;
		std::cout << "Min value : " << this->getMin(it.first) << std::endl;
		std::cout << "Max value : " << this->getMax(it.first) << std::endl;
		std::cout << "Average value : " << this->getAverage(it.first) << std::endl;
	}
}
#endif

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

static int mmm_Push( lua_State *L ){
	class NamedMinMax *minmax= checkMajordomeNamedMinMax(L);
	const char *n = luaL_checkstring(L, 2);
	lua_Number val = luaL_checknumber(L, 3);

	minmax->push(n, val);
	return 0;
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
	{"Push", mmm_Push},
	{"Clear", mmm_Clear},
	{"Reset", mmm_Clear},
	{"FiguresNames", mmm_FiguresNames},
	{NULL, NULL}
};

void NamedMinMax::initLuaInterface( lua_State *L ){
	SelLua->objFuncs( L, "MajordomeNamedMinMax", MajNamedMinMaxM );
	SelLua->libCreateOrAddFuncs( L, "MajordomeNamedMinMax", MajNamedMinMaxLib );
}
