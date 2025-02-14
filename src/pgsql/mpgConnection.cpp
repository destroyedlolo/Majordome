#include "../Helpers.h"
#include "../Selene.h"
#include "mpgConnection.h"
#include "pgSQL.h"

mpgConnection::~mpgConnection(){
puts("destructeur");
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

	return false;
}

void mpgConnection::disconnect(void){
	if(this->conn){
		if(debug)
			SelLog->Log('T', "[%s] Disconnect", this->db->getNameC());

		/* ToDo */

		this->conn = NULL;
	}
}
