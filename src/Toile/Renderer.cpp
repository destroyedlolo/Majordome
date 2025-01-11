/* Renderer object
 * 	Embodies a low-level rendering device.
 */

#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <sstream>	// stringstream

#include <cstring>
#include <cassert>

extern "C" {
    #include "lualib.h"
    #include "lauxlib.h"
};

Renderer::Renderer( const std::string &fch, std::string &where, std::string &name, lua_State *L ) : Object(fch, where, name), surface(NULL), fatal(false){
	if(verbose){
		SelLog->Log('L', "\t'%s'", fch.c_str());
#if DEBUG
		SelLog->Log('D', "\t\tid : (%p)", this);
#endif
	}


	this->extrName( fch, name );
	this->name = name;
	this->where = where;

if(debug) puts("*D**** Renderer::Renderer()");
	/*
	 * Reading file's content
	 */

	std::stringstream buffer;
	std::ifstream file;
	file.exceptions ( std::ios::eofbit | std::ios::failbit );
// if(debug) puts("*D*01");
	try {
		std::ifstream file(fch);
		std::streampos pos;

		bool nameused = false;	// if so, the name can't be changed anymore

		do {
			std::string l;
// if(debug) puts("*D*02");
			pos = file.tellg();
// if(debug) puts("*D*03");

			std::getline( file, l);
// if(debug) puts("*D*04");
			if( l.compare(0, 2, "--") ){	// End of comments
				file.seekg( pos );
				break;
			}
// if(debug) puts("*D*05");

			if( l == "-->> disabled" ){
				SelLog->Log('F', "\t\tDisabled is not supported with renderer");
				exit(EXIT_FAILURE);
			} else if( l == "-->> Fatal" ){
				if(verbose)
					SelLog->Log('C', "\t\tFailure is fatal");
				this->fatal = true;
			} else if( LuaExec::readConfigDirective(l, nameused) )
				nameused = true;
// if(debug) puts("*D*06");
		} while(true);

		/*
		 * Reading the remaining of the script and keep it as 
		 * an Lua's script
		 */

// if(debug) puts("*D*07");
		buffer << file.rdbuf();
// if(debug) puts("*D*08");
		file.close();
// if(debug) puts("*D*09");
	} catch(const std::ifstream::failure &e){
// if(debug) puts("*D*c0");
		if(!file.eof()){
			SelLog->Log('F', "%s : %s", fch.c_str(), strerror(errno) );
			exit(EXIT_FAILURE);
		}
// if(debug) puts("*D*c1");
	}

// if(debug) puts("*D*10");
	if( !this->LoadFunc( L, buffer, this->name.c_str() ))
		exit(EXIT_FAILURE);
if(debug) puts("*D***F Renderer::Renderer()");
}

bool Renderer::exec(){	/* From LuaExec::execSync() */
	if(debug)
		SelLog->Log('D', "Renderer::exec()");

	lua_State *L = luaL_newstate();
	if( !L ){
		SelLog->Log('E', "Unable to create a new Lua State for '%s' from '%s'", this->getNameC(), this->getWhereC() );
		return false;
	}

if(debug) puts("*D* exec 0");
	luaL_openlibs(L);
	threadEnvironment(L);
if(debug) puts("*D* exec 1");

	int err;
	if( (err = SelElasticStorage->loadsharedfunction( L, this->getFunc() )) ){
		SelLog->Log('E', "Unable to Renderer task '%s' from '%s' : %s", this->getNameC(), this->getWhereC(), (err == LUA_ERRSYNTAX) ? "Syntax error" : "Memory error" );
		lua_close( L );
		return false;
	}
if(debug) puts("*D* exec 2");

	if(verbose && !this->isQuiet())
		SelLog->Log('T', "Running Renderer '%s' from '%s'", this->getNameC(), this->getWhereC() );

if(debug) puts("*D* exec 3");
	if(lua_pcall( L, 0, 1, 0)){
		SelLog->Log('E', "Can't execute Renderer '%s' from '%s' : %s", this->getNameC(), this->getWhereC(), lua_tostring(L, -1));
		return false;
	}

if(debug) puts("*D* exec 4");
		/* Notez-Bien : we are checking only for userdata but there is strictly
		 * no way to ensure it's a SelGenericSurface derivate.
		 * Providing bullshits leads to crashing.
		 */
	struct SelGenericSurfaceLua *srf = (struct SelGenericSurfaceLua *)lua_touserdata(L, -1);
if(debug) puts("*D* exec 5");

	if(!srf || !checkCapabilities((SelObject *)srf->storage, SELCAP_RENDERER)){
		SelLog->Log('E', "Not suitable object returned by Renderer code");
		return false;
	}
	this->surface = srf->storage;
if(debug) printf("*D****** surface : %p (%p)\n", srf->storage, this->getSurface());

		/* cleaning */
	lua_close(L);

	if(debug)
		SelLog->Log('D', "Renderer::exec() - End");

	return true;
}

#ifdef DEBUG
void Renderer::dump(){
	std::cout << "Renderer::dump() of " << static_cast<void*>(this) << std::endl;
	std::cout << "\tName : " << this->getName() << std::endl;
	std::cout << "\tWhere : " << this->getWhere() << std::endl;
	std::cout << "\tsurface : " << static_cast<void*>(this->getSurface()) << std::endl;
}
#endif

void Renderer::refresh(){
	for(auto &dn: this->DecorationsList){
		auto d = config.findDecoration(dn);
		d->exec(*this);
	}
}

void Renderer::refreshAll(){
	this->refresh();

		// Refresh subsurface
	for(auto &paint: this->PaintingList)
		paint->refreshAll();
}
