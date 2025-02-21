#include "../Helpers.h"
#include "../Selene.h"
#include "mpgConnection.h"
#include "pgSQL.h"

mpgConnection::~mpgConnection(){
	this->disconnect();
}

bool mpgConnection::connect(void){
		/* Ensure the configuration is completed.
		 * Fail if not as it's a configuration issue, not a runtime one
		 */
	if(!this->db){
		SelLog->Log('F', "[%s] Not database associated", this->getNameC());
		exit(EXIT_FAILURE);
	} else if(!db->asConnectionString()){
		SelLog->Log('F', "[%s] no connection string", this->db->getNameC());
		exit(EXIT_FAILURE);
	}

		// Connection to the database
	this->conn = PQconnectdb(this->db->getConnectionString().c_str());
	if(PQstatus(this->conn) != CONNECTION_OK){
		SelLog->Log('E', "[%s] Error : %s", this->db->getNameC(), PQerrorMessage(conn));
		PQfinish(this->conn);
		this->conn = NULL;
		return false;
	}

	if(debug && !this->isQuiet())
		SelLog->Log('T', "[%s] Connected", this->db->getNameC());

	return true;
}

void mpgConnection::disconnect(void){
	if(this->conn){
		if(debug && !this->isQuiet())
			SelLog->Log('T', "[%s] Disconnect", this->db->getNameC());

		PQfinish(this->conn);
		this->conn = NULL;
	}
}

bool mpgConnection::doSQL(const char *q){
	if(debug && !this->isQuiet())
		SelLog->Log('T', "['%s'] Running query : %s", this->getNameC(), q);

	PGresult *res = PQexec(this->conn, q);
	bool ret = (PQresultStatus(res) == PGRES_COMMAND_OK);
	PQclear(res);

	return ret;
}

const char *mpgConnection::lastError(void){
	return PQerrorMessage(conn);
}
