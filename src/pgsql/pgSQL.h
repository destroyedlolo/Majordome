/* Postgeql database access
 * Reference : https://www.postgresql.org/docs/current/libpq-example.html
 */
#ifndef MAJPGSQL_H
#define MAJPGSQL_H

#include "../Object.h"
#include "../MayBeEmptyString.h"

#include <string>

class pgSQL : public Object {
	MayBeEmptyString conninfo;

	virtual void readConfigDirective( std::string &l, std::string &name, bool &nameused );

public:
	pgSQL(const std::string &fch, std::string &where, std::string &name);
};

#endif
