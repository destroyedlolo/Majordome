#include "Object.h"
#include "Selene.h"
#include "Helpers.h"

Object::Object(const std::string &fch, std::string &where, std::string &name) : disabled(false), quiet(false){
	if(verbose)
		SelLog->Log('L', "\t'%s'", fch.c_str());

	this->extrName( fch, name );
	this->name = name;
	this->where = where;
}

void Object::extrName( const std::string &fch, std::string &name){
	name = fch;
	const size_t last_slash_idx = name.find_last_of("/");	// Filename only
	if(std::string::npos != last_slash_idx)
		name.erase(0, last_slash_idx + 1);

	const size_t period_idx = name.rfind('.');	// Remove extension if present.
	if (std::string::npos != period_idx)
		name.erase(period_idx);
}

bool Object::readConfigDirective(std::string &l, bool &nameused){
	MayBeEmptyString arg;

	if( !!(arg = striKWcmp( l, "-->> name=" ))){
		if( nameused ){
			SelLog->Log('F', "\t\tName can be changed only before any other directives");
			exit(EXIT_FAILURE);
		}

		this->name = name = arg;
		if(verbose)
			SelLog->Log('C', "\t\tChanging name to '%s'", name.c_str());
	} else if( l == "-->> quiet" ){
		if(verbose)
			SelLog->Log('C', "\t\tBe quiet");
		this->beQuiet();
	} else if( l == "-->> disabled" ){
		if(verbose)
			SelLog->Log('C', "\t\tDisabled");
		this->disable();
	} else if(!! striKWcmp( l, "-->> ")){
		SelLog->Log('F', "Unknown directive '%s'", l.c_str());
		exit(EXIT_FAILURE);
	}

	return false;
}
