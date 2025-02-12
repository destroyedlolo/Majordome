/* Implement pgsql connection */
#ifndef MPGCONN_H
#define MPGCONN_H

#include "pgSQL.h"
#include <libpq-fe.h>

class mpgConnection {
	PGconn *conn;

public:
	bool connect(pgSQL &);
};

#endif
