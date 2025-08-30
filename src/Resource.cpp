#include "Resource.h"
#include "Config.h"

Resource::Resource( const std::string &file, std::string &where ){
	this->loadConfigurationFile(file, where);

	if(d2)
		fd2 << this->getFullId() << ".class: Topic" << std::endl;

}

bool Resource::readConfigDirective(std::string &l){
	std::string arg;

	if(!(arg = striKWcmp( l, "-->> limit=" )).empty()){
		auto t = stoi(arg);
		if(t <= 0){
			SelLog->Log('F', "Resource's limit can't be negative or nul");
			exit(EXIT_FAILURE);
		} else if(t > 254){
			SelLog->Log('F', "Resource's limit can't be greater than 255");
			t = 255;
		}
		this->limit = t;
		if(::verbose)
			SelLog->Log('C', "\t\tlimit : '%d'", this->limit);
	} else
		return this->Object::readConfigDirective(l);

	return true;
}
