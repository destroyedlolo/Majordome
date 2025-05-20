/* Purging a table
 */
#ifndef PURGE_H
#define PURGE_H

#include "mpgConnection.h"
#include "../Object.h"
#include "../Handler.h"
#include "../ObjCollection.h"
#include "../StringVector.h"
#include "../Event.h"

class Purge : virtual public mpgConnection, virtual public Handler {
protected:
	EventVector EventSuccessList;
	EventVector EventFailList;

	std::string TableName;	// Table to purge
	std::string upto;		// Selection of data to purge

	virtual void readConfigDirective( std::string &l );
	void feedState(lua_State *){};

		/* Executable */
	virtual bool execAsync(lua_State *L);	// Overloading to handle data acceptation
	virtual bool internalExec(void);

	Purge() = default;

public:
	Purge(const std::string &fch, std::string &where);

	/* Accessors */
	const char *getTableName(void);

	virtual const char *getNameC(){ return(this->Object::getNameC()); };
	bool isQuiet(){ return this->Object::isQuiet(); };

	virtual std::string getTri(){ return "PRG_"; }
};

typedef ObjCollection<Purge *> PurgeCollection;
#endif
