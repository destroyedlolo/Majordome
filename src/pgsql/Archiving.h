/* Archiving data
 */
#ifndef ARCHIVING_H
#define ARCHIVING_H

#include "mpgConnection.h"
#include "../Object.h"
#include "../Handler.h"
#include "../ObjCollection.h"
#include "../StringVector.h"
#include "../Event.h"

class Archiving : virtual public mpgConnection, virtual public Handler {
	EventVector EventSuccessList;
	EventVector EventFailList;

	void feedState(lua_State *){};
	bool internalExec(void);
protected:
	void readConfigDirective( std::string &l );

	std::string SourceName;
	std::string TableName;
	std::string Aggregation;

		/* Archiving kind */
	enum _kind {
		MINMAX,	// Min, Max and average
		SUM,	// Sumarize
		DELTA	// Delta between the first and the last value
	} kind;

	StringVector keys;
	std::string upto;

		/* Executable */
	virtual bool execAsync(lua_State *L);	// Overloading to handle data acceptation

public:
	Archiving(const std::string &fch, std::string &where);

	/* Accessors */
	const char *getTableName(void);
	bool hasSource(void){ return !this->SourceName.empty(); };
	virtual const char *getNameC(){ return(this->Object::getNameC()); };
	bool isQuiet(){ return this->Object::isQuiet(); };

	virtual std::string getTri(){ return "ARC_"; }
};

typedef ObjCollection<Archiving *> ArchivingCollection;
#endif
