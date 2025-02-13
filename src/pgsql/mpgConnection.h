/* Implement pgsql connection
 *
 * IMPORTANT NOTE : Usually, it's harmless to let the "C closing code"
 * to free all our resources. But in the case of a database connection,
 * resources are allocated both at the client side (Majordome) but also
 * at the service side (PostgreSQL database).
 * As a consequence, the destructor of this class MUST be called; otherwise,
 * we may face an unclosed connection at the database side.
 */
#ifndef MPGCONN_H
#define MPGCONN_H

#include "pgSQL.h"
#include <libpq-fe.h>

class pgSQL;

class mpgConnection {
	PGconn *conn;
	bool valide;

public:
	mpgConnection() : conn(NULL), valide(false){};
	~mpgConnection();

	bool connect(pgSQL &);
	bool isConnected(void){ return this->valide; }
};

#endif
