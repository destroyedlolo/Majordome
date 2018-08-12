#include "Helpers.h"
#include "Event.h"
#include "Config.h"

void Event::execTasks( Config &cfg, const char *trig_name ){
#ifdef DEBUG
	publishLog('D', "execTasks() : %d to run", this->tasks.size() );
#endif

	for( TaskEntries::iterator tsk = this->tasks.begin(); tsk != this->tasks.end(); tsk++){
		try {
			LuaTask &task = cfg.findTask( *tsk );
			task.exec( trig_name );
		} catch (...) {
			publishLog('F', "Internal error : can't find task \"%s\"", (*tsk).c_str() );
			exit(EXIT_FAILURE);
		}
	}
}
