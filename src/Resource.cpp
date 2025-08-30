#include "Resource.h"
#include "Config.h"

Resource::Resource( const std::string &file, std::string &where ){
	this->loadConfigurationFile(file, where);

	if(d2)
		fd2 << this->getFullId() << ".class: Topic" << std::endl;

}

bool Resource::readConfigDirective(std::string &l){
	return this->Object::readConfigDirective(l);
}
