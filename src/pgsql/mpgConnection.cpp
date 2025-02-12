#include "mpgConnection.h"

bool mpgConnection::connect(pgSQL &s){
	if(s.asConnectionString()){
		return true;
	} else
		return false;
}
