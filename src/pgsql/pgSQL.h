/* Postgeql database access
 * Reference : https://www.postgresql.org/docs/current/libpq-example.html
 */
#ifndef MPGSQL_H
#define MPGSQL_H

#include "../Object.h"
#include "../MayBeEmptyString.h"

#include <string>

class pgSQL : public Object {
	MayBeEmptyString conninfo;

	virtual void readConfigDirective( std::string &l, std::string &name, bool &nameused );

public:
	pgSQL(const std::string &fch, std::string &where, std::string &name);

	bool asConnectionString(void){ return(!this->conninfo.isEmpty()); }
	MayBeEmptyString &getConnectionString(void){ return this->conninfo; }

};

#endif
