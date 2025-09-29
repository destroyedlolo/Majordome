#include "Config.h"
#include "Constraint.h"
#include "Helpers.h"

bool Constraint::readConfigDirective(std::string &l){
	std::string arg;

	if(!(arg = striKWcmp( l, "-->> resourceWait=" )).empty()){
		ResourceCollection::iterator resource;

		if( (resource = config.ResourcesList.find(arg)) != config.ResourcesList.end()){
			this->res = resource->second;
			this->haveToWait = true;

			if(::verbose)
				SelLog->Log('C', "\t\tWill wait for resource '%s'", arg.c_str());

			if(d2)
				fd2 << this->getFullId() << " -- " << resource->second->getFullId() << ": resourceWait { class: lwaitresource }" << std::endl;
			return true;
		} else {
			SelLog->Log('F', "\t\tResource '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else 	if(!(arg = striKWcmp( l, "-->> resourceFail=" )).empty()){
		ResourceCollection::iterator resource;

		if( (resource = config.ResourcesList.find(arg)) != config.ResourcesList.end()){
			this->res = resource->second;
			this->haveToWait = true;

			if(::verbose)
				SelLog->Log('C', "\t\tWill try to acquire resource '%s'", arg.c_str());

			if(d2)
				fd2 << this->getFullId() << " -- " << resource->second->getFullId() << ": resourceFail { class: lfailresource }" << std::endl;
			return true;
		} else {
			SelLog->Log('F', "\t\tResource '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	}

	return false;
}

bool Constraint::tryToAcquireResource(void){
	if(res)
		return res->acquire(false);
	else
		return true;	// We can run if no resource attached
}

bool Constraint::waitForResource(void){
	if(res)
		return res->acquire();
	else
		return true;
}

void Constraint::release(void){
	if(res)
		res->release();
}
