/* Implement pgsql connection */
#ifndef MPGCONN_H
#define MPGCONN_H

#include "pgSQL.h"
#include <libpq-fe.h>

class pgSQL;

class mpgConnection {
	PGconn *conn;
	bool valide;

public:
	mpgConnection(pgSQL &);
};

#endif
