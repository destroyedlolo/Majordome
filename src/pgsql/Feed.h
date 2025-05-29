/* Add a data to table
 */
#ifndef FEED_H
#define FEED_H

#include "mpgConnection.h"
#include "../Object.h"
#include "../Handler.h"
#include "../ObjCollection.h"

class pgSQL;

class Feed : virtual public mpgConnection, virtual public Handler {
	virtual void feedState(lua_State *L);

		/* Executable */
	virtual bool execAsync(lua_State *L);	// Overloading to handle data acceptation 

#if 0
	bool numerical;	// Numerical value
	bool getNumerical(void){ return this->numerical; };
#endif
protected:
	void readConfigDirective( std::string &l );

	std::string TableName;
protected:
	Feed() = default;

public:
	Feed(const std::string &fch, std::string &where, lua_State *L);


	/* Accessors */
	const char *getTableName(void);
	virtual const char *getNameC(){ return(this->Object::getNameC()); };
	bool isQuiet(){ return !this->Object::isVerbose(); };

	/* Create Lua's object */
	static void initLuaInterface( lua_State *L );

	virtual std::string getTri(){ return Feed::trigramme(); }
	static std::string trigramme(){ return "FED_"; }
};

typedef ObjCollection<Feed *> FeedCollection;
#endif
