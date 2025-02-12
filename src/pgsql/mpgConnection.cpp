#include "mpgConnection.h"
#include "pgSQL.h"
#include "../Selene.h"

mpgConnection::mpgConnection(pgSQL &s){
	if(s.asConnectionString()){
		this->valide = true;
	} else {
		this->valide = false;
		SelLog->Log('E', "[%s] no connection string", s.getNameC());
	}
}
