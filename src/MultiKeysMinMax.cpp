#include "MultiKeysMinMax.h"
#include "Config.h"
#include "Helpers.h"

#include <fstream>

#include <cstring>
#include <cassert>

#if LUA_VERSION_NUM < 503
#	include "compat-5.3.h"
#endif

std::size_t MKuMapH::operator()(const std::vector<std::string> &a) const {
	std::size_t h = 0;

	for(auto e : a)
		h ^= std::hash<std::string>{}(e)  + 0x9e3779b9 + (h << 6) + (h >> 2); 
	return h;
}

MultiKeysMinMax::MultiKeysMinMax(const std::string &fch, std::string &where, lua_State *L) : Object(fch, where), Handler(fch, where), nk(0){
	this->loadConfigurationFile(fch, where,L);

	if(!nk){
		SelLog->Log('F', "NumberOfKeys is missing");
		exit(EXIT_FAILURE);
	}

	if(d2)
		fd2 << this->getFullId() << ".class: MultiKeysMinMax" << std::endl;
}

void MultiKeysMinMax::readConfigDirective( std::string &l ){
	std::string arg;

	if(!(arg = striKWcmp( l, "-->> NumberOfKeys=" )).empty()){
		if(std::stoul(arg)>255 || !std::stoul(arg)){
			SelLog->Log('F', "\t\tNumber of keys can't be > 255 or nul");
			exit(EXIT_FAILURE);
		}
		this->nk = std::stoul(arg);
		if(::verbose)
			SelLog->Log('C', "\t\t%u keys", this->nk);
	} else if(this->readConfigDirectiveData(l))
		;
	else 
		this->LuaExec::readConfigDirective(l);
}

void MultiKeysMinMax::feedState( lua_State *L ){
	class MultiKeysMinMax **nminmax = (class MultiKeysMinMax **)lua_newuserdata(L, sizeof(class MultiKeysMinMax *));
	assert(nminmax);

	lua_pushstring( L, this->getNameC() );	// Push the name of the tracker
	lua_setglobal( L, "MAJORDOME_MULTIKEYMINMAX" );

	*nminmax = this;
	luaL_getmetatable(L, "MajordomeMultiKeysMinMax");
	lua_setmetatable(L, -2);
	lua_setglobal( L, "MAJORDOME_Myself" );
}

void MultiKeysMinMax::push(const std::vector<std::string> &rs,lua_Number val){
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

	if(this->isVerbose()){
		std::string msg;
		for(auto e : rs)
			msg += "/'"+ e +"'";
		SelLog->Log('T', "MultiKeysMinMax ['%s' : %s] n:%.0f min:%.0f max:%.0f sum:%.0f", this->getNameC(), msg.c_str(), this->getSamplesNumber(rs), this->min[rs], this->max[rs], this->sum[rs]);
	}
}

bool MultiKeysMinMax::execAsync(lua_State *L){
	LuaExec::boolRetCode rc;
	std::vector<std::string> rs;
	lua_Number val;

	bool r = this->LuaExec::execSync(L, rs, this->nk, &rc, &val);
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


void MultiKeysMinMax::dump(){
	std::cout << "\n" << this->getName() << std::endl << "======="  << std::endl;
	for(auto & it: this->empty){	// Iterating against keys
		std::cout << "\n";
		for(auto n : it.first)
			std::cout << n << "/";
		std::cout << std::endl << "-------"  << std::endl;

		std::cout << "Number of samples : " << this->getSamplesNumber(it.first) << std::endl;
		std::cout << "Min value : " << this->getMin(it.first) << std::endl;
		std::cout << "Max value : " << this->getMax(it.first) << std::endl;
		std::cout << "Average value : " << this->getAverage(it.first) << std::endl;
		std::cout << "Sum value : " << this->getSum(it.first) << std::endl;
	}
}


	/*****
	 * Lua exposed functions
	 *****/

static class MultiKeysMinMax *checkMajordomeMultiKeysMinMax(lua_State *L){
	class MultiKeysMinMax **r = (class MultiKeysMinMax **)SelLua->testudata(L, 1, "MajordomeMultiKeysMinMax");
	luaL_argcheck(L, r != NULL, 1, "'MajordomeMultiKeysMinMax' expected");
	return *r;
}

void MultiKeysMinMax::readKeys(lua_State *L, int idx, std::vector<std::string> &keys){
	if(!lua_istable(L, idx)){
		SelLog->Log('E', "['%s'] experted an array of keys", this->getNameC());
		exit(EXIT_FAILURE);
	}
	keys.clear();

	lua_pushnil(L);
	while(lua_next(L, -2)){
		if(lua_type(L, -1) == LUA_TSTRING){
			keys.push_back(lua_tostring(L, -1));
		lua_pop(L, 1);	// Remove the value
		} else {
			SelLog->Log('E', "['%s'] experted an array of strings, got something else", this->getNameC());
			exit(EXIT_FAILURE);
		}
	}

	if(!this->keysSanityCheck(keys)){
		SelLog->Log('E', "['%s'] experted an array of %d keys, got %d", this->getNameC(), this->getNumberOfKeys(), keys.size());
		exit(EXIT_FAILURE);
	}
}

static int mmm_find(lua_State *L){
	const char *name = luaL_checkstring(L, 1);

	try {
		class MultiKeysMinMax *mm = config.MultiKeysMinMaxList.at( name );
		class MultiKeysMinMax **minmax = (class MultiKeysMinMax **)lua_newuserdata(L, sizeof(class MultiKeysMinMax *));
		assert(minmax);

		*minmax = mm;
		luaL_getmetatable(L, "MajordomeMultiKeysMinMax");
		lua_setmetatable(L, -2);

		return 1;
	} catch( std::out_of_range &e ){	// Not found 
		return 0;
	}
}

static const struct luaL_Reg MajMultiKeysMinMaxLib [] = {
	{"find", mmm_find},
	{NULL, NULL}
};

static int mmm_getContainer(lua_State *L){
	class MultiKeysMinMax *minmax= checkMajordomeMultiKeysMinMax(L);
	lua_pushstring( L, minmax->getWhereC() );
	return 1;
}

static int mmm_getName(lua_State *L){
	class MultiKeysMinMax *minmax= checkMajordomeMultiKeysMinMax(L);
	lua_pushstring( L, minmax->getName().c_str() );
	return 1;
}

static int mmm_isEnabled( lua_State *L ){
	class MultiKeysMinMax *minmax= checkMajordomeMultiKeysMinMax(L);
	lua_pushboolean( L, minmax->isEnabled() );
	return 1;
}

static int mmm_enabled( lua_State *L ){
	class MultiKeysMinMax *minmax= checkMajordomeMultiKeysMinMax(L);
	minmax->enable();
	return 0;
}

static int mmm_disable( lua_State *L ){
	class MultiKeysMinMax *minmax= checkMajordomeMultiKeysMinMax(L);
	minmax->disable();
	return 0;
}

static int mmm_dump( lua_State *L ){
	class MultiKeysMinMax *minmax= checkMajordomeMultiKeysMinMax(L);
	minmax->dump();
	return 0;
}

static int mmm_getMin( lua_State *L ){
	class MultiKeysMinMax *minmax= checkMajordomeMultiKeysMinMax(L);
	std::vector<std::string> keys;
	minmax->readKeys(L, 2, keys);

	lua_pushnumber( L, minmax->getMin(keys) );
	return 1;
}

static int mmm_getMax( lua_State *L ){
	class MultiKeysMinMax *minmax= checkMajordomeMultiKeysMinMax(L);
	std::vector<std::string> keys;
	minmax->readKeys(L, 2, keys);

	lua_pushnumber( L, minmax->getMax(keys) );
	return 1;
}

static int mmm_getAverage( lua_State *L ){
	class MultiKeysMinMax *minmax= checkMajordomeMultiKeysMinMax(L);
	std::vector<std::string> keys;
	minmax->readKeys(L, 2, keys);

	lua_pushnumber( L, minmax->getAverage(keys) );
	return 1;
}

static int mmm_getSum( lua_State *L ){
	class MultiKeysMinMax *minmax= checkMajordomeMultiKeysMinMax(L);
	std::vector<std::string> keys;
	minmax->readKeys(L, 2, keys);

	lua_pushnumber( L, minmax->getSum(keys) );
	return 1;
}

static int mmm_getSamplesNumber( lua_State *L ){
	class MultiKeysMinMax *minmax= checkMajordomeMultiKeysMinMax(L);
	std::vector<std::string> keys;
	minmax->readKeys(L, 2, keys);

	lua_pushnumber( L, minmax->getSamplesNumber(keys) );
	return 1;
}

static int mmm_Push( lua_State *L ){
	class MultiKeysMinMax *minmax= checkMajordomeMultiKeysMinMax(L);
	std::vector<std::string> keys;
	minmax->readKeys(L, 2, keys);
	lua_Number val = luaL_checknumber(L, 3);

	minmax->push(keys, val);
	return 0;
}

static int mmm_Clear( lua_State *L ){
	class MultiKeysMinMax *minmax= checkMajordomeMultiKeysMinMax(L);
	std::vector<std::string> keys;
	minmax->readKeys(L, 2, keys);
	minmax->Clear(keys);
	return 0;
}

static int mmm_FiguresNames( lua_State *L ){
	/* Instead of using Lua's iterator which will lead to race issue,
	 * We're returning here the list of keys. After all, we are not expecting
	 * to store zillions of keys.
	 */
	class MultiKeysMinMax *minmax= checkMajordomeMultiKeysMinMax(L);

	if(minmax->empty.empty())
		return 0;

	int nbre = 0;
	for(auto &it: minmax->empty){
		nbre++;

		lua_newtable(L);
		size_t i=0;
		for(auto &k: it.first){
			lua_pushstring( L, k.c_str() );
			lua_seti(L, -2, static_cast<lua_Integer>(++i));
		}
	}

	return nbre;
}

static const struct luaL_Reg MajMultiKeysMinMaxM [] = {
	{"getContainer", mmm_getContainer},
 	{"getName", mmm_getName},
	{"isEnabled", mmm_isEnabled},
	{"Enable", mmm_enabled},
	{"Disable", mmm_disable},
	{"dump", mmm_dump},
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

void MultiKeysMinMax::initLuaInterface( lua_State *L ){
	SelLua->objFuncs( L, "MajordomeMultiKeysMinMax", MajMultiKeysMinMaxM );
	SelLua->libCreateOrAddFuncs( L, "MajordomeMultiKeysMinMax", MajMultiKeysMinMaxLib );
}
