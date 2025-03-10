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

#include <libpq-fe.h>

class pgSQL;

class mpgConnection {
protected:
	pgSQL *db;		// Database to connect to
	PGconn *conn;	// libpg database connection
	PGresult   *res;

			/* Wrapper to Object's methods.
			 * Only as it's needed for logging and I don't want to
			 * inherit from Object.
			 * will be overwritten (see Feed)
			 */
	virtual const char *getNameC(){ return "????"; }
	virtual bool isQuiet() = 0;

public:
	mpgConnection() : db(NULL), conn(NULL), res(NULL){};
	virtual ~mpgConnection();

	bool connect(void);
	void disconnect(void);
	bool isConnected(void){ return !!this->conn; };

	/* Accessors */
	pgSQL *getDatabase(void){ return this->db; };
	PGconn *getConnection(void){ return this->conn; };

	bool doSQL(const char *);		// Execute a simple query
	const char *lastError(void);	// get last error message
};

#endif
