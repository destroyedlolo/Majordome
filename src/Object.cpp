#include "Object.h"
#include "Selene.h"
#include "Helpers.h"

void Object::extrName( const std::string &fch, std::string &name){
	name = fch;
	const size_t last_slash_idx = name.find_last_of("/");	// Filename only
	if(std::string::npos != last_slash_idx)
		name.erase(0, last_slash_idx + 1);

	const size_t period_idx = name.rfind('.');	// Remove extension if present.
	if (std::string::npos != period_idx)
		name.erase(period_idx);
}

	/* Read directives applicables to Object */
bool Object::readConfigDirective(std::string &l){
	if( l == "-->> quiet" ){
		if(verbose)
			SelLog->Log('C', "\t\tBe quiet");
		this->beQuiet();
	} else if( l == "-->> disabled" ){
		if(verbose)
			SelLog->Log('C', "\t\tDisabled");
		this->disable();
	}

	return false;
}
