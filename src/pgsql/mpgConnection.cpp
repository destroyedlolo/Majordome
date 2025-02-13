#include "mpgConnection.h"
#include "pgSQL.h"
#include "../Selene.h"

mpgConnection::mpgConnection(pgSQL &s) : conn(NULL) {
puts("constructeur");
	if(s.asConnectionString()){
		this->valide = true;
	} else {
		this->valide = false;
		SelLog->Log('E', "[%s] no connection string", s.getNameC());
	}
}

mpgConnection::~mpgConnection(){
puts("destructeur");
}
