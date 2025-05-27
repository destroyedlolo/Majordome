/* Postgeql database access
 * Reference : https://www.postgresql.org/docs/current/libpq-example.html
 */
#ifndef MPGSQL_H
#define MPGSQL_H

#include "../Object.h"
#include "../ObjCollection.h"
#include "mpgConnection.h"

#include <string>

class lua_State;

class pgSQL : public Object, public mpgConnection {
	std::string conninfo;

	virtual void readConfigDirective( std::string &l );

protected:
public:
	virtual const char *getNameC(){ return(this->Object::getNameC()); };
	bool isQuiet(){ return !this->Object::isVerbose(); };

	pgSQL(const std::string &fch, std::string &where);

	bool asConnectionString(void){ return(!this->conninfo.empty()); }
	std::string &getConnectionString(void){ return this->conninfo; }

	/* Create Lua's object */
	static void initLuaInterface( lua_State *L );

	virtual std::string getTri(){ return pgSQL::trigramme(); }
	static std::string trigramme(){ return "PGS_"; }
};

typedef ObjCollection<pgSQL *> pgSQLCollection;
#endif
