/* Add a data to table
 */
#ifndef FEED_H
#define FEED_H

#include "mpgConnection.h"
#include "../Object.h"
#include "../Handler.h"
#include "../HandlersExecutor.h"
#include "../ObjCollection.h"
#include "../MayBeEmptyString.h"

class pgSQL;

class Feed : public mpgConnection, public Handler, virtual public HandlersExecutor {
	virtual void readConfigDirective( std::string &l, std::string &name, bool &nameused );
	virtual void feedState(lua_State *L);

		/* Executable */
	virtual bool execAsync(lua_State *L);	// Overloading to handle data acceptation 

	bool isQuiet(){ return this->Object::isQuiet(); };

	MayBeEmptyString TableName;
	const char *getTableName(void);

protected:
	virtual const char *getNameC(){ return(this->Object::getNameC()); };

public:
	virtual ~Feed(){};

	Feed(const std::string &fch, std::string &where, std::string &name, lua_State *L);
};

typedef ObjCollection<Feed *> FeedCollection;
#endif
