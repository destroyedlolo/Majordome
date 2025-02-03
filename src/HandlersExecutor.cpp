#include "HandlersExecutor.h"
#include "Config.h"
#include "Helpers.h"

void HandlersExecutor::execHandlers(void){
	if(this->isEnabled()){
#ifdef DEBUG
		if(debug && !this->isQuiet())
			SelLog->Log('D', "[%s] Handlers called", this->getNameC());
#endif

		for(auto &i : *this)
			i->exec();

#ifdef DEBUG
	} else if( debug ){
		SelLog->Log('D', "Event %s is disabled : no tasks launched", this->getNameC());
	}
#endif
}

