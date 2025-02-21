/* Postgeql database access
 * Reference : https://www.postgresql.org/docs/current/libpq-example.html
 */
#ifndef MPGSQL_H
#define MPGSQL_H

#include "../Object.h"
#include "../ObjCollection.h"
#include "../MayBeEmptyString.h"
#include "mpgConnection.h"

#include <string>

class lua_State;

class pgSQL : public Object, public mpgConnection {
	MayBeEmptyString conninfo;

	virtual void readConfigDirective( std::string &l, std::string &name, bool &nameused );

protected:
public:
	virtual const char *getNameC(){ return(this->Object::getNameC()); };
	bool isQuiet(){ return this->Object::isQuiet(); };

	pgSQL(const std::string &fch, std::string &where, std::string &name);

	bool asConnectionString(void){ return(!this->conninfo.isEmpty()); }
	MayBeEmptyString &getConnectionString(void){ return this->conninfo; }

	/* Create Lua's object */
	static void initLuaInterface( lua_State *L );
};

typedef ObjCollection<pgSQL *> pgSQLCollection;
#endif
