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

class mpgConnection {
protected:
	pgSQL *db;		// Database to connect to
	PGconn *conn;	// libpg database connection

			// will be overwritten (see Feed)
	virtual const char *getNameC(){ return "????"; }

public:
	mpgConnection() : db(NULL), conn(NULL){};
	~mpgConnection();

	bool connect(void);
	void disconnect(void);
	bool isConnected(void){ return !!this->conn; }
};

#endif
