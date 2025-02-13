#include "mpgConnection.h"
#include "pgSQL.h"
#include "../Selene.h"

bool mpgConnection::connect(pgSQL &s) {
	if(s.asConnectionString()){
		this->valide = true;

			/* ToDo */
		return true;
	} else {
		this->valide = false;
		SelLog->Log('E', "[%s] no connection string", s.getNameC());
		return false;
	}
}

mpgConnection::~mpgConnection(){
puts("destructeur");
}
