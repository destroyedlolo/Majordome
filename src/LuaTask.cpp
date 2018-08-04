#include <iostream>
#include <fstream>
#include <sstream>	// stringstream

#include <cstring>
#include <cassert>

#include "Components.h"
#include "LuaTask.h"

LuaTask::LuaTask( const std::string &fch, std::string &where, std::string &name, lua_State *L ) {
	assert( EStorage_init(&this->func) );

	this->extrName( fch, name );
	this->name = name;
	this->where = where;

	/*
	 * Reading file's content
	 */

	std::ifstream t(fch);
	std::stringstream buffer;
	buffer << t.rdbuf();

	puts( buffer.str().c_str() );

//	lua_load(
}
