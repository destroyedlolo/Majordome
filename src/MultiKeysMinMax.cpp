#include "MultiKeysMinMax.h"
#include "Config.h"
#include "Helpers.h"

#include <fstream>

#include <cstring>
#include <cassert>

std::size_t MKuMapH::operator()(const std::vector<std::string> &a) const {
	std::size_t h = 0;

	for(auto e : a)
		h ^= std::hash<std::string>{}(e)  + 0x9e3779b9 + (h << 6) + (h >> 2); 
	return h;
}

MultiKeysMinMax::MultiKeysMinMax(const std::string &fch, std::string &where, lua_State *L) : Object(fch, where), Handler(fch, where), nk(0){
	this->loadConfigurationFile(fch, where,L);

	if(d2)
		fd2 << this->getFullId() << ".class: NamedMinMax" << std::endl;
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

void MultiKeysMinMax::push(std::vector<std::string> &rs,lua_Number val){
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

	if(debug && !this->isQuiet()){
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


