#include "Resource.h"
#include "Config.h"

Resource::Resource( const std::string &file, std::string &where ) : counter(1){
	this->loadConfigurationFile(file, where);

	if(d2)
		fd2 << this->getFullId() << ".class: Resource" << std::endl;

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
		this->counter = t;
		if(::verbose)
			SelLog->Log('C', "\t\tlimit : '%d'", this->counter);
	} else
		return this->Object::readConfigDirective(l);

	return true;
}

bool Resource::acquire(bool w){
	// the Mutex will be automatically released when lock will be destroyed,
	// at the end of this block
	std::unique_lock<std::mutex> lock(this->mutex);

	if(debug)
		SelLog->Log('D', "Resource::acquire()");
		
	if(w)	// wait until the resource is available
		this->cond.wait(lock, [this]() { return this->counter > 0; });
	 else 	// Fail if the resource is not available
		if(this->counter <= 0)
			return false;

	if(debug)
		SelLog->Log('D', "Resource acquired");
		
	--this->counter;
	return true;
}

void Resource::release(void){
	{		// In a block to release the lock before the notification
		std::lock_guard<std::mutex> lock(this->mutex);
		++this->counter;
	}

	if(debug)
		SelLog->Log('D', "Resource released");
		
	this->cond.notify_one();
}
