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

class Feed : virtual public mpgConnection, virtual public Handler, virtual public HandlersExecutor {
	virtual void feedState(lua_State *L);

		/* Executable */
	virtual bool execAsync(lua_State *L);	// Overloading to handle data acceptation 

#if 0
	bool numerical;	// Numerical value
	bool getNumerical(void){ return this->numerical; };
#endif
protected:
	void readConfigDirective( std::string &l, std::string &name, bool &nameused );

	MayBeEmptyString TableName;
public:
	Feed(const std::string &fch, std::string &where, std::string &name, lua_State *L);
	virtual ~Feed(){};

	/* Accessors */
	const char *getTableName(void);
	virtual const char *getNameC(){ return(this->Object::getNameC()); };
	bool isQuiet(){ return this->Object::isQuiet(); };

	/* Create Lua's object */
	static void initLuaInterface( lua_State *L );
};

typedef ObjCollection<Feed *> FeedCollection;
#endif
