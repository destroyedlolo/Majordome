/* Archiving data
 */
#ifndef ARCHIVING_H
#define ARCHIVING_H

#include "mpgConnection.h"
#include "../Object.h"
#include "../Handler.h"
#include "../ObjCollection.h"
#include "../MayBeEmptyString.h"
#include "../StringVector.h"

class Archiving : virtual public mpgConnection, virtual public Handler {
	void feedState(lua_State *){};

protected:
	void readConfigDirective( std::string &l, std::string &name, bool &nameused );

	std::string SourceName;
	MayBeEmptyString TableName;
	std::string Aggregation;

		/* Archiving kind */
	enum _kind {
		MINMAX,	// Min, Max and average
		DELTA	// Delta between the first and the last value
	} kind;

	StringVector keys;
	std::string parameter;

		/* Executable */
	virtual bool execAsync(lua_State *L);	// Overloading to handle data acceptation

public:
	Archiving(const std::string &fch, std::string &where, std::string &name, lua_State *L);

	/* Accessors */
	const char *getTableName(void);
	bool hasSource(void){ return !this->SourceName.empty(); };
	virtual const char *getNameC(){ return(this->Object::getNameC()); };
	bool isQuiet(){ return this->Object::isQuiet(); };

};

typedef ObjCollection<Archiving *> ArchivingCollection;
#endif
