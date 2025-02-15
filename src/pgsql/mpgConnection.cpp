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

	/* ToDo */
this->conn = (PGconn*)1;

	if(debug && !this->isQuiet())
		SelLog->Log('T', "[%s] Connected", this->db->getNameC());

	return true;
}

void mpgConnection::disconnect(void){
	if(this->conn){
		if(debug && !this->isQuiet())
			SelLog->Log('T', "[%s] Disconnect", this->db->getNameC());

		/* ToDo */

		this->conn = NULL;
	}
}
