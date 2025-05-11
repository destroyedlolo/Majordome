#include "HandlersExecutor.h"
#include "Config.h"
#include "Helpers.h"

void HandlersExecutor::execHandlers(void){
	if(this->isEnabled()){
#ifdef DEBUG
		if(debug && !this->isQuiet())
			SelLog->Log('D', "[%s] Handlers called", this->getNameC());
#endif

		for(auto &i : *this){
			if(i->isEnabled())
				i->exec(this);
			else
#ifdef DEBUG
				if(debug && !this->isQuiet())
					SelLog->Log('D', "[%s] is disabled", i->getNameC());
#endif

		}

#ifdef DEBUG
	} else if(debug && !this->isQuiet()){
		SelLog->Log('D', "Event %s is disabled : no tasks launched", this->getNameC());
	}
#endif
}

