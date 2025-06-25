#include "ToileObject.h"
#include "../Config.h"

ToileObject::ToileObject() : visible(true) {
}

bool ToileObject::isVisible(void){
	bool ret = this->getOwnVisibility();
	if(!ret && ::debug && this->isVerbose())
		SelLog->Log('D', "[%s/%s] is not visible", this->getWhereC(), this->getNameC());

	return ret;
}

bool ToileObject::readConfigDirective(std::string &l){
	std::string arg;

	if( l == "-->> hidden" ){
		if(::verbose)
			SelLog->Log('C', "\t\tHidden");
		this->visiblity = false;
		return true;
	} else
		return false;
}
