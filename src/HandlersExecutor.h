/* Handler.h
 * Abstract objects that can execute handlers
 */
#ifndef HANDLEREXECUTOR_H
#define HANDLEREXECUTOR_H

#include "Object.h"
#include "Handler.h"

class HandlersExecutor : virtual public Object, public std::vector<Handler *> {
public :
	/* Handlers execution
	 * (runs sequentially)
	 */
	void execHandlers(void);		// fresh State but in the same thread
};

#endif
